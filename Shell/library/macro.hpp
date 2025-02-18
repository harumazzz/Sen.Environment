#pragma once

#define WINDOWS _WIN32

#define LINUX __linux__

#define ANDROID __ANDROID__

#define APPLE __APPLE__

#define IPHONE TARGET_OS_IPHONE

#define MACINTOSH __MACH__

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#define MAIN_FUNCTION int wmain(int argc, wchar_t* argv[])
#else
#define MAIN_FUNCTION int main(int argc, char* argv[])
#endif

#define assert_conditional(condition, message) \
	if(!(condition)) {\
		throw std::runtime_error(message); \
	}\
	static_assert(true)

#define thiz (*this)

namespace Sen::Shell {

}