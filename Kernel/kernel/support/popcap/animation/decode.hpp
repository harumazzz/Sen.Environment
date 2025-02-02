#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/animation/definition.hpp"
#include "kernel/support/popcap/animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::Animation
{

    struct Decode
    {
        static auto process_whole() -> void
        {
        }

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };

}