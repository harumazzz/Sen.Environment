#pragma once

#include "bridge/data.hpp"

namespace Sen::Shell {

	using Callback = int(*)(StringList* list, String* destination);

	using Execute = int(*)(StringList* argument, Callback callback);

}