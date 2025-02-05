#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Compression::Lzma {

	enum class Level : std::uint8_t
	{
		LEVEL_0,
		LEVEL_1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_5,
		LEVEL_6,
		LEVEL_7,
		LEVEL_8,
		LEVEL_9,
	};

	struct Common {

		static auto constexpr compute_destination_size (
			const usize& data_size
		) -> usize {
			return static_cast<usize>(data_size * 1.1 + 1024);
		}

	};

}
