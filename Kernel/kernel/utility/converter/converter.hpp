#pragma once

#include "kernel/utility/converter/common.hpp"

namespace Sen::Kernel::Converter {

	inline auto constexpr to_int8(
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