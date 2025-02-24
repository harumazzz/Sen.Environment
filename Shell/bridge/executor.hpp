#pragma once

#include "bridge/data.hpp"

namespace Sen::Shell {

	struct Service;

	using Callback = int(*)(Service* service, Message* source, Message* destination);

	using Allocate = int(*)(Message* source, size_t* size);

	struct Service {

		Callback callback;

		Allocate allocate;

	};

	using Execute = int(*)(Message* argument, Service* service);
}