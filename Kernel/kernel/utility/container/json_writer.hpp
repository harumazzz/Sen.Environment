#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel
{
    namespace UTF8Json
    {

        class JsonConstants
        {
        public:
            inline static constexpr auto open_brace = std::string_view{"{"};
            inline static constexpr auto close_brace = std::string_view{"}"};
            inline static constexpr auto open_bracket = std::string_view{"["};
            inline static constexpr auto close_bracket = std::string_view{"]"};
            inline static constexpr auto space = std::string_view{" "};
            inline static constexpr auto carriage_return = std::string_view{"\r"};
            inline static constexpr auto line_feed = std::string_view{"\n"};
            inline static constexpr auto tab = std::string_view{"\t"};
            inline static constexpr auto list_separator = std::string_view{","};
            inline static constexpr auto key_value_separator = std::string_view{":"};
            inline static constexpr auto quote = std::string_view{"\""};
            inline static constexpr auto back_slash = std::string_view{"\\"};
            inline static constexpr auto slash = std::string_view{"/"};
            inline static constexpr auto back_space = std::string_view{"\b"};
            inline static constexpr auto form_feed = std::string_view{"\f"};
            inline static constexpr auto asterisk = std::string_view{"*"};
            inline static constexpr auto colon = std::string_view{":"};
            inline static constexpr auto period = std::string_view{"."};
            inline static constexpr auto plus = std::string_view{"+"};
            inline static constexpr auto hyphen = std::string_view{"-"};
            inline static constexpr auto utc_offset_token = std::string_view{"Z"};
            inline static constexpr auto time_prefix = std::string_view{"T"};

            inline static constexpr auto utf8_bom = std::string_view{"ï»¿"};
            inline static constexpr auto true_value = std::string_view{"true"};
            inline static constexpr auto false_value = std::string_view{"false"};
            inline static constexpr auto null_value = std::string_view{"null"};

            inline static constexpr auto remove_flags_bit_mask = 0x7FFFFFFF;

            inline static constexpr auto backspace_point = 0x08u;
            inline static constexpr auto horizontal_tab_point = 0x09u;
            inline static constexpr auto newline_point = 0x0Au;
            inline static constexpr auto form_feed_point = 0x0Cu;
            inline static constexpr auto carriage_return_point = 0x0Du;
            inline static constexpr auto quatation_mark_point = 0x22u;
            inline static constexpr auto reverse_solidus_point = 0x5Cu;
        };

        /// This enum defines the various JSON tokens that make up a JSON text and is used by
        enum JsonTokenType
        {
            // Do not re-order.
            // We rely on the ordering to quickly check things like IsTokenTypePrimitive
            /*
                Indicates that there is no value (as distinct from <see cref="Null"/>).

                This is the default token type if no data has been read by the <see cref="Utf8JsonReader"/>.
            */
            none,
            /*
                Indicates that the token type is the start of a JSON object.
            */
            start_object,
            /*
                Indicates that the token type is the end of a JSON object.
            */
            EndObject,
            /*
                Indicates that the token type is the start of a JSON array.
            */
            start_array,
            /*
                Indicates that the token type is the end of a JSON array.
            */
            end_array,
            /*
                Indicates that the token type is a JSON property name.
            */
            property_name,
            /*
                Indicates that the token type is the comment string.
            */
            comment,
            /*
                Indicates that the token type is a JSON string.
            */
            string,
            /*
                Indicates that the token type is a JSON number.
            */
            number,
            /*
                Indicates that the token type is the JSON literal <c>true</c>.
            */
            true_value,
            /*
                Indicates that the token type is the JSON literal <c>false</c>.
            */
            false_value,
            /*
                Indicates that the token type is the JSON literal <c>null</c>.
            */
            null_value,
        };

        struct JsonWriterHelper
        {

        private:
            inline static constexpr auto utf8_d = std::array<std::uint8_t, 400>{
                {
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00..1F
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20..3F
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 40..5F
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60..7F
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, // 80..9F
                    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, // A0..BF
                    8, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // C0..DF
                    0xA, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x3,                 // E0..EF
                    0xB, 0x6, 0x6, 0x6, 0x5, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,                 // F0..FF
                    0x0, 0x1, 0x2, 0x3, 0x5, 0x8, 0x7, 0x1, 0x1, 0x1, 0x4, 0x6, 0x1, 0x1, 0x1, 0x1,                 // s0..s0
                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, // s1..s2
                    1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, // s3..s4
                    1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, // s5..s6
                    1, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1  // s7..s8
                }};

        protected:
            inline static auto decode(
                std::uint8_t &state,
                std::uint32_t &codepoint,
                const std::uint8_t byte) -> std::uint8_t
            {
                assert_conditional(byte < utf8_d.size(), fmt::format("{}", Language::get("json_writer.byte_cannot_smaller_than_utf8_size")), "decode");
                auto type = static_cast<std::uint8_t>(utf8_d[byte]);
                codepoint = (state != 0_byte)
                                ? (byte & 0x3fu) | (codepoint << 6u)
                                : (0xFFu >> type) & (byte);
                auto index = 256u + static_cast<size_t>(state) * 16u + static_cast<size_t>(type);
                assert_conditional(index < utf8_d.size(), fmt::format("{}", Language::get("json_writer.index_cannot_smaller_than_utf8_size")), "decode");
                state = utf8_d[index];
                return state;
            }

            inline static auto compute_utf8_character_extra_size(
                char const &character) -> size_t
            {
                auto extra_size = size_t{};
                if (character < 0b1'0000000)
                {
                    extra_size = 0;
                }
                else if (character < 0b11'000000)
                {
                    assert("first utf-8 character is valid");
                }
                else if (character < 0b111'00000)
                {
                    extra_size = 1;
                }
                else if (character < 0b1111'0000)
                {
                    extra_size = 2;
                }
                else if (character < 0b11111'000)
                {
                    extra_size = 3;
                }
                else
                {
                    assert("first utf-8 character is valid");
                }
                return extra_size;
            }

            inline static auto clip_bit_char(
                char const &it,
                size_t const &begin,
                size_t const &size) -> std::string
            {
                return std::to_string((it >> begin) & ~(static_cast<char>((std::numeric_limits<char>::max)()) << size));
            }

        public:
            // Unnecessary.
            /*
            inline static auto Validate(std::string propertyName) -> void
            {
                return;
            }
            */

            inline static auto write_escape_string(
                std::string &output,
                const std::string &s) -> void
            {
                auto codepoint = std::uint32_t{0};
                auto state = 0_byte;
                for (auto i = 0; i < s.size(); i++)
                {
                    if (decode(state, codepoint, static_cast<std::uint8_t>(s[i])) == 0_byte)
                    {
                        switch (codepoint)
                        {
                        case JsonConstants::backspace_point: // backspace
                        {
                            output += "\\b";
                            break;
                        }

                        case JsonConstants::horizontal_tab_point: // horizontal tab
                        {
                            output += "\\t";
                            break;
                        }

                        case JsonConstants::newline_point: // newline
                        {
                            output += "\\n";
                            break;
                        }

                        case JsonConstants::form_feed_point: // formfeed
                        {
                            output += "\\f";
                            break;
                        }

                        case JsonConstants::carriage_return_point: // carriage return
                        {
                            output += "\\r";
                            break;
                        }

                        case JsonConstants::quatation_mark_point: // quotation mark
                        {
                            output += "\\\"";
                            break;
                        }
                        case JsonConstants::reverse_solidus_point: // reverse solidus
                        {
                            output += "\\\\";
                            break;
                        }
                        default:
                            auto &char_str = s[i];
                            if ((codepoint <= 0x1F))
                            {
                                if (codepoint >= 0xFFFF)
                                {
                                    output += "\\u";
                                    output += clip_bit_char(char_str, 28, 4);
                                    output += clip_bit_char(char_str, 24, 4);
                                    output += clip_bit_char(char_str, 20, 4);
                                    output += clip_bit_char(char_str, 16, 4);
                                }
                                output += "\\u";
                                output += clip_bit_char(char_str, 12, 4);
                                output += clip_bit_char(char_str, 8, 4);
                                output += clip_bit_char(char_str, 4, 4);
                                output += clip_bit_char(char_str, 0, 4);
                            }
                            else
                            {
                                output += char_str;
                            }
                            break;
                        }
                    }
                    else
                    {
                        auto extra_size = compute_utf8_character_extra_size(s[i]);
                        output += s[i];
                        while (extra_size > 0)
                        {
                            --extra_size;
                            auto char_8 = s[++i];
                            if ((char_8 & 0b11'000000) != 0b11'000000)
                            {
                                assert("extra utf-8 character is valid");
                            }
                            output += char_8;
                        }
                    }
                }
                assert_conditional(state == 0_byte, String::format(fmt::format("{}", Language::get("json_writer.incomplete_utf8_string")), std::to_string(output.at(output.size() - 1))), "write_escape_string");
                return;
            }

            inline static auto
            write_new_line(std::string &output) -> void
            {
// Write '\r\n' OR '\n', depending on OS
#if LINUX
                output += JsonConstants::carriage_return;
#endif
                output += JsonConstants::line_feed;
                return;
            }

            inline static auto write_indentation(std::string &output, int indent) -> void
            {
                for (auto i : Range<int>(indent))
                {
                    output += JsonConstants::tab;
                }
                return;
            }
        };

        // maybe has bug hex string and byte string when convert to escape string.

        struct JsonWriter
        {
        private:
            /// baseString to read or write Json.
            std::string mutable _baseString;

            bool mutable _commentAfterNoneOrProperyName;
            /// default none.
            JsonTokenType mutable _tokenType;

            // The highest order bit of _currentDepth is used to discern whether we are writing the first item in a list or not.
            // if (_currentDepth >> 31) == 1, add a list separator before writing the item
            // else, no list separator is needed since we are writing the first item.
            int mutable _currentDepth = 0;

            std::size_t mutable _indentLength = 1;

            inline auto write_start(const std::string &token) const -> void
            {
                if (thiz.WriteIndent)
                {
                    thiz.write_start_indented(token);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += JsonConstants::list_separator;
                    }
                    thiz._baseString += token;
                }
                thiz._currentDepth &= JsonConstants::remove_flags_bit_mask;
                ++thiz._currentDepth;
                return;
            }

            /*
                Writes the UTF-8 property name (as a JSON string) as the first part of a name/value pair of a JSON object.
            */

            inline auto write_string_property_name(const std::string &propertyName) const -> void
            {
                if (thiz.WriteIndent)
                {
                    thiz.write_string_indented_property_name(propertyName);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += JsonConstants::list_separator;
                    }
                    thiz._baseString += JsonConstants::quote;
                    JsonWriterHelper::write_escape_string(thiz._baseString, propertyName);
                    thiz._baseString += JsonConstants::quote;
                    thiz._baseString += JsonConstants::key_value_separator;
                }
                thiz._currentDepth &= JsonConstants::remove_flags_bit_mask;
                return;
            }

            inline auto write_string_value(const std::string &value) const -> void
            {
                if (thiz.WriteIndent)
                {
                    thiz.write_string_indented(value);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += JsonConstants::list_separator;
                    }
                    thiz._baseString += JsonConstants::quote;
                    JsonWriterHelper::write_escape_string(thiz._baseString, value);
                    thiz._baseString += JsonConstants::quote;
                }
                thiz.set_flag_to_add_list_separator_before_next_item();
                return;
            }

            inline auto write_simple_indented(const std::string &value) const -> void
            {
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += JsonConstants::list_separator;
                }
                if (thiz._tokenType != property_name)
                {
                    if (thiz._tokenType != none)
                    {
                        JsonWriterHelper::write_new_line(thiz._baseString);
                    }
                    auto indent = thiz._currentDepth & JsonConstants::remove_flags_bit_mask * thiz._indentLength;
                    JsonWriterHelper::write_indentation(thiz._baseString, indent);
                }
                thiz._baseString += value;
                return;
            }

            inline auto write_simple_value(const std::string &value) const -> void
            {
                if (thiz.WriteIndent)
                {
                    thiz.write_simple_indented(value);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += JsonConstants::list_separator;
                    }
                    thiz._baseString += value;
                }
                thiz.set_flag_to_add_list_separator_before_next_item();
                return;
            }

            inline auto write_end(const std::string &token) const -> void
            {
                if (thiz.WriteIndent)
                {
                    thiz.write_end_indented(token);
                }
                else
                {
                    thiz._baseString += token;
                }
                thiz.set_flag_to_add_list_separator_before_next_item();
                if (thiz._currentDepth != 0)
                {
                    --thiz._currentDepth;
                }
                return;
            }

            inline auto write_string_indented(const std::string &value) const -> void
            {
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += JsonConstants::list_separator;
                }
                if (thiz._tokenType != property_name)
                {
                    if (thiz._tokenType != none)
                    {
                        JsonWriterHelper::write_new_line(thiz._baseString);
                    }
                    auto indent = thiz._currentDepth & JsonConstants::remove_flags_bit_mask * thiz._indentLength;
                    JsonWriterHelper::write_indentation(thiz._baseString, indent);
                }
                thiz._baseString += JsonConstants::quote;
                JsonWriterHelper::write_escape_string(thiz._baseString, value);
                thiz._baseString += JsonConstants::quote;
                return;
            }

            inline auto write_string_indented_property_name(const std::string &propertyName) const -> void
            {
                auto indent = thiz._currentDepth & JsonConstants::remove_flags_bit_mask * thiz._indentLength;
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += JsonConstants::list_separator;
                }
                if (thiz._tokenType != none)
                {
                    JsonWriterHelper::write_new_line(thiz._baseString);
                }
                JsonWriterHelper::write_indentation(thiz._baseString, indent);
                thiz._baseString += JsonConstants::quote;
                JsonWriterHelper::write_escape_string(thiz._baseString, propertyName);
                thiz._baseString += JsonConstants::quote;
                thiz._baseString += JsonConstants::key_value_separator;
                thiz._baseString += JsonConstants::space;
                return;
            }

            inline auto write_end_indented(const std::string &token) const -> void
            {
                if (thiz._tokenType == start_array || thiz._tokenType == start_object)
                {
                    thiz._baseString += token;
                }
                else
                {
                    auto indent = thiz._currentDepth & JsonConstants::remove_flags_bit_mask * thiz._indentLength;
                    if (indent != -0)
                    {
                        indent -= thiz._indentLength;
                    }
                    JsonWriterHelper::write_new_line(thiz._baseString);
                    JsonWriterHelper::write_indentation(thiz._baseString, indent);
                    thiz._baseString += token;
                }
                return;
            }

            inline auto write_start_indented(std::string_view token) const -> void
            {
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += JsonConstants::list_separator;
                }
                if (thiz._tokenType != none && thiz._tokenType != property_name)
                {
                    JsonWriterHelper::write_new_line(thiz._baseString);
                    auto indent = thiz._currentDepth & JsonConstants::remove_flags_bit_mask * thiz._indentLength;
                    JsonWriterHelper::write_indentation(thiz._baseString, indent);
                }
                thiz._baseString += token;
                return;
            }

            inline auto set_flag_to_add_list_separator_before_next_item() const -> void
            {
                thiz._currentDepth |= 1 << 31;
                return;
            }

        public:
            // Prettify Json.
            bool mutable WriteIndent;
            /*
                Reset to default.
            */
            inline auto clear() const -> void
            {
                thiz._baseString.clear();
                thiz.WriteIndent = false;
                thiz._currentDepth = 0;
                thiz._tokenType = none;
                return;
            }
            /*
                Writes the beginning of a JSON object.
            */
            inline auto write_start_object() const -> void
            {
                thiz.write_start(JsonConstants::open_brace.data());
                thiz._tokenType = start_object;
                return;
            }
            /*
                Writes the beginning of a JSON array.
            */
            inline auto write_start_array() const -> void
            {
                thiz.write_start(JsonConstants::open_bracket.data());
                thiz._tokenType = start_array;
                return;
            }
            /*
                Writes the end of a JSON object.
            */
            inline auto write_end_object() const -> void
            {
                thiz.write_end(JsonConstants::close_brace.data());
                thiz._tokenType = EndObject;
                return;
            }
            /*
                Writes the end of a JSON array.
            */
            inline auto write_end_array() const -> void
            {
                thiz.write_end(JsonConstants::close_bracket.data());
                thiz._tokenType = end_array;
                return;
            }
            /*!
                Writes the property name (as a JSON string) as the first part of a name/value pair of a JSON object.
                @param[in] properyName The name of the property to write.
            */
            inline auto write_property_name(const std::string &propertyName) const -> void
            {
                thiz.write_string_property_name(propertyName);
                thiz._tokenType = property_name;
                return;
            }
            /*!
                Writes the property name (as a JSON string) as the first part of a name/value pair of a JSON object.
                @param[in] properyName The name of the property to write.
            */
            template <HasEmptyMethod T>
            inline auto write_property_name(T propertyName) const -> void
            {
                auto name = std::to_string(propertyName);
                thiz.write_string_property_name(name);
                thiz._tokenType = property_name;
                return;
            }
            /*!
                Writes the pre-encoded text value (as a JSON string) as an element of a JSON.
                @param[in] value The JSON-encoded value to write.
            */
            inline auto write_value(const std::string &value) const -> void
            {
                thiz.write_string_value(value);
                thiz._tokenType = string;
                return;
            }
            /*!
                Writes the number value (as a JSON number) as an element of a JSON.
                @param[in] value The JSON-encoded value to write.
            */
            template <typename T>
                requires std::is_arithmetic_v<T>
            inline auto write_value(T value) const -> void
            {
                auto num_value = std::to_string(value);
                thiz.write_simple_value(num_value);
                thiz._tokenType = number;
                return;
            }

            /*!
                Writes the null value (as a JSON literal "null") as an element of a JSON.
            */
            inline auto write_null() const -> void
            {
                thiz.write_simple_value(JsonConstants::null_value.data());
                thiz._tokenType = null_value;
                return;
            }
            /*!
                Write the boolean vaule (as a JSON literal "true" or "false") as an element of a JSON.
                @param[in] value The JSON-encoded boolean value to write.
            */
            inline auto write_boolean(bool boolean) const -> void
            {
                if (boolean)
                {
                    thiz.write_simple_value(JsonConstants::true_value.data());
                    thiz._tokenType = true_value;
                }
                else
                {
                    thiz.write_simple_value(JsonConstants::false_value.data());
                    thiz._tokenType = false_value;
                }
                return;
            }

            /*
                Returns the amount of strings written by the JsonWriter.
                That have been cleared to the base string.
            */
            inline auto to_string() const -> std::string
            {
                auto baseString = thiz._baseString;
                thiz.clear();
                return baseString;
            }
        };

        // Unnecessary.
        /*
        struct JsonReader
        {
        private:
        public:
        };
        */
    }

    using JsonWriter = Sen::Kernel::UTF8Json::JsonWriter;
}
