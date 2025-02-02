#pragma once

#include "kernel/support/popcap/res_info/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResInfo {

	struct Merge  {

		 static auto process(
			std::string_view source,
			std::string_view destination
		) -> void {
		}


        static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void {
		}
	};
}