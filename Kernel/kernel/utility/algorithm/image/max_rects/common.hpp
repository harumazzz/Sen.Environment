#pragma once
#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Image::MaxRects {

    enum class LOGIC : u8
    {
        MAX_AREA,
        MAX_EDGE,
    };

    struct Common {

        inline static auto constexpr EDGE_MAX_VALUE = 4096_size;

        inline static auto constexpr EDGE_MIN_VALUE = 128_size;

        inline static auto constexpr k_score = 1.7976931348623157e+308;

        inline static auto constexpr k_log_2e = 1.4426950408889634;

    };

}
