#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/definition.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    

    class FromXML : Common
    { static auto process_fs(
            std::string_view source,
            std::string_view destination
        ) -> void
        {
        }
    };

}