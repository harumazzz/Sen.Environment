module;

import sen.environment.shell.bridge.data;

export module sen.environment.shell.bridge.executor;

export namespace Sen::Shell {

	using Callback = int(*)(StringList* list, String* destination);

	using Execute = int(*)(StringList* argument, Callback callback);

}