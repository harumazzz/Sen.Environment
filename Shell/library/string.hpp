#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <span>

#if _WIN32
#include <Windows.h>
#endif

namespace Sen::Shell {

	#if _WIN32

		inline auto u8_to_u16(
			std::u8string_view const& utf8
		) -> std::vector<wchar_t>
		{
			auto size_needed = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCCH>(utf8.data()), static_cast<int>(utf8.size()), nullptr, 0);
			auto utf16 = std::vector<wchar_t>(size_needed + 1);
			MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCCH>(utf8.data()), static_cast<int>(utf8.size()), utf16.data(), size_needed);
			utf16[size_needed] = '\0'; 
			return utf16;
		}

		inline auto u16_to_u8(
			std::u16string_view const& source
		) -> std::string {
			auto size_needed = WideCharToMultiByte(
				CP_UTF8, 0,
				reinterpret_cast<LPCWCH>(source.data()), static_cast<int>(source.size()),
				nullptr, 0,
				nullptr, nullptr
			);
			auto utf8 = std::string(size_needed, '\0');
			WideCharToMultiByte(
				CP_UTF8, 0,
				reinterpret_cast<LPCWCH>(source.data()), static_cast<int>(source.size()),
				utf8.data(), size_needed,
				nullptr, nullptr
			);
			return utf8;
		}

	#endif

	constexpr auto hash_string (
		std::string_view const & string
	) -> std::uint64_t {
		auto offset = std::uint64_t{14695981039346656037ull};
		auto prime = std::uint64_t{1099511628211ull};
		auto result = offset;
		for (auto & element : string) {
			result ^= static_cast<std::uint8_t>(element);
			result *= prime;
		}
		return result;
	}


}