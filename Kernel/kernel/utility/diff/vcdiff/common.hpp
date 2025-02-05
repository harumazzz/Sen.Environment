#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Diff::VCDiff {

    struct Common {

        inline static auto constexpr limit = std::numeric_limits<std::int32_t>::max();

        inline static auto constexpr maximum_window_size = 0x7FFFFFFF_size;

    };
}