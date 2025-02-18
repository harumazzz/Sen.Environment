#pragma once

#include "kernel/interface/data.hpp"

namespace Sen::Kernel::Interface {

	template <typename T>
	using Pointer = T*;

	using Callback = int(*)(Message* source, Message* destination);

	struct Service {

		Callback callback;

	};
}