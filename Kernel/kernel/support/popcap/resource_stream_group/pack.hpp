#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{

    struct Pack : Common
    {

         static auto process_fs(
            StringView const& source,
            StringView const& destination
        ) -> void
        {
        }
    };

}