#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/reflection_object_notation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReflectionObjectNotation {
    struct Encode : Common {
        using StringStore = Map<usize, uint32_t>;

    private:
        static auto exchange_string(WriteMemoryStream &stream, const std::string_view& data, StringStore &string_index)  -> void {
            auto is_rtid = false;
            auto string = String{data.data(), data.size()};
            if (string.size() > k_rtid_0.size() && string.starts_with("RTID(") && string.ends_with(")")) {
                is_rtid = true;
                if (string == RTIDString::null) {
                    stream.u8(static_cast<u8>(RtonType::string_rtid_null));
                } else {
                    stream.u8(static_cast<u8>(RtonType::string_rtid));
                    string.substring("RTID("_sv.size(), string.size() - k_rtid_empty.size());
                    if (const auto at_position = string.find('@'); at_position != String::none) {
                        auto head_content = string.sub(0_size, at_position);
                        const auto tail_content = string.sub(at_position + 1_size, string.size());
                        const auto sheet_length = compute_utf8_character_length(tail_content);
                        if (head_content.find_all('.').size() == 2_size) {
                            stream.u8(static_cast<u8>(RTIDType::uid));
                            stream.v32(sheet_length);
                            stream.string_v32(tail_content);
                            auto uid_first = 0_ui;
                            auto uid_middle = 0_ui;
                            auto uid_last = 0_ui;
                            const auto scan_result = std::sscanf(head_content.data(), "%u.%u.%x", &uid_first,
                                                                 &uid_middle, &uid_last);
                            assert_conditional(scan_result == 3, "Invalid uid length", "exchange_string");
                            //TODO: add locale
                            stream.v32(uid_middle);
                            stream.v32(uid_first);
                            stream.u32(uid_last);
                        } else {
                            stream.u8(static_cast<u8>(RTIDType::alias));
                            stream.v32(sheet_length);
                            stream.string_v32(tail_content);
                            const auto alias_length = compute_utf8_character_length(head_content);
                            stream.v32(alias_length);
                            stream.string_v32(head_content);
                        }
                    }
                }
            }
            if (!is_rtid) {

                if (const auto string_length = compute_utf8_character_length(string) == string.size()) {
                    const auto hash = hash_string(string.view());
                    if (const auto indexed_string = string_index.find(hash); indexed_string != string_index.end()) {
                        stream.u8(static_cast<u8>(RtonType::string_native_indexed));
                        stream.v32(indexed_string->second);
                    } else {
                        stream.u8(static_cast<uint8_t>(RtonType::string_native_indexing));
                        stream.string_v32(string);
                        string_index.emplace(hash, static_cast<u32>(string_index.size()));
                    }
                } else {
                    const auto hash = hash_string(string.view()) + k_unicode_index;
                    if (const auto indexed_string = string_index.find(hash); indexed_string != string_index.end()) {
                        stream.u8(static_cast<u8>(RtonType::string_unicode_indexed));
                        stream.v32(indexed_string->second);
                    } else {
                        stream.u8(static_cast<uint8_t>(RtonType::string_native_indexing));
                        stream.v32(string_length);
                        stream.string_v32(string);
                        string_index.emplace(hash, static_cast<u32>(string_index.size()));
                    }
                }
            }
        }

        static auto exchange_integer(WriteMemoryStream &stream, i64 const &value) -> void {
            if (value == 0) {
                stream.u8(static_cast<u8>(RtonType::integer_signed_8));
            } else if (value + 0x40000000ll >= 0ll) {
                stream.u8(static_cast<u8>(RtonType::integer_variable_length_signed_32));
                stream.z32(static_cast<i32>(value));
            } else if ((std::numeric_limits<int32_t>::min)() <= value) {
                stream.u8(static_cast<u8>(RtonType::integer_signed_32));
                stream.i32(static_cast<i32>(value));
            } else {
                stream.u8(static_cast<u8>(RtonType::integer_signed_64));
                stream.i64(value);
            }
        }

        static auto exchange_unsigned(WriteMemoryStream &stream, u64 const &value) -> void {
            if (value == 0) {
                stream.u8(static_cast<u8>(RtonType::integer_unsigned_8_zero));
            } else if (value < (std::numeric_limits<int32_t>::max)()) {
                stream.u8(static_cast<u8>(RtonType::integer_variable_length_unsigned_32));
                stream.v32(value);
            } else if (value < (std::numeric_limits<uint64_t>::max)()) {
                stream.u8(static_cast<u8>(RtonType::integer_unsigned_64));
                stream.u64(value);
            } else {
                stream.u8(static_cast<u8>(RtonType::integer_variable_length_unsigned_64));
                stream.v64(value);
            }
        }

        static auto exchange_float(WriteMemoryStream &stream, f64 const &value) -> void {
            if (value == 0) {
                stream.u8(static_cast<u8>(RtonType::floater_signed_64_zero));
            } else {
                stream.u8(static_cast<u8>(RtonType::floater_signed_64));
                stream.f64(value);
            }
        }

        static auto exchange_boolean(WriteMemoryStream &stream, Boolean const &data) -> void {
            return stream.boolean(data);
        }

        static auto exchange_value(WriteMemoryStream &stream, JsonEvent const &event, StringStore &string_index) -> void {
            switch (event.event_type()) {
                case staj_event_type::begin_array:
                    stream.u8(static_cast<u8>(RtonType::array_begin));
                    stream.u8(static_cast<u8>(RtonType::array_size));
                    stream.v32(static_cast<u32>(event.size()));
                    break;
                case staj_event_type::end_array:
                    stream.u8(static_cast<u8>(RtonType::array_end));
                    break;
                case staj_event_type::begin_object:
                    stream.u8(static_cast<u8>(RtonType::object_begin));
                    break;
                case staj_event_type::end_object:
                    stream.u8(static_cast<u8>(RtonType::object_end));
                    break;
                case staj_event_type::string_value:
                case staj_event_type::key:
                    exchange_string(stream, event.get<std::string_view>(), string_index);
                    break;
                case staj_event_type::bool_value:
                    exchange_boolean(stream, event.get<bool>());
                    break;
                case staj_event_type::int64_value:
                    exchange_integer(stream, event.get<int64_t>());
                    break;
                case staj_event_type::uint64_value:
                    exchange_unsigned(stream, event.get<uint64_t>());
                    break;
                case staj_event_type::double_value:
                    exchange_float(stream, event.get<double>());
                    break;
                case staj_event_type::null_value:
                    assert_conditional(false, fmt::format("{}", Language::get("popcap.rton.type_is_null")), "exchange_value");
                default:
                    assert_conditional(false, fmt::format("Unhandled event type: {}", event.event_type()), "exchange_value");
            }
        }

    public:
        static auto process_whole(WriteMemoryStream &stream, const std::string &buffer) {
            auto is = std::istringstream{buffer};
            auto cursor = json_stream_cursor{is};
            stream.allocate_full(buffer.size());
            stream.string(k_magic_identifier);
            stream.u32(k_version);
            auto string_index = StringStore{};
            // auto unicode_string_index = StringStore{};
            const auto &event = cursor.current();
            assert_conditional(event.event_type() == staj_event_type::begin_object, "JSON must be object", "process_whole"); //TODO: add locale.
            cursor.next();
            while (!cursor.done()) {
                exchange_value(stream, cursor.current(), string_index);
                cursor.next();
            }
            stream.string(k_done_identifier);
            is.clear();
        }

        static auto process_fs(
            StringView const &source,
            StringView const &destination) -> void {
            auto stream = WriteMemoryStream{};
            auto buffer = std::string{};
            FileSystem::read_file(source, buffer);
            process_whole(stream, buffer);
            FileSystem::write_file(destination, stream.view());
        }
    };
}
