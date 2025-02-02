#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/package/definition.hpp"

namespace Sen::Kernel::Support::PopCap::Package
{
    struct Pack : Common
    {

         static auto process_whole(
            DataStreamView &stream,
            std::string_view source,
            std::string_view destination) -> void
        {
        }

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}