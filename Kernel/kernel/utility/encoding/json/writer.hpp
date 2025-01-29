#pragma once

#include "kernel/utility/encoding/json/helper.hpp"

namespace Sen::Kernel::Encoding::JSON {

    template <bool WriteIndent>
    class Writer;

    using IndentWriter = Writer<true>;

    template <bool WriteIndent>
    class Writer : public Common {

        protected:

            using Common = Common;

        private:

            std::string _baseString{};

            bool _commentAfterNoneOrProperyName{};

            Type _tokenType{};
            
            size_t _currentDepth{0};

            std::size_t _indentLength{1};
        
        public:

            explicit Writer(

            ) = default;

            ~Writer(

            ) = default;

            inline auto write_start(
                const std::string &token
            ) -> void
            {
                if constexpr (WriteIndent)
                {
                    thiz.write_start_indented(token);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += Common::list_separator;
                    }
                    thiz._baseString += token;
                }
                thiz._currentDepth &= Common::remove_flags_bit_mask;
                ++thiz._currentDepth;
                return;
            }

            inline auto write_string_property_name(
                const std::string &propertyName
            ) -> void
            {
                if constexpr (WriteIndent)
                {
                    thiz.write_string_indented_property_name(propertyName);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += Common::list_separator;
                    }
                    thiz._baseString += Common::quote;
                    Helper::write_escape_string(thiz._baseString, propertyName);
                    thiz._baseString += Common::quote;
                    thiz._baseString += Common::key_value_separator;
                }
                thiz._currentDepth &= Common::remove_flags_bit_mask;
                return;
            }

            inline auto write_string_value(
                const std::string &value
            ) -> void
            {
                if constexpr (WriteIndent)
                {
                    thiz.write_string_indented(value);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += Common::list_separator;
                    }
                    thiz._baseString += Common::quote;
                    Helper::write_escape_string(thiz._baseString, value);
                    thiz._baseString += Common::quote;
                }
                thiz.set_flag_to_add_list_separator_before_next_item();
                return;
            }

            inline auto write_simple_indented(
                const std::string &value
            ) -> void
            {
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += Common::list_separator;
                }
                if (thiz._tokenType != Type::property_name)
                {
                    if (thiz._tokenType != Type::none)
                    {
                        Helper::write_new_line(thiz._baseString);
                    }
                    auto indent = thiz._currentDepth & Common::remove_flags_bit_mask * thiz._indentLength;
                    Helper::write_indentation(thiz._baseString, indent);
                }
                thiz._baseString += value;
                return;
            }

            inline auto write_simple_value(
                const std::string &value
            ) -> void
            {
                if constexpr (WriteIndent)
                {
                    thiz.write_simple_indented(value);
                }
                else
                {
                    if (thiz._currentDepth < 0)
                    {
                        thiz._baseString += Common::list_separator;
                    }
                    thiz._baseString += value;
                }
                thiz.set_flag_to_add_list_separator_before_next_item();
                return;
            }

            inline auto write_end(
                const std::string &token
            ) -> void
            {
                if constexpr (WriteIndent)
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

            inline auto write_string_indented(
                const std::string &value
            ) -> void
            {
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += Common::list_separator;
                }
                if (thiz._tokenType != Type::property_name)
                {
                    if (thiz._tokenType != Type::none)
                    {
                        Helper::write_new_line(thiz._baseString);
                    }
                    auto indent = thiz._currentDepth & Common::remove_flags_bit_mask * thiz._indentLength;
                    Helper::write_indentation(thiz._baseString, indent);
                }
                thiz._baseString += Common::quote;
                Helper::write_escape_string(thiz._baseString, value);
                thiz._baseString += Common::quote;
                return;
            }

            inline auto write_string_indented_property_name(
                const std::string &propertyName
            ) -> void
            {
                auto indent = thiz._currentDepth & Common::remove_flags_bit_mask * thiz._indentLength;
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += Common::list_separator;
                }
                if (thiz._tokenType != Type::none)
                {
                    Helper::write_new_line(thiz._baseString);
                }
                Helper::write_indentation(thiz._baseString, indent);
                thiz._baseString += Common::quote;
                Helper::write_escape_string(thiz._baseString, propertyName);
                thiz._baseString += Common::quote;
                thiz._baseString += Common::key_value_separator;
                thiz._baseString += Common::space;
                return;
            }

            inline auto write_end_indented(
                const std::string &token
            ) -> void
            {
                if (thiz._tokenType == Type::start_array || thiz._tokenType == Type::start_object)
                {
                    thiz._baseString += token;
                }
                else
                {
                    auto indent = thiz._currentDepth & Common::remove_flags_bit_mask * thiz._indentLength;
                    if (indent != -0)
                    {
                        indent -= thiz._indentLength;
                    }
                    Helper::write_new_line(thiz._baseString);
                    Helper::write_indentation(thiz._baseString, indent);
                    thiz._baseString += token;
                }
                return;
            }

            inline auto write_start_indented(
                std::string_view token
            ) -> void
            {
                if (thiz._currentDepth < 0)
                {
                    thiz._baseString += Common::list_separator;
                }
                if (thiz._tokenType != Type::none && thiz._tokenType != Type::property_name)
                {
                    Helper::write_new_line(thiz._baseString);
                    auto indent = thiz._currentDepth & Common::remove_flags_bit_mask * thiz._indentLength;
                    Helper::write_indentation(thiz._baseString, indent);
                }
                thiz._baseString += token;
                return;
            }

            inline auto set_flag_to_add_list_separator_before_next_item(

            ) -> void
            {
                thiz._currentDepth |= 1 << 31;
                return;
            }

        public:
            
            inline auto clear(

            ) -> void
            {
                thiz._baseString.clear();
                thiz._currentDepth = 0;
                thiz._tokenType = Type::none;
                return;
            }

            inline auto write_start_object(

            ) -> void
            {
                thiz.write_start(Common::open_brace.data());
                thiz._tokenType = Type::start_object;
                return;
            }

            inline auto write_start_array(

            ) -> void
            {
                thiz.write_start(Common::open_bracket.data());
                thiz._tokenType = Type::start_array;
                return;
            }

            inline auto write_end_object(

            ) -> void
            {
                thiz.write_end(Common::close_brace.data());
                thiz._tokenType = Type::EndObject;
                return;
            }

            inline auto write_end_array(

            )  -> void
            {
                thiz.write_end(Common::close_bracket.data());
                thiz._tokenType = Type::end_array;
                return;
            }

            inline auto write_property_name(
                const std::string &propertyName
            )  -> void
            {
                thiz.write_string_property_name(propertyName);
                thiz._tokenType = Type::property_name;
                return;
            }
            
            template <HasEmptyMethod T>
            inline auto write_property_name(
                const T& propertyName
            ) -> void
            {
                auto name = std::to_string(propertyName);
                thiz.write_string_property_name(name);
                thiz._tokenType = Type::property_name;
                return;
            }
            
            inline auto write_value(
                const std::string &value
            ) -> void
            {
                thiz.write_string_value(value);
                thiz._tokenType = Type::string;
                return;
            }

            template <typename T> requires std::is_arithmetic_v<T>
            inline auto write_value(
                T value
            ) -> void
            {
                auto num_value = std::to_string(value);
                thiz.write_simple_value(num_value);
                thiz._tokenType = Type::number;
                return;
            }

            inline auto write_null(

            ) -> void
            {
                thiz.write_simple_value(Common::null_value.data());
                thiz._tokenType = Type::null_value;
                return;
            }

            inline auto write_boolean(
                bool boolean
            ) -> void
            {
                if (boolean)
                {
                    thiz.write_simple_value(Common::true_value.data());
                    thiz._tokenType = Type::true_value;
                }
                else
                {
                    thiz.write_simple_value(Common::false_value.data());
                    thiz._tokenType = Type::false_value;
                }
                return;
            }

            inline auto to_string(

            ) -> std::string
            {
                auto baseString = thiz._baseString;
                thiz.clear();
                return baseString;
            }
        };
        
}
