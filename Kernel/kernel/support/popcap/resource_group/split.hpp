#pragma once

#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup {

	struct Split : public Common {

         static auto process_fs(
            std::string_view source, 
            std::string_view destination
        ) -> void {
        }
    };


}