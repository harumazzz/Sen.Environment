#pragma once

#include "kernel/utility/container/base/common.hpp"

namespace Sen::Kernel {

    template <typename T>
    using HashSet = Subprojects::phmap::flat_hash_set<T>;

    template <typename T>
    inline auto operator<<(
        std::ostream& os,
        const HashSet<T>& set
    ) -> std::ostream& {
        os << "{";
        for (auto index : Range{set.size()}) {
            os << set[index];
            if (index < set.size() - 1) {
                os << ", ";
            }
        }
        os << "}";
        return os;
    }

}
