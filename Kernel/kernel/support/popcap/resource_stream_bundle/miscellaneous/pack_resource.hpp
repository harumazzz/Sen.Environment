#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/pack.hpp"
#include "kernel/support/popcap/resource_stream_bundle/pack.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous
{
    

    struct PackResource
    { 

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}