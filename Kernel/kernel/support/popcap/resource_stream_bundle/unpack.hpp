#pragma once


#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_bundle/structure.hpp"
#include "kernel/support/popcap/resource_stream_bundle/model.hpp"
#include "kernel/support/popcap/resource_stream_bundle/common.hpp"
#include "kernel/support/popcap/resource_stream_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle {
    struct Unpack : Common {
    private:
        /*
        static auto exchange_packet_resource(
            ReadMemoryStream &stream,
            Optional<ItemResource> &value,
            const std::string_view &path
        ) -> void {
            if (static_cast<bool>(stream.u32())) {
                value.emplace(std::in_place_type<TextureResource>);
                auto &texture = std::get<TextureResource>(value.operator*());
                texture.path = String{path.data(), path.size()};
                exchange_null_block(stream, sizeof(u32) * 2_size); // skip
                exchange_number_fixed<u32>(stream, texture.format);
                exchange_null_block(stream, sizeof(u32) * 2_size); // skip
                exchange_number_fixed<u32>(stream, texture.dimension.width);
                exchange_number_fixed<u32>(stream, texture.dimension.height);
            } else {
                value.emplace(std::in_place_type<GeneralResource>);
                auto &general = std::get<GeneralResource>(value.operator*());
                general.path = String{path.data(), path.size()};
                exchange_null_block(stream, sizeof(u32) * 2_size); // skip
            }
        }

        static auto exchange_simple_subgroup(ReadMemoryStream &stream, SimpleSubgroupInformation &model) -> void {
            exchange_number_fixed<u32>(stream, model.index);
            exchange_number_fixed<u32>(stream, model.resolution);
            exchange_number_fixed<u32>(stream, model.locale);
            exchange_null_block(stream, sizeof(u32));
        }

        static auto exchange_simple_group(ReadMemoryStream &stream, SimpleGroupInformation &model) -> void {
            const auto current_pos = stream.current_position();
           // exchange_string_block_size<k_string_block_size>(stream, model.id);
            stream.set_position(current_pos + 1152_size);
            exchange_list_size<u32>(stream, model.subgroup);
            stream.set_position(current_pos + k_string_block_size);
            exchange_list<void>(&exchange_simple_subgroup, stream, model.subgroup);
            stream.set_position(current_pos + 1156_size);
        }

        static auto exchange_texture_extend_version(const u32 &data, auto &value) -> void {
            value = data / sizeof(u32) - sizeof(u32);
        }

        static auto exchange_subgroup_information_section(ReadMemoryStream &stream, BasicSubgroupInformation &value) -> void {
            //exchange_string_block_size<k_string_block_size>(stream, value.id);
            exchange_number_fixed<u32>(stream, value.offset);
            exchange_number_fixed<u32>(stream, value.size);
            stream += sizeof(u32) * 15_size;
            exchange_number_fixed<u32>(stream, value.texture_resource_count);
            exchange_number_fixed<u32>(stream, value.texture_resource_begin);
        }

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

        static auto process_package(ReadMemoryStream &stream, BundleStructure &model, StringView const &destination) -> void {
            auto structure_information = StructureInformation{};
            exchange_header(stream, structure_information.header_information);
            exchange_texture_extend_version(structure_information.header_information.texture_resource_information_section_block_size, model.texture_extend_version);
            stream.set_position(structure_information.header_information.group_information_section_offset);
            structure_information.group_information.resize(structure_information.header_information.group_information_section_block_count);
            exchange_list<void>(&exchange_simple_group, stream, structure_information.group_information);
            FileSystem::make_directory(destination);
            for (const auto &[id, subgroup]: structure_information.group_information) {
                auto group_bundle_information = GroupInformation{};
                group_bundle_information.composite = id.ends_with(k_suffix_of_composite_shell);
                auto group_id = group_bundle_information.composite ? id.sub(k_begin_index, id.size() - k_suffix_of_composite_shell.size()) : id;
                for (const auto &[index, resolution, locale]: subgroup) {
                    auto subgroup_bundle_information = SubgroupInformation{};
                    const auto start_position = index * structure_information.header_information.subgroup_information_section_block_size + structure_information.header_information.subgroup_information_section_offset;
                    stream.set_position(start_position);
                    auto basic_subgroup_information = BasicSubgroupInformation{
                        .id = String{}
                    };
                    exchange_subgroup_information_section(stream, basic_subgroup_information);
                    if (resolution != 0_ui) {
                        subgroup_bundle_information.category.resolution = resolution;
                    }
                    if (locale != 0_ui) {
                        subgroup_bundle_information.category.locale = String{};
                        fourcc_from_integer(locale, *subgroup_bundle_information.category.locale);
                    }
                    constexpr auto resource_information_section_start_position = 0x48_size;
                    stream.set_position(basic_subgroup_information.offset + resource_information_section_start_position);
                   // ResourceStreamGroup::Common::compiled_map_decode(&exchange_packet_resource, stream, subgroup_bundle_information.resource);
                    if (basic_subgroup_information.texture_resource_count != k_none_size) {
                        auto start_texture_section_offset = basic_subgroup_information.texture_resource_begin * structure_information.header_information.texture_resource_information_section_block_size + structure_information.header_information.texture_resource_information_section_offset;
                        for (auto &resource: subgroup_bundle_information.resource) {
                            if (std::holds_alternative<TextureResource>(resource)) {
                                auto &texture_resource = std::get<TextureResource>(resource);
                                start_texture_section_offset += (*texture_resource.format * structure_information.header_information.texture_resource_information_section_block_size) + sizeof(u32) * 3_size;
                                stream.set_position(start_texture_section_offset);
                                exchange_number_fixed<u32>(stream, *texture_resource.format);
                            }
                        }
                    }
                    group_bundle_information.subgroup.emplace(basic_subgroup_information.id, subgroup_bundle_information);
                    structure_information.subgroup_information.append(basic_subgroup_information);
                }
                model.group.emplace(group_id, group_bundle_information);
            }
            auto pool = BS::thread_pool{};
            pool.detach_sequence(k_begin_index, structure_information.subgroup_information.size(),
                                 [&](const usize &i) {
                                     auto const &basic_subgroup_information = structure_information.subgroup_information[i];
                                     const auto file_destination = Path::join(destination, basic_subgroup_information.id) + k_packet_extension;
                                     const auto data_view = stream.subview(basic_subgroup_information.offset, basic_subgroup_information.size);
                                     FileSystem::write_file(file_destination, data_view);
                                 });
            pool.wait();
        }
        */

    public:
        static auto process_whole(ReadMemoryStream &stream, BundleStructure &model, StringView const &destination) -> void {
            assert_conditional(stream.u32() == k_magic_identifier, fmt::format("{}", Language::get("popcap.rsg.unpack.invalid_rsb_magic")), "process_whole");
           // exchange_number_fixed<u32>(stream, model.version);
            assert_conditional(contain_version(model.version), fmt::format("{}", Language::get("popcap.rsg.invalid_rsb_version")), "process_whole");
            FileSystem::make_directory(destination);
            const auto resource_directory = Path::join(destination, "resource"_s);
           // process_package(stream, model, resource_directory);
        }

        static auto process_fs(
            StringView const &source,
            StringView const &destination) -> void {
            auto stream = ReadMemoryStream{source};
            auto model = BundleStructure{};
            process_whole(stream, model, destination);
            FileSystem::write_json(Path::join(destination, "data.json"_s), model);
        }
    };
}
