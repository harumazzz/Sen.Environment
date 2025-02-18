#pragma once

#include <cstdint>

namespace Sen::Shell {

	struct Message {

		uint8_t* value{ nullptr };

		size_t size{0};

	};

}