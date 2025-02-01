#pragma once

#include "kernel/utility/encryption/md5/common.hpp"

namespace Sen::Kernel::Encryption::MD5 {

    struct Hash : Common {

    protected:

        using Common = Common;

    public:

		static constexpr auto decode(
			u8* source,
			u32* destination,
			const usize& size
		) -> void
		{
			for (auto i = std::size_t{ 0 }, j = std::size_t{ 0 }; j < size; ++i, j += 4) {
				destination[i] = to_u32(source[j], source[j + 1], source[j + 2], source[j + 3]);
			}
		}

		static auto transform(
			u32* state,
			u8* block
		) -> void
		{
			auto a = state[0];
			auto b = state[1];
			auto c = state[2];
			auto d = state[3];
			auto x = std::array<u32, 16>{};
			decode(block, x.data(), 64);

			/* Round 1 */
			FF<u32>(a, b, c, d, x[0], s11, 0xd76aa478);
			FF<u32>(d, a, b, c, x[1], s12, 0xe8c7b756);
			FF<u32>(c, d, a, b, x[2], s13, 0x242070db);
			FF<u32>(b, c, d, a, x[3], s14, 0xc1bdceee);
			FF<u32>(a, b, c, d, x[4], s11, 0xf57c0faf);
			FF<u32>(d, a, b, c, x[5], s12, 0x4787c62a);
			FF<u32>(c, d, a, b, x[6], s13, 0xa8304613);
			FF<u32>(b, c, d, a, x[7], s14, 0xfd469501);
			FF<u32>(a, b, c, d, x[8], s11, 0x698098d8);
			FF<u32>(d, a, b, c, x[9], s12, 0x8b44f7af);
			FF<u32>(c, d, a, b, x[10], s13, 0xffff5bb1);
			FF<u32>(b, c, d, a, x[11], s14, 0x895cd7be);
			FF<u32>(a, b, c, d, x[12], s11, 0x6b901122);
			FF<u32>(d, a, b, c, x[13], s12, 0xfd987193);
			FF<u32>(c, d, a, b, x[14], s13, 0xa679438e);
			FF<u32>(b, c, d, a, x[15], s14, 0x49b40821);

			/* Round 2 */
			GG<u32>(a, b, c, d, x[1], s21, 0xf61e2562);
			GG<u32>(d, a, b, c, x[6], s22, 0xc040b340);
			GG<u32>(c, d, a, b, x[11], s23, 0x265e5a51);
			GG<u32>(b, c, d, a, x[0], s24, 0xe9b6c7aa);
			GG<u32>(a, b, c, d, x[5], s21, 0xd62f105d);
			GG<u32>(d, a, b, c, x[10], s22, 0x2441453);
			GG<u32>(c, d, a, b, x[15], s23, 0xd8a1e681);
			GG<u32>(b, c, d, a, x[4], s24, 0xe7d3fbc8);
			GG<u32>(a, b, c, d, x[9], s21, 0x21e1cde6);
			GG<u32>(d, a, b, c, x[14], s22, 0xc33707d6);
			GG<u32>(c, d, a, b, x[3], s23, 0xf4d50d87);
			GG<u32>(b, c, d, a, x[8], s24, 0x455a14ed);
			GG<u32>(a, b, c, d, x[13], s21, 0xa9e3e905);
			GG<u32>(d, a, b, c, x[2], s22, 0xfcefa3f8);
			GG<u32>(c, d, a, b, x[7], s23, 0x676f02d9);
			GG<u32>(b, c, d, a, x[12], s24, 0x8d2a4c8a);

			/* Round 3 */
			HH<u32>(a, b, c, d, x[5], s31, 0xfffa3942);
			HH<u32>(d, a, b, c, x[8], s32, 0x8771f681);
			HH<u32>(c, d, a, b, x[11], s33, 0x6d9d6122);
			HH<u32>(b, c, d, a, x[14], s34, 0xfde5380c);
			HH<u32>(a, b, c, d, x[1], s31, 0xa4beea44);
			HH<u32>(d, a, b, c, x[4], s32, 0x4bdecfa9);
			HH<u32>(c, d, a, b, x[7], s33, 0xf6bb4b60);
			HH<u32>(b, c, d, a, x[10], s34, 0xbebfbc70);
			HH<u32>(a, b, c, d, x[13], s31, 0x289b7ec6);
			HH<u32>(d, a, b, c, x[0], s32, 0xeaa127fa);
			HH<u32>(c, d, a, b, x[3], s33, 0xd4ef3085);
			HH<u32>(b, c, d, a, x[6], s34, 0x4881d05);
			HH<u32>(a, b, c, d, x[9], s31, 0xd9d4d039);
			HH<u32>(d, a, b, c, x[12], s32, 0xe6db99e5);
			HH<u32>(c, d, a, b, x[15], s33, 0x1fa27cf8);
			HH<u32>(b, c, d, a, x[2], s34, 0xc4ac5665);

			/* Round 4 */
			II<u32>(a, b, c, d, x[0], s41, 0xf4292244);
			II<u32>(d, a, b, c, x[7], s42, 0x432aff97);
			II<u32>(c, d, a, b, x[14], s43, 0xab9423a7);
			II<u32>(b, c, d, a, x[5], s44, 0xfc93a039);
			II<u32>(a, b, c, d, x[12], s41, 0x655b59c3);
			II<u32>(d, a, b, c, x[3], s42, 0x8f0ccc92);
			II<u32>(c, d, a, b, x[10], s43, 0xffeff47d);
			II<u32>(b, c, d, a, x[1], s44, 0x85845dd1);
			II<u32>(a, b, c, d, x[8], s41, 0x6fa87e4f);
			II<u32>(d, a, b, c, x[15], s42, 0xfe2ce6e0);
			II<u32>(c, d, a, b, x[6], s43, 0xa3014314);
			II<u32>(b, c, d, a, x[13], s44, 0x4e0811a1);
			II<u32>(a, b, c, d, x[4], s41, 0xf7537e82);
			II<u32>(d, a, b, c, x[11], s42, 0xbd3af235);
			II<u32>(c, d, a, b, x[2], s43, 0x2ad7d2bb);
			II<u32>(b, c, d, a, x[9], s44, 0xeb86d391);

			state[0] += a;
			state[1] += b;
			state[2] += c;
			state[3] += d;
		}

		static auto initialize(
			u8* source,
			const usize& len,
			u32* count,
			u8* buffer,
			u32* state
		) -> void
		{
			auto index = static_cast<u32>((count[0] >> 3) & 0x3f);
			auto part = u32{};
			if ((count[0] += (static_cast<u32>(len) << 3)) < (static_cast<u32>(len) << 3)) {
				++count[1];
			}
			count[1] += (static_cast<u32>(len) >> 29);
			part = 64 - index;
			auto i = u32{};
			if (len >= part) {
				std::memcpy(&buffer[index], source, part);
				transform(state, buffer);
				for (i = part; i + 63 < len; i += 64) {
					transform(state, &source[i]);
				}
				index = 0;
			}
			else {
				i = 0;
			}
			std::memcpy(&buffer[index], &source[i], len - i);
		}

		static auto process(
			Uint8Array& source,
			Uint8Array& destination
		) -> void
		{
			auto count = std::to_array<u32>({ 0, 0 });
			auto padding = std::array<u8, 64>{ 0x80 };
			auto state = std::to_array<u32>({ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 });
			auto buffer = std::array<u8, 64>{};
			initialize(source.begin(), source.size(), count.data(), buffer.data(), state.data());
			auto bits = std::array<u8, 8>{};
			auto old_state = std::array<u32, 4>{};
			auto old_count = std::array<u32, 2>{};
			std::memcpy(old_state.data(), state.data(), sizeof(u32) * state.size());
			std::memcpy(old_count.data(), count.data(), sizeof(u32) * count.size());
			encode(count.data(), bits.data(), 8);
			auto index = static_cast<u32>((count[0] >> 3) & 0x3f);
			auto padding_length = static_cast<u32>((index < 56) ? (56 - index) : (120 - index));
			initialize(padding.data(), padding_length, count.data(), buffer.data(), state.data());
			initialize(bits.data(), sizeof(u8) * bits.size(), count.data(), buffer.data(), state.data());
			encode(state.data(), destination.begin(), sizeof(u32) * state.size());
			std::memcpy(state.data(), old_state.data(), sizeof(u32) * state.size());
			std::memcpy(count.data(), old_count.data(), sizeof(u32) * count.size());
		}

    };

	inline auto quick_hash (
		Uint8Array& source,
		String& destination
	) -> void {
		destination.allocate(compute_size());
		auto temporary = Uint8Array{compute_size()};
		Hash::process(source, temporary);
		make_string(temporary, destination);
	}

}