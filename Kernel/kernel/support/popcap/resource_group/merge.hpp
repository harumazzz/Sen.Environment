#pragma once

#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup {

	struct Merge : Common {
 static auto process(
			std::string_view source,
			std::string_view destination,
			const nlohmann::ordered_json& content
		) -> void {
		}

		 static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void {
		}

	};

}