#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::Package
{
    struct Resource
    {
        String path;

        constexpr explicit Resource() = default;

        ~Resource() = default;
    };

    struct PackageInfo
    {
        List<Resource> resource;

        constexpr explicit PackageInfo() = default;

        ~PackageInfo() = default;
    };
}

using namespace Sen::Kernel::Support::PopCap::Package;

JSONCONS_ALL_MEMBER_TRAITS(Resource, path);

JSONCONS_ALL_MEMBER_TRAITS(PackageInfo, resource);