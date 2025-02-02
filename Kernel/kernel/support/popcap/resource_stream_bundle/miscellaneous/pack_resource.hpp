#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/pack.hpp"
#include "kernel/support/popcap/resource_stream_bundle/pack.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous
{
    

    struct PackResource
    { static auto process_whole(
            DataStreamView &stream,
            BundleStructure const &definition,
            ManifestStructure const &manifest,
            std::string_view source) -> void
        {
            return;
        }

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}