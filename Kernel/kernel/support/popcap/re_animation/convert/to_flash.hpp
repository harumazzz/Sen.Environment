#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/model.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation::Convert
{

	struct ToFlash
	{static auto process_fs(
			std::string_view source,
			std::string_view destination) -> void
		{
		}
	};
}