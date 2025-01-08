module;

import <string>;
import <vector>;

export module sen.environment.shell.bridge.data;

export namespace Sen::Shell {

	export struct String {

		const char* value{ nullptr };

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