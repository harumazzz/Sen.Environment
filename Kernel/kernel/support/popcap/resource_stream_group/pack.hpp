#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/definition.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{

    struct Pack : Common
    {

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };

}