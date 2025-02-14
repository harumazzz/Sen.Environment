#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/miscellaneous/shared.hpp"
#include "kernel/support/popcap/player_info/definition.hpp"
#include "kernel/support/popcap/player_info/common.hpp"

namespace Sen::Kernel::Support::PopCap::PlayerInfo
{

    struct Decode : Common
    { 

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };
}