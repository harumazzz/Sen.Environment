#pragma once

#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup { 

	struct Convert  {
 static auto process_whole(
				const nlohmann::ordered_json& resource_group
			) -> nlohmann::ordered_json 
			{
			}

			 static auto process_fs(
				std::string_view source, 
				std::string_view destination
			) -> void
			{
			}
	};
}