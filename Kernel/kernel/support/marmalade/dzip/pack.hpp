#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/marmalade/dzip/definition.hpp"

namespace Sen::Kernel::Support::Marmalade::DZip
{

    struct Pack : Common
    {

         static auto process_whole(
            DataStreamView &stream,
            std::string_view source) -> void
        {
        }

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };

}