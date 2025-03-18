#pragma once

#include "kernel/support/popcap/text_table/common.hpp"

namespace Sen::Kernel::Support::PopCap::TextTable {

    struct Convert : Common {

        static auto process_text(
            std::string_view source,
            List<String>& destination
        ) -> void {
            while (!source.empty()) {
                if (auto key_match = Subprojects::ctre::search<key_regex>(source)) {
                    auto key = key_match.get<1>().to_string();
                    source.remove_prefix(key_match.view().end() - source.begin());
                    if (auto value_match = Subprojects::ctre::search<value_regex>(source)) {
                        auto value = value_match.get<1>().to_string();
                        while (!value.empty() && value.back() == '\n') {
                            value.pop_back();
                        }
                        destination.append(StringHelper::make_string(key));
                        destination.append(StringHelper::make_string(value));
                        source.remove_prefix(value_match.view().end() - source.begin());
                    }
                } else {
                    break;
                }
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
                process_text(source_text.view(), result);
            }
            if constexpr (from == Type::utf16_text) {
                auto source_text = FileSystem::read_utf16(source);
                process_text(source_text.view(), result);
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
                auto data = [&] () -> LawnStrings<true> {
                    return LawnStrings<true>{
                        1,
                        make_list<ObjectEntry<true>>([] () -> ObjectEntry<true> {
                            return ObjectEntry<true>{
                                make_list<String>("LawnStringsData"_s),
                                "LawnStringsData"_s,
                                [&]() -> ObjectData {
                                    return ObjectData {
                                        LinearMap<String, String>{}
                                    };
                                }(),
                            };
                        }()),
                    };
                }();
                auto& LocStringValues = data.objects[0].objdata.LocStringValues;
                for (auto index = 0_size; index < result.size(); index += 2) {
                    LocStringValues.append(std::make_pair(result[index], result[index + 1]));
                }
                FileSystem::write_json(destination, data);
            }
            if constexpr (to == Type::json_array) {
                auto data = [&]() -> LawnStrings<false> {
                    return LawnStrings<false> {
                        1,
                        make_list<ObjectEntry<false>>([&]() -> ObjectEntry<false> {
                            return ObjectEntry<false>{
                                make_list<String>("LawnStringsData"_s),
                                "LawnStringsData"_s,
                                [&]() -> ObjectList {
                                    return ObjectList{
                                        std::move(result),
                                    };
                                }(),
                            };
                        }()),
                    };
                }();                
                FileSystem::write_json(destination, data);
            }
        }

        static constexpr auto process_table = std::to_array({
            std::to_array({process_whole<Type::utf16_text, Type::utf16_text>, process_whole<Type::utf16_text, Type::utf8_text>, process_whole<Type::utf16_text, Type::json_map>, process_whole<Type::utf16_text, Type::json_array>}),
            std::to_array({process_whole<Type::utf8_text, Type::utf16_text>, process_whole<Type::utf8_text, Type::utf8_text>, process_whole<Type::utf8_text, Type::json_map>, process_whole<Type::utf8_text, Type::json_array>}),
            std::to_array({process_whole<Type::json_map, Type::utf16_text>, process_whole<Type::json_map, Type::utf8_text>, process_whole<Type::json_map, Type::json_map>, process_whole<Type::json_map, Type::json_array>}),
            std::to_array({process_whole<Type::json_array, Type::utf16_text>, process_whole<Type::json_array, Type::utf8_text>, process_whole<Type::json_array, Type::json_map>, process_whole<Type::json_array, Type::json_array>})
        });

        static auto process_fs(
            const StringView& source,
            const StringView& destination,
            const Type& from,
            const Type& to
        ) -> void {
            const auto from_index = type_index(from);
            const auto to_index = type_index(to);
            return process_table[from_index.operator*()][to_index.operator*()](source, destination);
        }


    };


}