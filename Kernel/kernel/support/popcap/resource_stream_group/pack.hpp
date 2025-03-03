#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup {
    struct Pack : Common {
    protected:
        /*
        static auto exchange_packet_resource(
            WriteMemoryStream &stream,
            ItemResource const &value) -> void {
            if (std::holds_alternative<TextureResource>(value)) {
                auto &texture_resource = std::get<TextureResource>(value);
                exchange_number_fixed<u32>(stream, static_cast<u32>(true));
                exchange_number_fixed<u32>(stream, texture_resource._mapping.offset);
                exchange_number_fixed<u32>(stream, texture_resource._mapping.size);
                exchange_number_fixed<u32>(stream, texture_resource.format);
                exchange_null_block(stream, sizeof(u32) * 2_size);
                exchange_number_fixed<u32>(stream, texture_resource.dimension.width);
                exchange_number_fixed<u32>(stream, texture_resource.dimension.height);
            } else if (std::holds_alternative<GeneralResource>(value)) {
                auto &general_resource = std::get<GeneralResource>(value);
                exchange_number_fixed<u32>(stream, static_cast<u32>(false));
                exchange_number_fixed<u32>(stream, general_resource._mapping.offset);
                exchange_number_fixed<u32>(stream, general_resource._mapping.size);
            }
        }

        template<is_class Class>
        static auto read_item(Class &resource, WriteMemoryStream &view_store, StringView const &resource_directory) -> void {
            auto view = Uint8Array{};
            const auto file_destination = Path::join(resource_directory, resource.path);
            FileSystem::read_file(file_destination, view);
            resource._mapping.offset = view_store.size();
            resource._mapping.size = view.size();
            view_store.bytes(view);
        }

        template<auto general_compressed, auto texture_compressed> requires std::is_same_v<type_of<general_compressed>, bool> && std::is_same_v<type_of<texture_compressed>, bool>
        static auto encode_packet_data(WriteMemoryStream &stream, WriteMemoryStream &general, WriteMemoryStream &texture, u32 const &offset, u32 const &size) -> void {
            const auto information_section_size = offset + size;
            exchange_number_fixed<u32>(stream, information_section_size);
            exchange_number_fixed<u32>(stream, information_section_size); // general position
            auto general_data = Uint8Array{};
            auto texture_data = Uint8Array{};
            if constexpr (general_compressed) {
                //Compression::Zlib::Compress::process(general.view(), general_data);
                exchange_number_fixed<u32>(stream, general_data.size());
                exchange_number_fixed<u32>(stream, general.size());
            } else {
                exchange_number_fixed<u32>(stream, general.size());
                exchange_number_fixed<u32>(stream, general.size());
                general_data.assign(general.view());
            }
            exchange_null_block(stream, sizeof(u32) * 1_size);
            exchange_number_fixed<u32>(stream, information_section_size + general_data.size());
            if constexpr (texture_compressed) {
                if (texture.size() != k_none_size)
                {
                   // Compression::Zlib::Compress::process(texture.view(), texture_data);
                }
                exchange_number_fixed<u32>(stream, texture_data.size());
                exchange_number_fixed<u32>(stream, texture.size());
            } else {
                exchange_number_fixed<u32>(stream, texture.size());
                exchange_number_fixed<u32>(stream, texture.size());
                texture_data.assign(texture.view());
            }
            stream += sizeof(u32) * 5_size;
            exchange_number_fixed<u32>(stream, size);
            exchange_number_fixed<u32>(stream, offset);
            stream.set_position(information_section_size);
            stream.bytes(general_data);
            stream.bytes(texture_data);
        }

        static auto process_package(WriteMemoryStream &stream, PacketInformation &model, StringView const &source) -> void {
           // debug("process_package");
            exchange_null_block(stream, sizeof(u32) * 2_size);
            assert_conditional(contain_compression(model.compression), fmt::format("{}", Language::get("popcap.rsg.invalid_rsg_compression")), "process_package");
            exchange_number_fixed<u32>(stream, model.compression);
            const auto current_position = stream.current_position();
            exchange_null_block(stream, sizeof(u32) * 18_size);
            auto general_view_store = WriteMemoryStream{6400000};
            auto texture_view_store = WriteMemoryStream{6400000};
            for (auto &resource: model.resource) {
                if (std::holds_alternative<TextureResource>(resource)) {
                    auto &texture_resource = std::get<TextureResource>(resource);
                    read_item(texture_resource, texture_view_store, source);
                } else if (std::holds_alternative<GeneralResource>(resource)) {
                    auto &general_resource = std::get<GeneralResource>(resource);
                    read_item(general_resource, general_view_store, source);
                }
            }
            const auto resource_information_section_offset = stream.current_position();
            compiled_map_encode(&exchange_packet_resource, stream, model.resource);
            const auto resource_information_section_size = stream.current_position() - resource_information_section_offset;
            stream.set_position(current_position);
            switch (static_cast<PacketCompressionEnumeration>(model.compression)) {
                case PacketCompressionEnumeration::none: {
                    encode_packet_data<false, false>(stream, general_view_store, texture_view_store, resource_information_section_offset, resource_information_section_size);
                    break;
                }
                case PacketCompressionEnumeration::general: {
                    encode_packet_data<true, false>(stream, general_view_store, texture_view_store, resource_information_section_offset, resource_information_section_size);
                    break;
                }
                case PacketCompressionEnumeration::texture: {
                    encode_packet_data<false, true>(stream, general_view_store, texture_view_store, resource_information_section_offset, resource_information_section_size);
                    break;
                }
                case PacketCompressionEnumeration::all: {
                    encode_packet_data<true, true>(stream, general_view_store, texture_view_store, resource_information_section_offset, resource_information_section_size);
                    break;
                }
            }
        }
        */
    public:
        static auto process_whole(WriteMemoryStream &stream, PacketInformation &model, StringView const &source) -> void {
            /*
            exchange_number_fixed<u32>(stream, k_magic_identifier);
            assert_conditional(contain_version(model.version), fmt::format("{}", Language::get("popcap.rsg.invalid_version")), "process_whole");
            exchange_number_fixed<u32>(stream, model.version);
            const auto resource_directory = Path::join(source, "resource"_s);
            process_package(stream, model, resource_directory);
            */
        }

        static auto process_fs(
            StringView const &source,
            StringView const &destination
        ) -> void {
            auto stream = WriteMemoryStream{6400000}; // allocate size
            auto model = PacketInformation{};
            FileSystem::read_json(Path::join(source, "data.json"_s), model);
            process_whole(stream, model, source);
            FileSystem::write_file(destination, stream.view());
        }
    };
}
