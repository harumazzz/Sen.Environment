#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/player_info/model.hpp"
#include "kernel/support/popcap/player_info/common.hpp"

namespace Sen::Kernel::Support::PopCap::PlayerInfo
{

    struct Encode : Common
    { 

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
            return;
        }
    };
}