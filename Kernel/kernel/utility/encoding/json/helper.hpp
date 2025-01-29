#pragma once
#include "kernel/utility/encoding/json/common.hpp"

namespace Sen::Kernel::Encoding::JSON {

	struct Helper : public Common {

		using Common = Common;

		inline static auto decode(
			std::uint8_t &state,
			std::uint32_t &codepoint,
			const std::uint8_t byte
		) -> std::uint8_t
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
			char const &character
		) -> size_t
		{
			auto extra_size = size_t{};
			if (character < 0b1'0000000)
			{
				extra_size = 0;
			}
			else if (character < 0b11'000000)
			{
				// TODO : add localization
				assert_conditional(false, "first utf-8 character is valid", "compute_utf8_character_extra_size");
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
				// TODO : add localization
				assert_conditional(false, "first utf-8 character is valid", "compute_utf8_character_extra_size");
			}
			return extra_size;
		}

            inline static auto clip_bit_char(
                char const &it,
                size_t const &begin,
                size_t const &size,
				std::string& destination
			) -> void
            {
                destination += std::to_string((it >> begin) & ~(static_cast<char>((std::numeric_limits<char>::max)()) << size));
				return;
            }

        public:

            inline static auto write_escape_string(
                std::string &output,
                const std::string &s
			) -> void
            {
                auto codepoint = std::uint32_t{0};
                auto state = 0_byte;
                for (auto i : Range{s.size()})
                {
                    if (decode(state, codepoint, static_cast<std::uint8_t>(s[i])) == 0_byte)
                    {
                        switch (codepoint)
                        {
                        case Common::backspace_point: // backspace
                        {
                            output += "\\b";
                            break;
                        }

                        case Common::horizontal_tab_point: // horizontal tab
                        {
                            output += "\\t";
                            break;
                        }

                        case Common::newline_point: // newline
                        {
                            output += "\\n";
                            break;
                        }

                        case Common::form_feed_point: // formfeed
                        {
                            output += "\\f";
                            break;
                        }

                        case Common::carriage_return_point: // carriage return
                        {
                            output += "\\r";
                            break;
                        }

                        case Common::quatation_mark_point: // quotation mark
                        {
                            output += "\\\"";
                            break;
                        }
                        case Common::reverse_solidus_point: // reverse solidus
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
                                    clip_bit_char(char_str, 28, 4, output);
                                    clip_bit_char(char_str, 24, 4, output);
                                    clip_bit_char(char_str, 20, 4, output);
                                    clip_bit_char(char_str, 16, 4, output);
                                }
                                output += "\\u";
                                clip_bit_char(char_str, 12, 4, output);
                                clip_bit_char(char_str, 8, 4, output);
                                clip_bit_char(char_str, 4, 4, output);
                                clip_bit_char(char_str, 0, 4, output);
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
                                // TODO : add localization
                                assert_conditional(false, "extra utf-8 character is valid", "write_escape_string");
                            }
                            output += char_8;
                        }
                    }
                }
                assert_conditional(state == 0_byte, String::format(fmt::format("{}", Language::get("json_writer.incomplete_utf8_string")), std::to_string(output.at(output.size() - 1))), "write_escape_string");
                return;
            }

            inline static auto write_new_line(
				std::string &output
			) -> void
            {
				// Write '\r\n' OR '\n', depending on OS
				#if LINUX
                output += Common::carriage_return;
				#endif
                output += Common::line_feed;
                return;
            }

            inline static auto write_indentation(
				std::string &output, 
				int indent
			) -> void
            {
                for (auto index : Range{indent})
                {
                    output += Common::tab;
                }
                return;
            }
        };

}