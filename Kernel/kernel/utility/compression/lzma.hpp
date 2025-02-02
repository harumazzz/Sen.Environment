#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/filesystem.hpp"

namespace Sen::Kernel::Compression {

	/**
	 * Lzma compression and uncompression
	*/

	class Lzma {

		public:

			explicit Lzma(

			) = default;


			~Lzma(

			) = default;

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

	};

}