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
        Javascript::Context& context,
        Javascript::Value& object,
        Array<Javascript::Value>& arguments,
        Javascript::Value& result
    ) -> void {
        result.set(Kernel::version);
    }

}