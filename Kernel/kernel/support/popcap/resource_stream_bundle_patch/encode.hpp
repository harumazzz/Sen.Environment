#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_bundle/unpack.hpp"
#include "kernel/support/popcap/resource_stream_bundle_patch/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundlePatch
{
    

    struct Encode : Common
    { 


         static auto process_fs(
            std::string_view source_before,
            std::string_view source_after,
            std::string_view destination
        ) -> void
        {
        }
    };

}