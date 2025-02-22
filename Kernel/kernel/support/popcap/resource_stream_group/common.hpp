#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{
    struct Common
    {
        static constexpr auto k_magic_identifier = 0x72736770_ui;

        static constexpr auto version_list = {3_ui, 4_ui}; // unsupported rsg version 1.

        static constexpr auto contain_version(u32 const & version) -> bool
        {
            return std::ranges::find(version_list, version) != version_list.end();
        }
    };
}
