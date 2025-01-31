#pragma once

#include "kernel/utility/container/array/array.hpp"

namespace Sen::Kernel {

    struct Common {

        static constexpr auto max_capacity(
        ) -> size_t {
            return std::numeric_limits<size_t>::max();
        }

    };

}