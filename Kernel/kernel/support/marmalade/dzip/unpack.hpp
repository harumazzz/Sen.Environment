#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/marmalade/dzip/definition.hpp"
#include "kernel/support/marmalade/dzip/common.hpp"

namespace Sen::Kernel::Support::Marmalade::DZip
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