#pragma once

#include <map>
#include <string>
#include <memory>
#include <cstdio>
#include <vector>
#include <cstdarg>
#include <string_view>
#include "kernel/definition/assert.hpp"
#include "kernel/dependencies/fmt.hpp"
#include "kernel/dependencies/simdjson.hpp"

namespace Sen::Kernel::Language
{

	/**
	 * DO NOT USE THIS, THIS IS LANGUAGE CONTAINER
	*/

	static inline simdjson::ondemand::parser parser;
    static inline simdjson::padded_string json_data;
    static inline simdjson::ondemand::document language;

	/**
	 * Lambda auto close file
	*/

	inline static auto constexpr close_file = [](FILE* f)
	{
		if(f != nullptr)
		{
			std::fclose(f);
			f = nullptr;
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
		json_data = simdjson::padded_string::load(source); 
        language = parser.iterate(json_data);
		return;
	}


	/**
	 * Get the string from Kernel
	 * The Script will obtain from here, also inner kernel method
	*/

	inline static auto get(
		std::string_view key
	) -> std::string
	{
		auto value = language[key];
		if (value.error() == simdjson::SUCCESS) {
			auto result = value.get_string().value();
			auto destination = std::string{result.data(), result.size()};
			return destination;
		}
		return std::string{key.data(), key.size()};
	}
}