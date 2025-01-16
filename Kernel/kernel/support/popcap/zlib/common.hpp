#pragma once

#include "kernel/utility/compression/zlib.hpp"

namespace Sen::Kernel::Support::PopCap::Zlib {

	struct Common {

		inline static auto constexpr magic = 0xDEADFED4;


		inline static auto const level = Kernel::Compression::Zlib::Level::LEVEL_9;

	};

}
