module;

import <string>;
import <locale>;
import <codecvt>;
import <vector>;
import <span>;

#if _WIN32
#include <Windows.h>
#endif

export module sen.environment.shell.library.string;

export namespace Sen::Shell {

	inline auto utf8_to_utf16(
		const std::string& str
	) -> std::wstring
	{
		auto myconv = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
		return myconv.from_bytes(str);
	}

	#if _WIN32

		inline auto u8_to_u16(
			const std::string& utf8
		) -> std::vector<wchar_t>
		{
			auto size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
			auto utf16 = std::vector<wchar_t>(size_needed + 1);
			MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), utf16.data(), size_needed);
			utf16[size_needed] = '\0'; 
			return utf16;
		}
	#endif

	inline auto utf16_to_utf8(
		std::u16string_view const& source
	) -> std::u8string {
		auto converter = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{};
		auto result = converter.to_bytes(
			source.data(),
			source.data() + source.size()
		);
		return reinterpret_cast<std::u8string&>(result);
	}

	inline auto utf16_to_utf8(
		const std::wstring& wstr
	) -> std::string
	{
		auto myconv = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
		return myconv.to_bytes(wstr);
	}

	inline constexpr auto hash_string (
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