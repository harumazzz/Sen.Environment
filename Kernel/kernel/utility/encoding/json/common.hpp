#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel::Encoding::JSON {

	struct Common {

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
	};

	enum class Type : uint8_t {
		none,
		start_object,
		EndObject,
		start_array,
		end_array,
		property_name,
		comment,
		string,
		number,
		true_value,
		false_value,
		null_value,
	};

}