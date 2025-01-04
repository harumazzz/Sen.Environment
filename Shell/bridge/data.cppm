module;

import <string>;
import <vector>;

export module sen.environment.shell.bridge.data;

export namespace Sen::Shell {

	export struct String {

		size_t size{0};

		const char* value{ nullptr };

	};

	export struct StringList {

		String* value{nullptr};
		
		size_t size{0};

	};

	// TODO : Implement Executor

	export struct Executor {
		
	};

}