#pragma once

namespace Sen::Kernel {

	using u8 = std::uint8_t;

	using u16 = std::uint16_t;

	using u32 = std::uint32_t;

	using u64 = std::uint64_t;

	using i8 = std::int8_t;

	using i16 = std::int16_t;

	using i32 = std::int32_t;

	using i64 = std::int64_t;

	using usize = std::size_t;

	using uchar = unsigned char;

	using f32 = float;

	using f64 = double;

	inline constexpr auto operator "" _c(
		char c
	) -> char
	{
		return c;
	}

	inline constexpr auto operator "" _uc(
		char c
	) -> unsigned char
	{
		return static_cast<unsigned char>(c);
	}

	inline constexpr auto operator "" _byte(
		unsigned long long c
	) -> unsigned char
	{
		return static_cast<unsigned char>(c);
	}

	inline constexpr auto operator "" _us(
		unsigned long long n
	) -> unsigned short
	{
    	return static_cast<unsigned short>(n);
	}

	inline constexpr auto operator "" _ui(
		unsigned long long n
	) -> unsigned int
	{
    	return static_cast<unsigned int>(n);
	}

	inline constexpr auto operator "" _size(
		unsigned long long n
	) -> size_t
	{
    	return static_cast<size_t>(n);
	}

	inline constexpr auto operator "" _ul(
		unsigned long long n
	) -> unsigned long
	{
    	return static_cast<unsigned long>(n);
	}

	inline static auto constexpr operator "" _sv(
		const char* str, std::size_t len
	) noexcept -> std::string_view
	{
		return std::string_view{ str, len };
	}

	inline constexpr auto operator "" _u8(
		unsigned long long c
	) -> uint8_t
	{
		return static_cast<uint8_t>(c);
	}

	inline constexpr auto operator "" _i8(
		unsigned long long c
	) -> std::int8_t
	{
		return static_cast<std::int8_t>(c);
	}

	inline constexpr auto operator "" _u16(
		unsigned long long c
	) -> uint16_t
	{
		return static_cast<uint16_t>(c);
	}

	inline constexpr auto operator "" _i16(
		unsigned long long c
	) -> int16_t
	{
		return static_cast<int16_t>(c);
	}

	inline constexpr auto operator "" _u32(
		unsigned long long c
	) -> uint32_t
	{
		return static_cast<uint32_t>(c);
	}

	inline constexpr auto operator "" _i32(
		unsigned long long c
	) -> int32_t
	{
		return static_cast<int32_t>(c);
	}

	inline constexpr auto operator "" _u64(
		unsigned long long c
	) -> uint64_t
	{
		return static_cast<uint64_t>(c);
	}

	inline constexpr auto operator "" _i64(
		unsigned long long c
	) -> int64_t
	{
		return static_cast<int64_t>(c);
	}
}