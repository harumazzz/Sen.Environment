#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/assert.hpp"

namespace Sen::Kernel::Encryption::Rijndael {

	enum class Mode : uint8_t {
		ECB,
		CBC,
		CFB,
	};

	struct Common {

		template <typename T> requires std::is_integral_v<T>
		inline static auto constexpr is_padded_size(
			const T& size,
			const T& block_size
		) -> bool
		{
    		return size % block_size == 0;
		}

		template <typename T> requires std::is_integral_v<T>
		inline static auto constexpr is_valid_block_size(
			const T& block_size
		) -> bool {
    		return block_size == 128 || block_size == 160 || block_size == 192 || block_size == 224 || block_size == 256;
		}
	};
}