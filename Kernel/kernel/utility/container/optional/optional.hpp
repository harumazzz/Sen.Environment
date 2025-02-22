#pragma once

#include "kernel/utility/container/base/common.hpp"

namespace Sen::Kernel {
    template <typename T>
    using Optional = std::optional<T>;
}

namespace std {

    template <typename T>
    inline auto operator<<(
        std::ostream& os,
        const std::optional<T>& opt
    ) -> std::ostream& {
        if (opt.has_value()) {
            os << "{" << *opt << "}";
        } else {
            os << "null";
        }
        return os;
    }

}