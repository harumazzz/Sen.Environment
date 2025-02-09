#pragma once

#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::Interface::API {

    template <typename T>
    using List = CList<T>;

    template <typename T>
    using Array = CArray<T>;

    inline auto version (
    ) -> usize {
        return Kernel::version;
    }

}