#pragma once

#define WINDOWS _WIN32

#define LINUX __linux__

#define ANDROID __ANDROID__

#define APPLE __APPLE__

#define IPHONE TARGET_OS_IPHONE

#define MACINTOSH __MACH__

#if WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#if WINDOWS
#define MAIN_FUNCTION int wmain(int size, wchar_t** argc)
#else
#define MAIN_FUNCTION int main(int size, char** argc)
#endif

#define assert_conditional(condition, message) \
	if(!(condition)) {\
		throw std::runtime_error(message); \
	}\
	static_assert(true)