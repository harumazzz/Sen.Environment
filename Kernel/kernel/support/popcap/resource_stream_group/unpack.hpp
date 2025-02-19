#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
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