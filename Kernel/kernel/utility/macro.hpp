#pragma once 

#include "kernel/utility/library.hpp"

#define thiz (*this)

#define VAR_TO_STRING(x) #x

#ifdef DEBUG_LOG

    template<typename... Args>
    auto debugger(
        Args&&... args
    ) -> void {
        std::cout << std::boolalpha;
        (std::cout << ... << args) << '\n' << std::flush;
    }

    #define debug(...) debugger(__VA_ARGS__)
#else
    #define debug(...)
#endif


#define debug_json(arg) std::cout << arg << '\n' << std::flush;

#define WINDOWS _WIN32

#define LINUX __linux__

#define ANDROID __ANDROID__

#define APPLE __APPLE__

#define IPHONE TARGET_OS_IPHONE

#define MACINTOSH __MACH__

#if WINDOWS
#define M_EXPORT_API extern "C" __declspec(dllexport)
#else
#define M_EXPORT_API extern "C" __attribute__((visibility("default")))
#endif

#if WINDOWS
#include <windows.h>
#else
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <csetjmp>
#include <csignal>
#endif

#if APPLE
#include <Carbon/Carbon.h>
#endif

#define HAS_ARGS(...) BOOL(FIRST(__VA_ARGS__))
#define BOOL(x) BOOL_ ## x
#define BOOL_0 0
#define BOOL_1 1
#define FIRST(a, ...) a
#define IF_ELSE(condition) _IF_ELSE(BOOL(condition))
#define _IF_ELSE(condition) _IF_ ## condition
#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE
#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#if WINDOWS
    #if defined(__x86_64__) || defined(_M_X64)
    #define fsize _ftelli64
    #else
    #define fsize std::ftell
    #endif
#else
    #if defined(__x86_64__) || defined(_M_X64)
    #define fsize ftello64
    #else
    #define fsize std::ftell
    #endif
#endif

namespace Sen::Kernel {
	
}