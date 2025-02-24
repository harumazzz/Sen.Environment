#pragma once

#include "kernel/interface/data.hpp"

namespace Sen::Kernel::Interface {

	template <typename T>
	using Pointer = T*;

	struct Service;

	using Callback = int(*)(Service* service, Message* source, Message* destination);

	using Allocate = int(*)(Message* source, size_t* size);

	struct Service {

		Callback callback;

		Allocate allocate;

	};
}