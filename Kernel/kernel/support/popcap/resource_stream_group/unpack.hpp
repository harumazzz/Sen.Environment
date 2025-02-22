#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup {
    struct Unpack : Common {

    protected:

        static auto exchange_packet_resource(
            ReadMemoryStream &stream,
            Optional<ItemResource> &value,
            const std::string_view& path
        ) -> void {
            if (static_cast<bool>(stream.u32())) {
                value.emplace(std::in_place_type<TextureResource>);
                auto &texture = std::get<TextureResource>(value.operator*());
                texture.path = String{path.data(), path.size()};
                std::ranges::replace(texture.path, '\\', '/');
                exchange_number_fixed<u32>(stream, texture._mapping.offset);
                exchange_number_fixed<u32>(stream, texture._mapping.size);
                exchange_null_block(stream, sizeof(u32) * 3_size); // skip
                exchange_number_fixed<u32>(stream, texture.dimension.width);
                exchange_number_fixed<u32>(stream, texture.dimension.height);
            } else {
                value.emplace(std::in_place_type<GeneralResource>);
                auto &general = std::get<GeneralResource>(value.operator*());
                general.path = String{path.data(), path.size()};
                std::ranges::replace(general.path, '\\', '/');
                exchange_number_fixed<u32>(stream, general._mapping.offset);
                exchange_number_fixed<u32>(stream, general._mapping.size);
            }
        }

        static auto compiled_map_decode(
            ReadMemoryStream &stream,
            List<ItemResource> &value_list
        ) -> void {
            const auto resource_information_section_size = stream.u32();
            const auto resource_information_section_offset = stream.u32();
            const auto resource_information_end = resource_information_section_offset + resource_information_section_size;
            stream.set_position(resource_information_section_offset);
            auto parent_string = HashMap<usize, std::string>{};
            while (stream.current_position() < resource_information_end) {
                auto string_stream = std::ostringstream{};
                if (const auto position = (stream.current_position() - resource_information_section_offset) / k_block_size; parent_string.contains(position)) {
                    string_stream << parent_string[position];
                    parent_string.erase(position);
                }
                while (true) {
                    const auto composite_value = stream.u32();
                    const auto child_string_offset = (composite_value & 0xffffff00) >> 8;
                    const auto current_character = composite_value & 0xff;
                    if (child_string_offset != k_none_size) {
                        parent_string[child_string_offset] = string_stream.str();
                    }
                    if (current_character == k_none_size) {
                        break;
                    }
                    string_stream << static_cast<char>(current_character);
                }
                if (string_stream.view().empty()) {
                    break;
                }
                auto make_resource = [&]() -> Optional<ItemResource> {
                    auto resource = Optional<ItemResource>{};
                    exchange_packet_resource(stream, resource, string_stream.str());
                    return resource;
                };
                value_list.append(make_resource().operator*());
            }
        }

        template<auto general_compressed, auto texture_compressed> requires std::is_same_v<type_of<general_compressed>, bool> && std::is_same_v<type_of<texture_compressed>, bool>
        static auto decode_packet_data(
            ReadMemoryStream &stream,
            Uint8Array &general,
            Uint8Array &texture
        ) -> void {
            const auto general_resource_data_section_offset = stream.u32();
            const auto general_resource_data_section_size = stream.u32();
            if (const auto general_resource_data_section_size_original = stream.u32(); general_resource_data_section_size_original != k_none_size) {
                if constexpr (general_compressed) {
                    general.allocate(general_resource_data_section_size_original);
                    auto compressed_data = stream.subview(general_resource_data_section_offset, general_resource_data_section_size);
                    Compression::Zlib::Uncompress::process(compressed_data, general);
                } else {
                    auto uncompressed_data = stream.sub(general_resource_data_section_offset, general_resource_data_section_size_original);
                    general.assign(uncompressed_data);
                }
            }
            exchange_null_block(stream, sizeof(u32));
            const auto texture_resource_data_section_offset = stream.u32();
            const auto texture_resource_data_section_size = stream.u32();
            if (const auto texture_resource_data_section_size_original = stream.u32(); texture_resource_data_section_size_original != k_none_size) {
                if constexpr (texture_compressed) {
                    texture.allocate(texture_resource_data_section_size_original);
                    auto compressed_data = stream.subview(texture_resource_data_section_offset, texture_resource_data_section_size);
                    Compression::Zlib::Uncompress::process(compressed_data, texture);
                } else {
                    auto uncompressed_data = stream.sub(texture_resource_data_section_offset, texture_resource_data_section_size_original);
                    texture.assign(uncompressed_data);
                }
            }
        }

        template <typename ResourceClass> requires is_class<ResourceClass>
        static auto exchange_write(ResourceClass const &value, Uint8ArrayView const &general, Uint8ArrayView const &texture, StringView const &resource_directory) -> void {
            const auto file_destination = Path::join(resource_directory, value.path);
            if (!FileSystem::exist(Path::dirname(file_destination)))
            {
                FileSystem::create_directory(Path::dirname(file_destination));
            }
            const auto item_resource_offset = value._mapping.offset;
            const auto item_resource_size = value._mapping.size;
            if constexpr (std::is_same_v<ResourceClass, TextureResource>) {
                auto data_view = texture.subview(item_resource_offset, item_resource_size);
                FileSystem::write_file(file_destination, data_view);
            }
            else {
                auto view = general.subview(item_resource_offset, item_resource_size);
                FileSystem::write_file(file_destination, view);
            }
        }

    public:
        template<typename DestType> requires std::is_same_v<DestType, StringView> || std::is_same_v<DestType, Uint8Array>
        static auto process_package(ReadMemoryStream &stream, PacketInformation &model, DestType const &destination) -> void {
            exchange_null_block(stream, sizeof(u32) * 2_size);
            exchange_number_fixed<u32>(stream, model.compression);
            const auto information_section_size = stream.u32();
            auto general_view_store = Uint8Array{};
            auto texture_view_store = Uint8Array{};
            switch (static_cast<PacketCompressionEnumeration>(model.compression)) {
                case PacketCompressionEnumeration::none: {
                    decode_packet_data<false, false>(stream, general_view_store, texture_view_store);
                    break;
                }
                case PacketCompressionEnumeration::general: {
                    decode_packet_data<true, false>(stream, general_view_store, texture_view_store);
                    break;
                }
                case PacketCompressionEnumeration::texture: {
                    decode_packet_data<false, true>(stream, general_view_store, texture_view_store);
                    break;
                }
                case PacketCompressionEnumeration::all: {
                    decode_packet_data<true, true>(stream, general_view_store, texture_view_store);
                    break;
                }
            }
            exchange_null_block(stream, sizeof(u32) * 5_size);
            compiled_map_decode(stream, model.resources);
            if constexpr (std::is_same_v<DestType, StringView>) {
                auto pool = BS::thread_pool{};
                pool.detach_loop(k_begin_index, model.resources.size(),
                [&](const usize &i) {
                    if (auto const &resource = model.resources[i]; std::holds_alternative<TextureResource>(resource)) {
                        exchange_write(std::get<TextureResource>(resource), general_view_store.view(), texture_view_store.view(), destination);
                    }
                    else {
                        exchange_write(std::get<GeneralResource>(resource), general_view_store.view(), texture_view_store.view(), destination);
                    }
                });
                pool.wait();
            }
        }

        static auto process_whole(ReadMemoryStream &stream, PacketInformation &model, StringView const &destination) -> void {
            assert_conditional(stream.u32() == k_magic_identifier, fmt::format("{}", Language::get("popcap.rsg.unpack.invalid_rsg_magic")), "process_whole");
            exchange_number_fixed<u32>(stream, model.version);
            assert_conditional(contain_version(model.version), fmt::format("{}", Language::get("popcap.rsg.invalid_version")), "process_whole");
            const auto resource_directory = Path::join(destination, "resource"_s);
            process_package<StringView>(stream, model, resource_directory);
        }

        static auto process_fs(
            StringView const &source,
            StringView const &destination
        ) -> void {
            auto stream = ReadMemoryStream{source};
            auto model = PacketInformation{};
            process_whole(stream, model, destination);
            FileSystem::write_json(Path::join(destination, "data.json"_s), model);
        }
    };
}
