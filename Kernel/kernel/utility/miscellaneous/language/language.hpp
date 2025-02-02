#pragma once

#include <string>
#include <unordered_map>

namespace Sen::Kernel::Language {

	static std::unordered_map<std::string, std::string> language;

	inline auto read_language (
		std::string_view source
	) -> void {

	}

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