#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/unpack.hpp"
#include "kernel/support/popcap/resource_stream_bundle/definition.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle
{

    struct Unpack : Common
    { static auto process_whole(
            DataStreamView &stream,
            BundleStructure &definition,
            ManifestStructure &manifest) -> void
        {
        }

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}