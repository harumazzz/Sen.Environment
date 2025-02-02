#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/definition.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"
#include "kernel/support/popcap/zlib/uncompress.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    

    class Decode : Common
    {static auto process_fs(
            std::string_view source,
            std::string_view destination,
            ReanimPlatform platform
        ) -> void
        {
        }
    };

}