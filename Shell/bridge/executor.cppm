module;

import sen.environment.shell.bridge.data;

export module sen.environment.shell.bridge.executor;

export namespace Sen::Shell {

	using Callback = void(*)(StringList* list, String* destination);

	using Execute = int(*)(String* script, StringList* argument, Callback m_callback);

}