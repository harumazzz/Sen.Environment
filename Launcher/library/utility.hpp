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
#include <cstdio>
#include <memory>
#include <array>
#include <cstdlib>
#include <print> 
#ifdef _WIN32
#include <cwchar>
#include <ShlObj_core.h>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <limits.h>
#endif
#include "deps/json/json.hpp"
#include "deps/zip/zip.h"


#if _WIN32
#define MAIN int wmain(int argc, wchar_t* argv[])
#else
#define MAIN int main(int argc, char* argv[])
#endif

namespace Sen::Launcher {

}