module;

#include <string>
#include <vector>
#include <cstdint>

export module sen.environment.shell.bridge.data;

export namespace Sen::Shell {

	export struct String {

		uint8_t* value{ nullptr };

		size_t size{0};

	};

	export struct StringList {

		String* value{nullptr};
		
		size_t size{0};

	};

	// TODO : Implement Executor

	export struct Executor {
		
	};

}