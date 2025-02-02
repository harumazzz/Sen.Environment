#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::Animation::Convert
{

	struct Resize : Common
	{
		 static auto process_whole(
			std::string_view source,
			int const &resolution) -> void
		{
		}

		 static auto process_fs(
			std::string_view source,
			int const &resolution) -> void
		{
		}
	};
}