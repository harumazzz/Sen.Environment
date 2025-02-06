#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel::Encryption::CRC {

	template <typename polinomial> requires std::is_integral_v<polinomial>
	struct Common;

	template <typename polinomial> requires std::is_integral_v<polinomial>
	struct Common {

		protected:

			using Polinomial = polinomial;

			using Type = polinomial;

			using value_type = Type;

			inline static constexpr auto polynomial = static_cast<polinomial>(0xEDB88320);

		protected:

			constexpr explicit Common(

			) = default;

			constexpr ~Common(

			) = default;

			Common (
				const Common& other
			) = delete;

			Common (
				Common&& other
			) = delete;

			auto operator =(
				const Common& other
			) -> Common& = delete;

			auto operator =(
				Common&& other
			) -> Common& = delete;

			template <typename T, auto size> requires is_numeric_v<T> && is_numeric_v<type_of<size>>
			static auto constexpr generate_table(
				std::array<T, size>* table
			) -> void
			{
				for (auto i = 0; i < table->size(); ++i)
				{
					auto c = static_cast<T>(i);
					for (auto j = 0; j < 8; ++j)
					{
						if (c & 1) {
							c = polynomial ^ (c >> 1);
						}
						else {
							c >>= 1;
						}
					}
					(*table)[i] = c;
				}
			}

			template <typename T, auto size> requires is_numeric_v<T> && is_numeric_v<type_of<size>>
			static constexpr auto update (
				const std::array<T, size> & table,
				const T& initial,
				Uint8Array & buffer
			) -> T
			{
				auto c = initial ^ 0xFFFFFFFF;
				for (auto & e : buffer) {
					c = table[(c ^ e) & 0xFF] ^ (c >> 8);
				}
				return c ^ 0xFFFFFFFF;
			}

			template <typename T, auto size> requires is_numeric_v<T> && is_numeric_v<type_of<size>>
			static auto constexpr compute (
				const std::array<T, size> & table,
				const T& initial,
				Uint8Array & buffer
			) -> T
			{
				return update<T, size>(table, initial, buffer);
			}
	};

}
