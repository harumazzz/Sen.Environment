#pragma once

#include <cstdint>

namespace Sen::Shell {

	struct String {

		uint8_t* value{ nullptr };

		size_t size{0};

	};

	struct StringList {

		String* value{nullptr};
		
		size_t size{0};

	};

}