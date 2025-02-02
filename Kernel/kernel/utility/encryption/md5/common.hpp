#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Encryption::MD5 {

	using String = BasicString;

    struct Common {

        inline static constexpr auto s11 = 7;

        inline static constexpr auto s12 = 12;

        inline static constexpr auto s13 = 17;

        inline static constexpr auto s14 = 22;

        inline static constexpr auto s21 = 5;

        inline static constexpr auto s22 = 9;

        inline static constexpr auto s23 = 14;

        inline static constexpr auto s24 = 20;

        inline static constexpr auto s31 = 4;

        inline static constexpr auto s32 = 11;

        inline static constexpr auto s33 = 16;

        inline static constexpr auto s34 = 23;

        inline static constexpr auto s41 = 6;

        inline static constexpr auto s42 = 10;

        inline static constexpr auto s43 = 15;

        inline static constexpr auto s44 = 21;

        template <typename T> requires std::is_integral_v<T>
		static constexpr auto rotate_left(
			const T& num,
			const T& n
		) -> T {
			return (num << n) | (num >> (32 - n));
		}

		template <typename T> requires std::is_integral_v<T>
		static constexpr auto F(
			const T& x,
			const T& y,
			const T& z
		) -> T {
			return (x & y) | (~x & z);
		}

		template <typename T> requires std::is_integral_v<T>
		static constexpr auto G(
			const T& x,
			const T& y,
			const T& z
		) -> T {
			return (x & z) | (y & ~z);
		}

		template <typename T> requires std::is_integral_v<T>
		static constexpr auto H(
			const T& x,
			const T& y,
			const T& z
		) -> T {
			return x ^ y ^ z;
		}

		template <typename T> requires std::is_integral_v<T>
		static constexpr auto I(
			const T& x,
			const T& y,
			const T& z
		) -> T {
			return y ^ (x | ~z);
		}

		template <typename T> requires std::is_integral_v<T>
		static auto constexpr FF(
			T& a,
			const T& b,
			const T& c,
			const T& d,
			const T& x,
			const T& s,
			const T& ac
		) -> void
		{
			a += F(b, c, d) + x + ac;
			a = rotate_left<T>(a, s);
			a += b;
		}

		template <typename T> requires std::is_integral_v<T>
		static auto GG(
			T& a,
			const T& b,
			const T& c,
			const T& d,
			const T& x,
			const T& s,
			const T& ac
		) -> void
		{
			a += G(b, c, d) + x + ac;
			a = rotate_left<T>(a, s);
			a += b;
		}

		template <typename T> requires std::is_integral_v<T>
		static auto HH(
			T& a,
			const T& b,
			const T& c,
			const T& d,
			const T& x,
			const T& s,
			const T& ac
		) -> void
		{
			a += H(b, c, d) + x + ac;
			a = rotate_left<T>(a, s);
			a += b;
		}

		template <typename T> requires std::is_integral_v<T>
		static auto II(
			T& a,
			const T& b,
			const T& c,
			const T& d,
			const T& x,
			const T& s,
			const T& ac
		) -> void
		{
			a += I(b, c, d) + x + ac;
			a = rotate_left<T>(a, s);
			a += b;
		}

		static constexpr auto to_u8(
			const u32& b
		) -> u8
		{
			return static_cast<u8>(b & 0xff);
		}

		static constexpr auto encode(
			u32* source,
			u8* destination,
			const usize& size
		) -> void
		{
			for (auto i = size_t{ 0 }, j = size_t{ 0 }; j < size; ++i, j += 4) {
				destination[j] = to_u8(source[i]);
				destination[j + 1] = to_u8(source[i] >> 8);
				destination[j + 2] = to_u8(source[i] >> 16);
				destination[j + 3] = to_u8(source[i] >> 24);
			}
		}

		static constexpr auto to_u32(
			u8 byte0,
			u8 byte1,
			u8 byte2,
			u8 byte3
		) -> u32
		{
			return static_cast<u32>(byte0) |
				(static_cast<u32>(byte1) << 8) |
				(static_cast<u32>(byte2) << 16) |
				(static_cast<u32>(byte3) << 24);
		}

    };

	namespace Detail {

		inline static constexpr auto Table = std::array<char, 16>{
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			'8', '9', 'a', 'b',
			'c', 'd', 'e', 'f',
		};

	};

	constexpr auto compute_size (
	) -> usize {
		return static_cast<usize>(16 << 1);
	}

	inline auto make_string (
		Uint8Array& source,
		String& destination
	) -> void {
		destination.allocate(compute_size());
		for (auto & e : source) {
			destination.append(1, Detail::Table[e / 16]);
			destination.append(1, Detail::Table[e % 16]);
		}
	}
}