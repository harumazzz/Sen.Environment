#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Converter {

	template <typename T> requires std::is_arithmetic<T>::value
	inline static auto constexpr convert(
		std::string_view str,
		std::string_view error
	) -> T
	{
		auto result = T{};
		if constexpr (std::is_same<T, float>::value) {
			auto end_ptr = static_cast<char*>(nullptr);
			result = std::strtof(str.data(), &end_ptr);
			if (end_ptr != str.data() + str.size()) {
				throw Exception{std::string{error.data(), error.size()}, std::source_location::current(), "convert"};
			}
		} else if constexpr (std::is_same<T, double>::value) {
			auto end_ptr = static_cast<char*>(nullptr);
			result = std::strtod(str.data(), &end_ptr);
			if (end_ptr != str.data() + str.size()) {
				throw Exception{std::string{error.data(), error.size()}, std::source_location::current(), "convert"};
			}
		} else {
			if (std::from_chars(str.data(), str.data() + str.size(), result).ec != std::errc()) {
				throw Exception{std::string{error.data(), error.size()}, std::source_location::current(), "convert"};
			}
		}
		return result;
	}

}