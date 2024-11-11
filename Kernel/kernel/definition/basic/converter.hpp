#pragma once

#include "kernel/definition/library.hpp"
#include "kernel/definition/assert.hpp"
#include "kernel/definition/macro.hpp"

namespace Sen::Kernel::Converter {

	/**
	 * str -> string to convert
	 * error -> if cannot convert, the exception will be thrown
	 */

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
				throw Exception(std::string{error.data(), error.size()}, std::source_location::current(), "convert");
			}
		} else if constexpr (std::is_same<T, double>::value) {
			auto end_ptr = static_cast<char*>(nullptr);
			result = std::strtod(str.data(), &end_ptr);
			if (end_ptr != str.data() + str.size()) {
				throw Exception(std::string{error.data(), error.size()}, std::source_location::current(), "convert");
			}
		} else {
			if (std::from_chars(str.data(), str.data() + str.size(), result).ec != std::errc()) {
				throw Exception(std::string{error.data(), error.size()}, std::source_location::current(), "convert");
			}
		}
		return result;
	}

	inline static auto constexpr to_int8(
		std::string_view str,
		std::string_view error
	) -> std::int8_t
	{
		return convert<std::int8_t>(str, error);
	}

	inline static auto constexpr to_uint8(
		std::string_view str,
		std::string_view error
	) -> std::uint8_t
	{
		return convert<std::uint8_t>(str, error);
	}

	inline static auto constexpr to_int16(
		std::string_view str,
		std::string_view error
	) -> std::int16_t
	{
		return convert<std::int16_t>(str, error);
	}

	inline static auto constexpr to_uint16(
		std::string_view str,
		std::string_view error
	) -> std::uint16_t
	{
		return convert<std::uint16_t>(str, error);
	}

	inline static auto constexpr to_int32(
		std::string_view str,
		std::string_view error
	) -> std::int32_t
	{
		return convert<std::int32_t>(str, error);
	}

	inline static auto constexpr to_uint32(
		std::string_view str,
		std::string_view error
	) -> std::uint32_t
	{
		return convert<std::uint32_t>(str, error);
	}

	inline static auto constexpr to_int64(
		std::string_view str,
		std::string_view error
	) -> std::int64_t
	{
		return convert<std::int64_t>(str, error);
	}

	inline static auto constexpr to_uint64(
		std::string_view str,
		std::string_view error
	) -> std::uint64_t
	{
		return convert<std::uint64_t>(str, error);
	}

	inline static auto constexpr to_float32(
		std::string_view str,
		std::string_view error
	) -> float
	{
		return convert<float>(str, error);
	}

	inline static auto constexpr to_float64(
		std::string_view str,
		std::string_view error
	) -> double
	{
		return convert<double>(str, error);
	}
}