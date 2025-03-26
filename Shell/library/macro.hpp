#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
	#define WINDOWS 1
#else
	#define WINDOWS 0
#endif

#ifdef __linux__
#define LINUX 1
#else
#define LINUX 0
#endif

#define ANDROID __ANDROID__

#define APPLE __APPLE__

#define IPHONE TARGET_OS_IPHONE

#ifdef __MACH__
#define MACINTOSH 1
#else
#define MACINTOSH 0
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
