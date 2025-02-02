#pragma once

#include "kernel/support/popcap/animation/definition.hpp"
#include "kernel/support/popcap/animation/convert/definition.hpp"
#include "kernel/support/popcap/animation/convert/common.hpp"

namespace Sen::Kernel::Support::PopCap::Animation::Convert {
    class FromFlash : Common {
        

        static  auto process_fs(
                std::string_view source,
                std::string_view destination) -> void {
        }
    };

}