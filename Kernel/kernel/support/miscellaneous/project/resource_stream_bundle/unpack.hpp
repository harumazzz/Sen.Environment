#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_bundle/definition.hpp"
#include "kernel/support/miscellaneous/project/stream_compressed_group/common.hpp"

namespace Sen::Kernel::Support::Miscellaneous::Project::ResourceStreamBundle
{


    struct Unpack : Common
    {
     static auto process_whole(
            DataStreamView &stream,
            std::string_view destination) -> void
        {
        }

        inline static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}