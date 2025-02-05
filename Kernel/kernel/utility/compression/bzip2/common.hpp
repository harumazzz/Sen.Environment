#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Compression::bzip2 {

    struct Common {

        inline static auto constexpr EXTRA_ALLOCATION = 128_size;

    };

}