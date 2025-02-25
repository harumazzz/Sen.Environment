#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/unpack.hpp"
#include "kernel/support/popcap/resource_stream_bundle/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle
{

    struct Unpack : Common
    { 

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}