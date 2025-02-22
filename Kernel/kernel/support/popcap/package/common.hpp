#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::Package
{
    enum PackagePlatformEnumeration : uint8_t
    {

    };

    struct Common
    {
        static constexpr auto k_magic_identifier = 0xBAC04AC0_ui;

        static constexpr auto k_version = 0_ui;

        enum struct ResourceInformationListStateFlag : uint8_t
        {
            next = 0x0,
            done = 0x80
        };

        struct ResourceInformation
        {
            String path;
            u32 size;
            u32 size_original;
            u64 time;

            constexpr explicit ResourceInformation() = default;

            ~ResourceInformation() = default;

            friend auto operator << (
                std::ostream& os,
                const ResourceInformation& other
            ) -> std::ostream& {
                os << "ResourceInformation(" << other.path << ", " << other.size << ", " << other.size_original << ", " << other.time << ")";
                return os;
            }
        };
    };
}