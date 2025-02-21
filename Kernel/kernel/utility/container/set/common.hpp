#pragma once

#include "kernel/utility/container/base/common.hpp"

namespace Sen::Kernel {

    template <typename T>
    using HashSet = Subprojects::phmap::flat_hash_set<T>;

}
