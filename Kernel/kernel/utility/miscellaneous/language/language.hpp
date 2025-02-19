#pragma once

#include "kernel/subprojects/jsoncons.hpp"
#include <string_view>
#include <optional>

namespace Sen::Kernel::Language {

	static auto language = std::optional<Subprojects::jsoncons::json>{std::nullopt};

	inline auto read_language (
		const std::function<void(std::optional<Subprojects::jsoncons::json>&)>& callback
	) -> void {
		callback(language);
	}

	inline auto get (
		const std::string_view& key
	) -> std::string_view {
		if (!language.has_value()) {
			return key;
		}
		if (language->contains(key)) {
			return language->operator[](key).as<std::string_view>();
		}
		return key;
	}
}
