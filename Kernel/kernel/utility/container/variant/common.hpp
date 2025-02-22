#pragma once

#include "kernel/utility/container/base/common.hpp"

namespace Sen::Kernel {


}

namespace std {

    template <typename... Ts>
    inline auto operator<<(
        std::ostream& os,
        const std::variant<Ts...>& v
    ) -> std::ostream& {
        std::visit([&os](const auto& value) { os << value; }, v);
        return os;
    }

}