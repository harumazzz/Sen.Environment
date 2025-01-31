#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/reflection_object_notation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReflectionObjectNotation
{
    
    struct Decode : Common
    {
    private:

        using Writer = Encoding::JSON::IndentWriter;
        
    protected:
        template <auto write_key>
        inline static auto exchange_value(
            DataStreamView &stream,
            Writer &value,
            List<std::string> &native_string_index,
            List<std::string> &unicode_string_index,
            TypeIdentifierEnumeration::Type const &type) -> void
        {
            static_assert(write_key == true || write_key == false, "write_key must be true or false");
            switch (type)
            {
            case TypeIdentifierEnumeration::Type::boolean_false:
            {
                value.write_boolean(false);
                break;
            }
            case TypeIdentifierEnumeration::Type::boolean_true:
            {
                value.write_boolean(true);
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_8:
            {
                value.write_value(stream.readInt8());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_8_zero:
            {
                value.write_value(static_cast<int8_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_8:
            {
                value.write_value(stream.readUint8());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_8_zero:
            {
                value.write_value(static_cast<uint8_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_16:
            {
                value.write_value(stream.readInt16());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_16_zero:
            {
                value.write_value(static_cast<int16_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_16:
            {
                value.write_value(stream.readUint16());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_16_zero:
            {
                value.write_value(static_cast<uint16_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_32:
            {
                value.write_value(stream.readInt32());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_32_zero:
            {
                value.write_value(static_cast<int32_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_32:
            {
                value.write_value(stream.readUint32());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_32_zero:
            {
                value.write_value(static_cast<uint32_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_64:
            {
                value.write_value(stream.readInt64());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_signed_64_zero:
            {
                value.write_value(static_cast<int64_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_64:
            {
                value.write_value(stream.readUint64());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_unsigned_64_zero:
            {
                value.write_value(static_cast<uint64_t>(0));
                break;
            }
            case TypeIdentifierEnumeration::Type::floater_signed_32:
            {
                value.write_value(stream.readFloat());
                break;
            }
            case TypeIdentifierEnumeration::Type::floater_signed_32_zero:
            {
                value.write_value(static_cast<float>(0.0f));
                break;
            }
            case TypeIdentifierEnumeration::Type::floater_signed_64:
            {
                value.write_value(stream.readDouble());
                break;
            }
            case TypeIdentifierEnumeration::Type::floater_signed_64_zero:
            {
                value.write_value(static_cast<double>(0.0));
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_variable_length_unsigned_32:
            case TypeIdentifierEnumeration::Type::integer_variable_length_unsigned_32_equivalent:
            {
                value.write_value(stream.readVarInt32());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_variable_length_signed_32:
            {
                value.write_value(stream.readZigZag32());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_variable_length_unsigned_64:
            case TypeIdentifierEnumeration::Type::integer_variable_length_unsigned_64_equivalent:
            {
                value.write_value(stream.readVarInt64());
                break;
            }
            case TypeIdentifierEnumeration::Type::integer_variable_length_signed_64:
            {
                value.write_value(stream.readZigZag64());
                break;
            }
            case TypeIdentifierEnumeration::Type::string_native:
            {
                if constexpr (write_key)
                {
                    value.write_property_name(stream.readStringByVarInt32());
                }
                else
                {
                    value.write_value(stream.readStringByVarInt32());
                }
                break;
            }
            case TypeIdentifierEnumeration::Type::string_native_indexing:
            {

                auto content = stream.readStringByVarInt32();
                if constexpr (write_key)
                {
                    value.write_property_name(content);
                }
                else
                {
                    value.write_value(content);
                }
                native_string_index.emplace_back(content);
                break;
            }
            case TypeIdentifierEnumeration::Type::string_native_indexed:
            {
                auto index = stream.readVarInt32();
                if constexpr (write_key)
                {
                    value.write_property_name(native_string_index[index]);
                }
                else
                {
                    value.write_value(native_string_index[index]);
                }
                break;
            }
            case TypeIdentifierEnumeration::Type::string_unicode:
            {
                auto length = stream.readVarInt32();
                auto content = stream.readStringByVarInt32();
                if constexpr (write_key)
                {
                    value.write_property_name(content);
                }
                else
                {
                    value.write_value(content);
                }
                break;
            }
            case TypeIdentifierEnumeration::Type::string_unicode_indexing:
            {
                auto length = stream.readVarInt32();
                auto content = stream.readStringByVarInt32();
                if constexpr (write_key)
                {
                    value.write_property_name(content);
                }
                else
                {
                    value.write_value(content);
                }
                unicode_string_index.emplace_back(content);
                break;
            }
            case TypeIdentifierEnumeration::Type::string_unicode_indexed:
            {
                auto index = stream.readVarInt32();
                if constexpr (write_key)
                {
                    value.write_property_name(unicode_string_index[index]);
                }
                else
                {
                    value.write_value(unicode_string_index[index]);
                }
                break;
            }
            case TypeIdentifierEnumeration::Type::string_rtid:
            {
                switch (RTIDTypeIdentifierEnumeration::Type{stream.readUint8()})
                {
                case RTIDTypeIdentifierEnumeration::Type::null:
                {
                    if constexpr (write_key)
                    {
                        value.write_property_name(RTIDString::null);
                    }
                    else
                    {
                        value.write_value(RTIDString::null);
                    }
                    break;
                }
                case RTIDTypeIdentifierEnumeration::Type::uid:
                {
                    auto sheet_length = stream.readVarInt32();
                    auto sheet_content = stream.readStringByVarInt32();
                    auto uid_middle = stream.readVarInt32();
                    auto uid_first = stream.readVarInt32();
                    auto uid_last = stream.readUint32();
                    if constexpr (write_key)
                    {
                        value.write_property_name(fmt::format(RTIDString::uid, uid_first, uid_middle, uid_last, sheet_content));
                    }
                    else
                    {
                        value.write_value(fmt::format(RTIDString::uid, uid_first, uid_middle, uid_last, sheet_content));
                    }

                    break;
                }
                case RTIDTypeIdentifierEnumeration::Type::alias:
                {
                    auto sheet_length = stream.readVarInt32();
                    auto sheet_content = stream.readStringByVarInt32();
                    auto alias_length = stream.readVarInt32();
                    auto alias_content = stream.readStringByVarInt32();
                    if constexpr (write_key)
                    {
                        value.write_property_name(fmt::format(RTIDString::alias, alias_content, sheet_content));
                    }
                    else
                    {
                        value.write_value(fmt::format(RTIDString::alias, alias_content, sheet_content));
                    }

                    break;
                }
                default:
                    assert_conditional(false, fmt::format("{}. {}: 0x{:02x}", Kernel::Language::get("popcap.rton.decode.invalid_bytecode"), Kernel::Language::get("offset"), stream.read_pos), "exchange_value");
                }
                break;
            }
            case TypeIdentifierEnumeration::Type::string_rtid_null:
            {
                value.write_value(RTIDString::null);
                break;
            }
            case TypeIdentifierEnumeration::Type::array_begin:
            {
                value.write_start_array();
                assert_conditional(TypeIdentifierEnumeration::Type{stream.readUint8()} == TypeIdentifierEnumeration::Type::array_size, fmt::format("{} {:02x}. {}: {:02x}", Kernel::Language::get("popcap.rton.decode.invalid_rton_array_starts"), static_cast<uint8_t>(TypeIdentifierEnumeration::Type::array_size), Kernel::Language::get("offset"), stream.read_pos), "exchange_value");
                auto size = stream.readVarInt32();
                auto array_size = 0;
                while (true)
                {
                    auto value_type_identifier = TypeIdentifierEnumeration::Type{stream.readUint8()};
                    if (value_type_identifier == TypeIdentifierEnumeration::Type::array_end)
                    {
                        break;
                    }
                    ++array_size;
                    exchange_value<false>(stream, value, native_string_index, unicode_string_index, value_type_identifier);
                }
                assert(array_size == size);
                value.write_end_array();
                break;
            }
            case TypeIdentifierEnumeration::Type::object_begin:
            {
                value.write_start_object();
                while (true)
                {
                    auto key_type_identifier = TypeIdentifierEnumeration::Type{stream.readUint8()};
                    if (key_type_identifier == TypeIdentifierEnumeration::Type::object_end)
                    {
                        break;
                    }
                    exchange_value<true>(stream, value, native_string_index, unicode_string_index, key_type_identifier);
                    auto value_type_identifier = TypeIdentifierEnumeration::Type{stream.readUint8()};
                    exchange_value<false>(stream, value, native_string_index, unicode_string_index, value_type_identifier);
                }
                value.write_end_object();
                break;
            }
            default:
                assert_conditional(false, fmt::format("{}. {}: 0x{:02x}", Kernel::Language::get("popcap.rton.decode.invalid_bytecode"), Kernel::Language::get("offset"), stream.read_pos), "exchange_value");
            }
            return;
        }

    public:
        inline static auto process_whole(
            DataStreamView &stream,
            Writer &value) -> void
        {
            assert_conditional(stream.readString(4) == k_magic_identifier, fmt::format("{}", Kernel::Language::get("popcap.rton.decode.invalid_rton_magic")), "process_whole");
            {
                auto version = stream.readUint32();
                assert_conditional(version == k_version, format(fmt::format("{}", Language::get("popcap.rton.version_is_invalid")), std::to_string(version)), "process_whole");
            }
            auto native_string_index = List<std::string>{};
            auto unicode_string_index = List<std::string>{};
            exchange_value<false>(stream, value, native_string_index, unicode_string_index, TypeIdentifierEnumeration::Type::object_begin);
            assert_conditional(stream.readString(4) == k_done_identifier, fmt::format("{}", Language::get("popcap.rton.done_invalid")), "process_whole");
            return;
        }

        inline static auto process_fs(
            std::string_view source,
            std::string_view definition) -> void
        {
            auto stream = DataStreamView{source};
            auto writer = Writer{};
            process_whole(stream, writer);
            FileSystem::write_file(definition, writer.to_string());
            return;
        }
    };
}