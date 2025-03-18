#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle {

    using TextureResource = ResourceStreamGroup::TextureResource;

    using GeneralResource = ResourceStreamGroup::GeneralResource;

    using ItemResource = ResourceStreamGroup::ItemResource;

    struct CategoryInformation {
        Optional<u32> resolution;
        Optional<String> locale;

        constexpr explicit CategoryInformation( ) = default;

        ~CategoryInformation( ) = default;
    };

    struct SubgroupInformation {
        CategoryInformation category;
        //u32 compression; // unnecessary
        List<ItemResource> resource;

        constexpr explicit SubgroupInformation() = default;

        ~SubgroupInformation() = default;
    };

    struct GroupInformation {
        bool composite;
        Map<String, SubgroupInformation> subgroup;

        explicit GroupInformation( ) = default;

        ~GroupInformation( ) = default;
    };

    struct BundleStructure {
        u32 version;
        u32 texture_extend_version;
        Map<String, GroupInformation> group;

        explicit BundleStructure( ) = default;

        ~BundleStructure( ) = default;
    };
}

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamBundle::CategoryInformation, resolution, locale);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamBundle::SubgroupInformation, category, resource);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamBundle::GroupInformation, composite, subgroup);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamBundle::BundleStructure, version, texture_extend_version, group);