#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/reflection_object_notation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReflectionObjectNotation
{
    struct Decode : Common
    {
        using StringStore = Map<usize, String>;

    private:
        static auto exchange_string(ReadMemoryStream& stream, RtonType const& type, StringStore& string_index) -> String
        {
            auto content = String{};
            switch (type)
            {
            case RtonType::string_native: {
                content = stream.string_v32();
                break;
            }
            case RtonType::string_native_indexing: {
                content = stream.string_v32();
                string_index[string_index.size()] = content;
                break;
            }
            case RtonType::string_native_indexed: {
                const auto index = stream.v32();
                content = string_index.at(index);
                break;
            }
            case RtonType::string_unicode: {
                auto length = stream.v32();
                content = stream.string_v32();
                break;
            }
            case RtonType::string_unicode_indexing: {
                auto length = stream.v32();
                content = stream.string_v32();
                string_index[string_index.size() + k_unicode_index] = content;
                break;
            }
            case RtonType::string_unicode_indexed: {
                const auto index = stream.v32() + k_unicode_index;
                content = string_index.at(index);
                break;
            }
            case RtonType::string_rtid:
                {
                    switch (RTIDType{stream.u8()}) {
                    case RTIDType::null: {
                        content = String{RTIDString::null.data(), RTIDString::null.size()};
                        break;
                    }
                    case RTIDType::uid: {
                        auto sheet_length = stream.v32();
                        auto sheet_content = stream.string_v32();
                        auto uid_middle = stream.v32();
                        auto uid_first = stream.v32();
                        auto uid_last = stream.u32();
                        content = StringHelper::make_string(fmt::format(RTIDString::uid, uid_first, uid_middle, uid_last, sheet_content.data()));
                        break;
                    }
                    case RTIDType::alias: {
                        auto sheet_length = stream.v32();
                        auto sheet_content = stream.string_v32();
                        auto alias_length = stream.v32();
                        auto alias_content = stream.string_v32();
                        content = StringHelper::make_string(fmt::format(RTIDString::alias, alias_content.data(), sheet_content.data()));
                        break;
                    }
                    default:
                        assert_conditional(false, fmt::format("{}. {}: 0x{:02x}", Kernel::Language::get("popcap.rton.decode.invalid_bytecode"), Kernel::Language::get("offset"), stream.current_position()), "exchange_value");
                    }
                    break;
                }
            case RtonType::string_rtid_null: {
                content = String{RTIDString::null.data(), RTIDString::null.size()};
                break;
            }
            default:
                assert_conditional(false, fmt::format("{}: 0x{:02x}. {}: 0x{:02x}", Kernel::Language::get("popcap.rton.decode.invalid_bytecode"), static_cast<u8>(type), Kernel::Language::get("offset"), stream.current_position()), "exchange_value");
            }
            return content;
        }

        static auto exchange_value(ReadMemoryStream& stream, JsonWriter& value, RtonType const& type, StringStore& string_index) -> void
        {
            //debug(fmt::format("Type: 0x{:02x}. Position: 0x{:02x}", static_cast<u8>(type), stream.current_position()));
            switch (type)
            {
            case RtonType::boolean_false:
                {
                    value.bool_value(false);
                    break;
                }
            case RtonType::boolean_true:
                {
                    value.bool_value(true);
                    break;
                }
            case RtonType::integer_signed_8:
                {
                    value.int64_value(stream.i8());
                    break;
                }
            case RtonType::integer_unsigned_8:
                {
                    value.uint64_value(stream.u8());
                    break;
                }
            case RtonType::integer_signed_16:
                {
                    value.int64_value(stream.i16());
                    break;
                }
            case RtonType::integer_unsigned_16:
                {
                    value.uint64_value(stream.u16());
                    break;
                }
            case RtonType::integer_signed_32:
                {
                    value.int64_value(stream.i32());
                    break;
                }
            case RtonType::integer_unsigned_32:
                {
                    value.uint64_value(stream.u32());
                    break;
                }
            case RtonType::integer_signed_64:
                {
                    value.int64_value(stream.i64());
                    break;
                }
            case RtonType::integer_unsigned_64:
                {
                    value.uint64_value(stream.u64());
                    break;
                }
            case RtonType::floater_signed_32:
                {
                    value.double_value(stream.f32());
                    break;
                }
            case RtonType::floater_signed_64:
                {
                    value.double_value(stream.f64());
                    break;
                }
            case RtonType::integer_signed_8_zero:
            case RtonType::integer_unsigned_8_zero:
            case RtonType::integer_signed_16_zero:
            case RtonType::integer_unsigned_16_zero:
            case RtonType::integer_signed_32_zero:
            case RtonType::integer_unsigned_32_zero:
            case RtonType::integer_signed_64_zero:
            case RtonType::integer_unsigned_64_zero:
                {
                    value.uint64_value(0ull);
                    break;
                }

            case RtonType::floater_signed_32_zero:
            case RtonType::floater_signed_64_zero:
                {
                    value.double_value(0.0);
                    break;
                }
            case RtonType::integer_variable_length_unsigned_32:
            case RtonType::integer_variable_length_unsigned_32_equivalent:
                {
                    value.uint64_value(stream.v32());
                    break;
                }
            case RtonType::integer_variable_length_signed_32:
                {
                    value.int64_value(stream.z32());
                    break;
                }
            case RtonType::integer_variable_length_unsigned_64:
            case RtonType::integer_variable_length_unsigned_64_equivalent:
                {
                    value.uint64_value(stream.v64());
                    break;
                }
            case RtonType::integer_variable_length_signed_64:
                {
                    value.int64_value(stream.z64());
                    break;
                }
            case RtonType::array_begin:
                {
                    value.begin_array();
                    const auto size_byte = stream.u8();
                    assert_conditional(size_byte == static_cast<u8>(RtonType::array_size), fmt::format("{} 0x{:02x}. {}: 0x{:02x}", Kernel::Language::get("popcap.rton.decode.invalid_rton_array_size"), size_byte, Kernel::Language::get("offset"), stream.current_position()), "exchange_value"); //TODO: add locale.
                    for (const auto size = stream.v32(); auto _ : Range{size})
                    {
                        const auto value_type = RtonType{stream.u8()};
                        exchange_value(stream, value, value_type, string_index);
                    }
                    const auto end_byte = stream.u8();
                    assert_conditional(end_byte == static_cast<u8>(RtonType::array_end), fmt::format("{} 0x{:02x}. {}: 0x{:02x}", Kernel::Language::get("popcap.rton.decode.invalid_rton_array_end"), end_byte, Kernel::Language::get("offset"), stream.current_position()), "exchange_value");
                    value.end_array();
                    break;
                }
            case RtonType::object_begin:
                {
                    value.begin_object();
                    while (true)
                    {
                        const auto key_type = stream.u8();
                        if (key_type == static_cast<u8>(RtonType::object_end))
                        {
                            value.end_object();
                            break;
                        }
                     //   auto key_string = String{};
                        auto key_string = exchange_string(stream, static_cast<RtonType>(key_type), string_index);
                        value.key(key_string);
                        const auto value_type = RtonType{stream.u8()};
                        exchange_value(stream, value, value_type, string_index);
                    }
                    break;
                }

            default:
                {
                    auto string = exchange_string(stream, type, string_index);
                    value.string_value(string);
                    break;
                }
            }
        }

    public:
        static auto process_whole(ReadMemoryStream& stream, std::ostringstream& os) -> void
        {
            assert_conditional(stream.string(4_size) == k_magic_identifier, fmt::format("{}", Kernel::Language::get("popcap.rton.decode.invalid_rton_magic")), "process_whole");
            stream += 4_size; //Skip version read.
            auto string_index = StringStore{};
            auto writer = JsonWriter{os};
            exchange_value(stream, writer, RtonType::object_begin, string_index);
            assert_conditional(stream.string(4_size) == k_done_identifier, fmt::format("{}", Language::get("popcap.rton.done_invalid")), "process_whole");
            writer.flush();
        }

        static auto process_fs(
            StringView const& source,
            StringView const& destination
        ) -> void
        {
            auto stream = ReadMemoryStream{source};
            auto os = std::ostringstream{};
            process_whole(stream, os);
            auto buffer = CharacterArrayView{const_cast<char*>(os.view().data()), os.view().size()};
            FileSystem::write_file(destination, buffer);
            os.clear();
        }
    };
}
