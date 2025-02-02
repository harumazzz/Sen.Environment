#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/miscellaneous/shared.hpp"
#include "kernel/support/wwise/sound_bank/definition.hpp"

namespace Sen::Kernel::Support::WWise::SoundBank
{


    struct Decode : Common
    { static auto process_whole() -> void
        {
        }

         static auto process_fs(
            std::string_view source,
            std::string_view destination) -> void
        {
        }
    };

}