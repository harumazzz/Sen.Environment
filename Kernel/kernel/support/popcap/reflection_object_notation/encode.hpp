#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/reflection_object_notation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReflectionObjectNotation
{

    class Encode : Common
    {

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
            return;
        }
    };

}