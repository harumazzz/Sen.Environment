#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_bundle/structure.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle
{
    struct Common
    {
        static constexpr auto k_magic_identifier = 0x72736231_ui;

        static constexpr auto version_list = {3_ui, 4_ui}; // unsupported rsg version 1.

        static constexpr auto k_suffix_of_composite_shell = "_CompositeShell"_sv;

        static constexpr auto k_suffix_of_automation_pool = "_AutoPool"_sv;

        static constexpr auto k_packet_extension =  ".rsg"_sv;

        static constexpr auto k_header_start_position = 0x0c_size;

        static constexpr auto k_string_block_size = 128_size;

        static constexpr auto contain_version(u32 const & version) -> bool
        {
            return std::ranges::find(version_list, version) != version_list.end();
        }

        static constexpr auto get_bpp(const u32 &format) -> u32 { //TODO
            switch (format) {
                case 0:
                case 30:
                case 147:
                case 148:
                case 150:
                default:
                    return 4;
            }
        }
        /*
        static auto exchange_header(ReadMemoryStream &stream, HeaderInformation &value) -> void {
            stream.set_position(k_header_start_position);
            exchange_number_fixed<u32>(stream, value.information_section_size);
            stream += sizeof(u32) * 6_size;
            exchange_number_fixed<u32>(stream, value.subgroup_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.subgroup_information_section_offset);
            exchange_number_fixed<u32>(stream, value.subgroup_information_section_block_size);
            exchange_number_fixed<u32>(stream, value.group_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.group_information_section_offset);
            exchange_number_fixed<u32>(stream, value.group_information_section_block_size);
            stream += sizeof(u32) * 5_size;
            exchange_number_fixed<u32>(stream, value.texture_resource_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.texture_resource_information_section_offset);
            exchange_number_fixed<u32>(stream, value.texture_resource_information_section_block_size);
            exchange_number_fixed<u32>(stream, value.group_manifest_information_section_offset);
            exchange_number_fixed<u32>(stream, value.resource_manifest_information_section_offset);
            exchange_number_fixed<u32>(stream, value.string_manifest_information_section_offset);
            exchange_number_fixed<u32>(stream, value.information_without_manifest_section_size);
        }

        static auto exchange_header(WriteMemoryStream &stream, HeaderInformation const &value) -> void {
            stream.set_position(k_header_start_position);
            exchange_number_fixed<u32>(stream, value.information_section_size);
            exchange_number_fixed<u32>(stream, value.resource_path_section_size);
            exchange_number_fixed<u32>(stream, value.resource_path_section_offset);
            stream += sizeof(u32) * 2_size;
            exchange_number_fixed<u32>(stream, value.subgroup_id_section_size);
            exchange_number_fixed<u32>(stream, value.subgroup_id_section_offset);
            exchange_number_fixed<u32>(stream, value.subgroup_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.subgroup_information_section_offset);
            exchange_number_fixed<u32>(stream, value.subgroup_information_section_block_size);
            exchange_number_fixed<u32>(stream, value.group_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.group_information_section_offset);
            exchange_number_fixed<u32>(stream, value.group_information_section_block_size);
            exchange_number_fixed<u32>(stream, value.group_id_section_size);
            exchange_number_fixed<u32>(stream, value.group_id_section_offset);
            exchange_number_fixed<u32>(stream, value.pool_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.pool_information_section_offset);
            exchange_number_fixed<u32>(stream, value.pool_information_section_block_size);
            exchange_number_fixed<u32>(stream, value.texture_resource_information_section_block_count);
            exchange_number_fixed<u32>(stream, value.texture_resource_information_section_offset);
            exchange_number_fixed<u32>(stream, value.texture_resource_information_section_block_size);
            exchange_number_fixed<u32>(stream, value.group_manifest_information_section_offset);
            exchange_number_fixed<u32>(stream, value.resource_manifest_information_section_offset);
            exchange_number_fixed<u32>(stream, value.string_manifest_information_section_offset);
            if (value.information_without_manifest_section_size != k_none_size) {
                exchange_number_fixed<u32>(stream, value.information_without_manifest_section_size);
            }
        }

        static auto exchange_subgroup_information_section(ReadMemoryStream &stream, BasicSubgroupInformation &value) -> void {
          //  exchange_string_block_size<k_string_block_size>(stream, value.id);
            exchange_number_fixed<u32>(stream, value.offset);
            exchange_number_fixed<u32>(stream, value.size);
            stream += sizeof(u32) * 15_size;
            exchange_number_fixed<u32>(stream, value.texture_resource_count);
            exchange_number_fixed<u32>(stream, value.texture_resource_begin);
        }

        static auto exchange_subgroup_information_section(WriteMemoryStream &stream, BasicSubgroupInformation const &value) -> void {
         //   exchange_string_block_size<k_string_block_size>(stream, value.id);
            exchange_number_fixed<u32>(stream, value.offset);
            exchange_number_fixed<u32>(stream, value.size);
            exchange_number_fixed<u32>(stream, value.pool);
            exchange_number_fixed<u32>(stream, value.resource_data_section_compression);
            exchange_number_fixed<u32>(stream, value.information_section_size);
            exchange_number_fixed<u32>(stream, value.general_resource_data_section_offset);
            exchange_number_fixed<u32>(stream, value.general_resource_data_section_size);
            exchange_number_fixed<u32>(stream, value.general_resource_data_section_size_original);
            exchange_number_fixed<u32>(stream, value.general_resource_data_section_size_pool);
            exchange_number_fixed<u32>(stream, value.texture_resource_data_section_offset);
            exchange_number_fixed<u32>(stream, value.texture_resource_data_section_size);
            exchange_number_fixed<u32>(stream, value.texture_resource_data_section_size_original);
            exchange_number_fixed<u32>(stream, value.texture_resource_data_section_size_pool);
            exchange_null_block(stream, sizeof(u32) * 4_size);
            exchange_number_fixed<u32>(stream, value.texture_resource_count);
            exchange_number_fixed<u32>(stream, value.texture_resource_begin);
        }

        */

        static auto exchange_basic_subgroup_list(WriteMemoryStream &stream, List<BasicSubgroupInformation> const &value_list, u32 const &information_section_size) -> void {
            for (auto &value : value_list) {
                value.offset += information_section_size;
               // exchange_subgroup_information_section(stream, value);
            }
        }

    };
}
