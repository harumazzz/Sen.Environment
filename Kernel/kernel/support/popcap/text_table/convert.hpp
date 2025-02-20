#pragma once

#include "kernel/support/popcap/text_table/common.hpp"

namespace Sen::Kernel::Support::PopCap::TextTable {

    struct Convert : Common {

        static auto process_text (
            String& source_text,
            List<String>& destination
        ) -> void {
            auto source = source_text.string();
            std::regex key_regex(R"(\[.+?\])");  // Matches section headers
            std::regex value_regex(
                R"([\s\S]*?(?=\n*\[|$))");

            std::sregex_iterator key_it(source.begin(), source.end(),
                                key_regex);
            std::sregex_iterator key_end;

            size_t search_pos = 0;

            while (key_it != key_end) {
                std::smatch key_match = *key_it;
                std::string key = key_match.str().substr(
                    1, key_match.length() - 2);  // Remove brackets

                search_pos = key_match.position() + key_match.length() +
                             1;  // Move past section header

                std::string remaining_text = source.substr(search_pos);
                std::smatch value_match;

                if (std::regex_search(remaining_text, value_match, value_regex)) {
                    destination.append(StringHelper::make_string(key));
                    destination.append(StringHelper::make_string(value_match.str(0)));
                    search_pos += value_match.length();  // Move past value
                }

                ++key_it;  // Move to next section
            }
        }

        static auto process_map (
            Subprojects::jsoncons::json& source,
            List<String>& destination
        ) -> void {
            assert_conditional(source.contains("objects") && source["objects"].is_array(), "Expected objects to be an array", "process_map");
            auto& objects = source["objects"];
            assert_conditional(!objects.empty() && objects[0].contains("objdata"), "Expected objects[0] to contain objdata", "process_map");
            auto& objdata = objects[0]["objdata"];
            assert_conditional(objdata.contains("LocStringValues"), "Expected objdata to contain LocStringValues", "process_map");
            auto& locStringValues = objdata["LocStringValues"];
            assert_conditional(locStringValues.is_object(), "Expected LocStringValues must be an object", "process_map");
            for (const auto & [key, value] : locStringValues.object_range()) {
                assert_conditional(value.is_string(), "Expected value must be a string", "process_map");
                destination.append(StringHelper::make_string(key));
                destination.append(StringHelper::make_string(value.template as<std::string_view>()));
            }
        }

        static auto process_array (
            Subprojects::jsoncons::json& source,
            List<String>& destination
        ) -> void {
            assert_conditional(source.contains("objects") && source["objects"].is_array(), "Expected objects to be an array", "process_array");
            auto& objects = source["objects"];
            assert_conditional(!objects.empty() && objects[0].contains("objdata"), "Expected objects[0] to contain objdata", "process_array");
            auto& objdata = objects[0]["objdata"];
            assert_conditional(objdata.contains("LocStringValues"), "Expected objdata to contain LocStringValues", "process_array");
            auto& locStringValues = objdata["LocStringValues"];
            assert_conditional(locStringValues.is_array(), "Expected LocStringValues must be an Array", "process_array");
            for (const auto& value : locStringValues.array_range()) {
                assert_conditional(value.is_string(), "Expected value must be a string", "process_array");
                destination.append(StringHelper::make_string(value.template as<std::string_view>()));
            }
        }

        template <auto from, auto to> requires std::is_same_v<type_of<from>, type_of<to>> && std::is_same_v<type_of<from>, Type>
        static auto process_whole (
            const StringView& source,
            const StringView& destination
        ) -> void {
            if constexpr (from == to) {
                assert_conditional(false, "Invalid type", "process_whole");
            }
            auto result = List<String>{};
            if constexpr (from == Type::utf8_text) {
                auto source_text = FileSystem::read_utf8_bom(source);
                process_text(source_text, result);
            }
            if constexpr (from == Type::utf16_text) {
                auto source_text = FileSystem::read_utf16(source);
                process_text(source_text, result);
            }
            if constexpr (from == Type::json_map) {
                auto data = Subprojects::jsoncons::json{};
                FileSystem::read_json(source, data);
                process_map(data, result);
            }
            if constexpr (from == Type::json_array) {
                auto data = Subprojects::jsoncons::json{};
                FileSystem::read_json(source, data);
                process_array(data, result);
            }
            if constexpr (to == Type::utf8_text) {
                auto data = String{};
                data.allocate_full(std::accumulate(result.begin(), result.end(), 0_size, [](auto&& a, auto &b) {
                    return a + b.size();
                }));
                for (auto & e : result) {
                    data += e;
                    data += '\n';
                }
                FileSystem::write_file_s<false, true>(destination, data);
            }
            if constexpr (to == Type::utf16_text) {
                auto data = String{};
                data.allocate_full(std::accumulate(result.begin(), result.end(), 0_size, [](auto&& a, auto &b) {
                    return a + b.size();
                }));
                for (auto & e : result) {
                    data += e;
                    data += '\n';
                }
                FileSystem::write_file_s<true, true>(destination, data);
            }
            if constexpr (to == Type::json_map) {
                auto data = MapLawnStrings{
                    .version = 1,
                    .objects = make_list<ObjectEntry<true>>(ObjectEntry<true>{
                        .aliases = make_list<String>("LawnStringsData"_s),
                        .objclass = "LawnStringsData"_s,
                        .objdata = ObjectData{
                            .LocStringValues = HashMap<String, String>{},
                        },
                    })
                };
                auto& LocStringValues = data.objects[0].objdata.LocStringValues;
                for (auto index = 0_size; index < result.size(); index += 2) {
                    LocStringValues.emplace(result[index], result[index + 1]);
                }
                FileSystem::write_json(destination, data);
            }
            if constexpr (to == Type::json_array) {
                auto data = static_cast<Subprojects::jsoncons::ojson>(Subprojects::jsoncons::json::object({
                    {"version", 1},
                    {"objects", json::array({
                        json::object({
                            {"aliases", json::array({"LawnStringsData"})},
                            {"objclass", "LawnStringsData"},
                            {"objdata", json::object({
                                {"LocStringValues", json::array()},
                            })}
                        })
                    })}
                }));
                auto &LocStringValues = data["objects"][0]["objdata"]["LocStringValues"];
                for (auto index : Range{result.size()}) {
                    LocStringValues.push_back(result[index]);
                }
                FileSystem::write_json(destination, data);
            }
        }

        template <auto From, auto... Is> requires std::is_same_v<type_of<From>, Type> && (std::is_same_v<type_of<Is>, usize> && ...)
        static auto process(
            const StringView& source,
            const StringView& destination,
            const Type& to,
            std::index_sequence<Is...>
        ) -> void {
            ((to == Detail::all_types[Is] ? (process_whole<From, Detail::all_types[Is]>(source, destination), void()) : void()), ...);
        }

        static auto process_fs(
            const StringView& source,
            const StringView& destination,
            const Type& from,
            const Type& to
        ) -> void {
            process_whole<Type::utf16_text, Type::json_map>(source, destination);
        }

    };


}