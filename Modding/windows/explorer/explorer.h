#pragma once

#pragma warning(push)
#pragma warning(disable:4191)
#pragma warning(disable:4265)
#pragma warning(disable:4267)
#pragma warning(disable:4324)
#pragma warning(disable:4365)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:5026)
#pragma warning(disable:5027)
#pragma warning(disable:5039)
#pragma warning(disable:5204)
#pragma warning(disable:5220)

#include <string>
#include <regex>
#include <optional>
#include <array>
#include <vector>
#include <unordered_map>
#include <codecvt>
#include <exception>
#include <filesystem>
#include <Windows.h>
#include <ShObjIdl_core.h>
#include <wrl/module.h>
#include <wrl/implements.h>
#include <wrl/client.h>
#include <wil/resource.h>

#pragma warning(pop)

#define assert(conditional, message)\
    if (!conditional) {\
        throw std::runtime_error{message};\
    }\
    static_assert(true)

using Microsoft::WRL::RuntimeClass;
using Microsoft::WRL::RuntimeClassFlags;
using Microsoft::WRL::ClassicCom;
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Make;
using Microsoft::WRL::Module;
using Microsoft::WRL::InProc;