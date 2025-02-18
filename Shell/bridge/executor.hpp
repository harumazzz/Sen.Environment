#pragma once

#include "bridge/data.hpp"

namespace Sen::Shell {

	using Callback = int(*)(Message* source, Message* destination);

	struct Service {

		Callback callback;

	};

	using Execute = int(*)(Message* argument, Service* service);
}