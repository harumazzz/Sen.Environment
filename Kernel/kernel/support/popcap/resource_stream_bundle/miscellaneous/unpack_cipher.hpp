#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/unpack.hpp"
#include "kernel/support/popcap/resource_stream_bundle/unpack.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous
{

	struct UnpackCipher
	{ 

		 static auto process_fs(
			std::string_view source,
			std::string_view destination) -> void
		{
		}
	};
}