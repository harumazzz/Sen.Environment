#pragma once
#include "model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle {

    struct HeaderInformation { // start position: 0x0C
        u32 information_section_size;
        u32 resource_path_section_size;
        u32 resource_path_section_offset;
        u32 subgroup_id_section_size;
        u32 subgroup_id_section_offset;
        u32 subgroup_information_section_block_count;
        u32 subgroup_information_section_offset;
        u32 subgroup_information_section_block_size;
        u32 group_information_section_block_count;
        u32 group_information_section_offset;
        u32 group_information_section_block_size;
        u32 group_id_section_size;
        u32 group_id_section_offset;
        u32 pool_information_section_block_count;
        u32 pool_information_section_offset;
        u32 pool_information_section_block_size;
        u32 texture_resource_information_section_block_count;
        u32 texture_resource_information_section_offset;
        u32 texture_resource_information_section_block_size;
        u32 group_manifest_information_section_offset;
        u32 resource_manifest_information_section_offset;
        u32 string_manifest_information_section_offset;
        u32 information_without_manifest_section_size;
    };

    struct BasicSubgroupInformation {
        String id;
        u32 offset;
        u32 size;
        u32 pool;
        u32 resource_data_section_compression;
        u32 information_section_size;
        u32 general_resource_data_section_offset;
        u32 general_resource_data_section_size;
        u32 general_resource_data_section_size_original;
        u32 general_resource_data_section_size_pool;
        u32 texture_resource_data_section_offset;
        u32 texture_resource_data_section_size;
        u32 texture_resource_data_section_size_original;
        u32 texture_resource_data_section_size_pool;
        u32 texture_resource_count;
        u32 texture_resource_begin;
    };

    struct SimpleSubgroupInformation {
        u32 index;
        u32 resolution;
        u32 locale;
    };

    struct SimpleGroupInformation {
        String id;
        List<SimpleSubgroupInformation> subgroup;
    };

    struct PoolInformation
    {
        u32 texture_resource_data_section_offset;
        u32 texture_resource_data_section_size;
        u32 instance_count;
        u32 flag;
        u32 texture_resource_count;
        u32 texture_resource_begin;
    };

    struct TextureInformation
    {
        u32 size_width;
        u32 size_height;
        u32 pitch;
        u32 format;
        u32 alpha_size;
        u32 scale;
    };

    struct StructureInformation {
        HeaderInformation header_information;
        List<ItemResource> resource_id;
        List<ItemResource> subgroup_id;
        List<ItemResource> group_id;
        List<BasicSubgroupInformation> subgroup_information;
        List<SimpleGroupInformation> group_information;
        List<PoolInformation> pool_information;
        List<TextureInformation> texture_information;

        constexpr explicit StructureInformation() = default;

        ~StructureInformation() = default;
    };
}
