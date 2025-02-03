#pragma once

#include "kernel/utility/macro.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel::Path 
{

	namespace fs = std::filesystem;

	inline auto absolute(
		const std::string & source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(fs::absolute(utf8_to_utf16(source)).wstring());
		#else
		return fs::absolute(source).string();
		#endif
	}

	inline auto getParents(
		const std::string &source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(fs::path(utf8_to_utf16(source)).parent_path().wstring());
		#else
		return fs::path(source).parent_path().string();
		#endif
	}


	inline auto getFileName(
		const std::string &source
	) -> std::string const
	{
		#if WINDOWS
		return utf16_to_utf8(fs::path(utf8_to_utf16(source)).filename().wstring());
		#else
		return fs::path(source).filename().string();
		#endif
	}

	inline auto getNearestParent(
		const std::string &source
	) -> std::string const
	{
		#if WINDOWS
		return utf16_to_utf8(fs::path(utf8_to_utf16(source)).parent_path().filename().wstring());
		#else
		return fs::path(source).parent_path().filename().string();
		#endif
	}

	inline auto getExtension(
		const std::string &source
	) -> std::string const
	{
		#if WINDOWS
		return utf16_to_utf8(fs::path(utf8_to_utf16(source)).extension().wstring());
		#else
		return fs::path(source).extension().string();
		#endif
	}

	inline auto getFileNameWithoutExtension(
		const std::string &source
	) -> std::string const
	{
		#if WINDOWS
		return utf16_to_utf8(fs::path(utf8_to_utf16(source)).stem().wstring());
		#else
		return fs::path(source).stem().string();
		#endif
	}

	inline auto join(
		const std::string & source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(fs::path(source).wstring());
		#else
		return fs::path(source).string();
		#endif
	}

	inline auto join(
		std::initializer_list<std::string>& list
	) -> std::string
	{
		#if WINDOWS
		auto path = fs::path{ std::wstring{} };
		for(auto & element : list){
			path.append(utf8_to_utf16(element));
		}
		return utf16_to_utf8(path.wstring());
		#else
		auto path = fs::path{std::string{}};
		for(auto & element : list){
			path.append(element);
		}
		return path.string();
		#endif
	}

	template<typename... Args>
	inline static auto join(
		const Args&... args
	) -> std::string
	{
		#if WINDOWS && !defined MSVC_COMPILER
		static_assert(false, "msvc compiler is required on windows");
		#endif
		auto result = std::filesystem::path{};
		(result /= ... /= args);
		#if WINDOWS
		return utf16_to_utf8(result.wstring());
		#else
		return result.string();
		#endif
	}

	inline static auto join(
		const List<std::string>& args
	) -> std::string
	{
		auto result = std::filesystem::path{};
		for (auto& arg : args) {
			result /= utf8_to_utf16(arg);
		}
		#if WINDOWS
		auto posix_string = result.wstring();
		std::replace(posix_string.begin(), posix_string.end(), L'\\', L'/');
		#else
		auto posix_string = result.string();
		std::replace(posix_string.begin(), posix_string.end(), '\\', '/');
		#endif
		#if WINDOWS
		return utf16_to_utf8(result.wstring());
		#else
		return result.string();
		#endif
	}

	inline static auto basename(
		const std::string& source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::path{ utf8_to_utf16(source) }.filename().wstring());
		#else
		return std::filesystem::path{ source }.filename().string();
		#endif
	}

	inline static auto base_without_extension(
		const std::string& source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::path{ utf8_to_utf16(source) }.stem().wstring());
		#else
		return std::filesystem::path{ source }.stem().string();
		#endif
	}

	inline static auto dirname(
		const std::string& source
	) -> std::string;

	inline static auto except_extension(
		const std::string& source
	) -> std::string
	{
		return fmt::format("{}/{}", dirname(source), base_without_extension(source));
	}

	inline static auto delimiter(

	) -> std::string
	{
		#if WINDOWS
		return std::string{ "\\" };
		#else
		return std::string{ "/" };
		#endif
	}

	inline static auto dirname(
		const std::string& source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::path{ utf8_to_utf16(source) }.parent_path().wstring());
		#else
		return std::filesystem::path{ source }.parent_path().string();
		#endif
	}

	inline static auto normalize(
		const std::string& source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::path{ utf8_to_utf16(source) }.lexically_normal().wstring());
		#else
		return std::filesystem::path{ source }.lexically_normal().string();
		#endif
	}

	inline static auto resolve(
		const std::string& source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::absolute(utf8_to_utf16(source)).wstring());
		#else
		return std::filesystem::absolute(source).string();
		#endif
	}

	inline static auto extname(
		const std::string& source
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::path{ utf8_to_utf16(source) }.extension().wstring());
		#else
		return std::filesystem::path{ source }.extension().string();
		#endif
	}

	inline static auto is_absolute(
		const std::string& source
	) -> bool
	{
		#if WINDOWS
		return source == utf16_to_utf8(std::filesystem::path(utf8_to_utf16(source)).extension().wstring());
		#else
		return source == std::filesystem::path(source).string();
		#endif
	}

	inline static auto relative(
		const std::string& from,
		const std::string& to
	) -> std::string
	{
		#if WINDOWS
		return utf16_to_utf8(std::filesystem::relative(utf8_to_utf16(to), utf8_to_utf16(from)).wstring());
		#else
		return std::filesystem::relative(to, from).string();
		#endif
	}
}