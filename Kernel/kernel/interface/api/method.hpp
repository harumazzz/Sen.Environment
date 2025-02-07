#pragma once

#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"

namespace Sen::Kernel::Interface::API {

    inline auto version (
    ) -> usize {
        return Kernel::version;
    }

}