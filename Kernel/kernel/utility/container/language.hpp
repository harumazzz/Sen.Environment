#pragma once

#include <string>
#include <cstdio>
#include <string_view>
#include <fstream>
#include "kernel/utility/assert.hpp"
#include "kernel/subprojects/fmt.hpp"
#include "kernel/subprojects/json.hpp"

#if WIN32
#include <Windows.h>
#endif

namespace Sen::Kernel::Language {

	/**
	 * DO NOT USE THIS, THIS IS LANGUAGE CONTAINER
	*/

	static std::unordered_map<std::string, std::string> language;

	/**
	 * Lambda auto close file
	*/

	inline static auto constexpr close_file = [](FILE* f)
	{
		if(f != nullptr)
		{
			std::fclose(f);
		}
		return;
	};
	/**
	 * Language file reader
	 * Kernel will parse language before start program
	*/

	inline static auto read_language (
		std::string_view source
	) -> void {
		#if _WIN32
		auto static constexpr utf8_to_utf16 = [](std::string_view str) -> std::wstring
		{
			auto size = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0));
			auto destination = std::wstring{};
			destination.resize(size);
			MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), &destination[0], size);
			return destination;
		};
		auto file = std::ifstream(utf8_to_utf16(source));
		#else
		auto file = std::ifstream(source.data());
		#endif
		if (!(file.is_open())) {
			#if _WIN32
			auto path = std::string{source.data(), source.size()};
			std::replace(path.begin(), path.end(), '\\', '/');
			throw Exception{fmt::format("Could not read language file: {}", path), std::source_location::current(), "read_language"};
			#else
			throw Exception{fmt::format("Could not read language file: {}", source), std::source_location::current(), "read_language"};
			#endif
		}
		auto buffer = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		language = nlohmann::json::parse(buffer);
	}


	/**
	 * Get the string from Kernel
	 * The Script will obtain from here, also inner kernel method
	*/

	inline auto get(
		std::string_view key
	) -> std::string
	{
		auto value = language.find({key.data(), key.size()});
		if (value == language.end()) {
			return {key.data(), key.size()};
		}
		return value->second;
	}
}