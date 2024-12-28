#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <math.h>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <regex>
#include <version>
#include <stdio.h>
#include <future>
#include <map>
#include <stack>
#include <list>
#include <stdlib.h>
#include <string.h>
#include <array>
#include <cstdio>
#include <cstring>
#include <exception>
#include <span>
#include "library/color.hpp"
#include "String.hpp"

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
struct StringView {
	size_t size;
	const char* value{ nullptr };
};

struct StringList {
	StringView* value;
	size_t size;

	inline static auto to_vector(
		const StringList& that
	) -> std::vector<std::string>
	{
		auto destination = std::vector<std::string>{};
		for (auto i = 0; i < that.size; ++i)
		{
			destination.emplace_back(std::string{ that.value[i].value, that.value[i].size });
		}
		return destination;
	}
};

using CStringView = StringView;

using CStringList = StringList;

typedef void(*ShellCallback)(CStringList* list, CStringView* destination);

class MemoryBuffer {
public:
    MemoryBuffer(std::size_t size) {
		this->allocate(size);
	}

    ~MemoryBuffer() {
        this->release();
    }

	inline static auto release(

	) -> void
	{
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}

	inline static auto has_value (

	) -> bool
	{
		return buffer != nullptr;
	}

	inline static auto allocate(
		std::size_t size
	) -> void {
		if (has_value()) {
			release();
		}
		buffer = new char[size];
		buffer[size - 1] = '\0';
	}

    inline static auto get(

	) -> char* {
        return buffer;
    }

private:
    inline static char* buffer{nullptr};
};

typedef int (*execute)
(CStringView* script, CStringList* argument, ShellCallback m_callback);

#define assert_conditional(condition, message) \
	if(!(condition)) {\
		throw std::runtime_error(message); \
	}


namespace Sen::Shell {

}