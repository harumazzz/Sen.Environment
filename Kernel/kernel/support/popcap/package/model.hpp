#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::Package
{
    struct Resource
    {
        String path;

        constexpr explicit Resource() = default;

        ~Resource() = default;

        friend auto operator << (
            std::ostream& os,
            const Resource& other
        ) -> std::ostream& {
            os << "Resource(" << other.path << ")";
            return os;
        }
    };

    struct PackageInfo
    {
        List<Resource> resource;

        constexpr explicit PackageInfo() = default;

        ~PackageInfo() = default;

        friend auto operator << (
            std::ostream& os,
            const PackageInfo& other
        ) -> std::ostream& {
            os << "PackageInfo(" << other.resource << ")";
            return os;
        }
    };
}

using namespace Sen::Kernel::Support::PopCap::Package;

JSONCONS_ALL_MEMBER_TRAITS(Resource, path);

JSONCONS_ALL_MEMBER_TRAITS(PackageInfo, resource);