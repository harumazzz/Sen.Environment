#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"
#include "kernel/support/popcap/resource_stream_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup {
    struct Unpack : Common {

    protected:

        template<auto general_compressed, auto texture_compressed> requires std::is_same_v<type_of<general_compressed>, bool> && std::is_same_v<type_of<texture_compressed>, bool>
        static auto decode_packet_data(
            Uint32ReadFileStream &stream,
            Uint8Array &general,
            Uint8Array &texture
        ) -> void {
            const auto general_resource_data_section_offset = stream.numeric();
            const auto general_resource_data_section_size = stream.numeric();
            debug(general_resource_data_section_offset);
            debug(general_resource_data_section_size);
            debug("position");
            debug(stream.current_position());
            if (const auto general_resource_data_section_size_original = stream.numeric(); general_resource_data_section_size_original != k_none_size) {
                if constexpr (general_compressed) {

                    general.allocate(general_resource_data_section_size_original);
                    auto compressed_data = stream.bytes<u8>(general_resource_data_section_offset, general_resource_data_section_size);
                    Compression::Zlib::Uncompress::process(compressed_data, general);
                } else {
                    auto uncompressed_data = stream.bytes<u8>(general_resource_data_section_offset, general_resource_data_section_size_original);
                    general.assign(uncompressed_data);
                }
            }
            exchange_null_block(stream, 1_size);
            const auto texture_resource_data_section_offset = stream.numeric();
            const auto texture_resource_data_section_size = stream.numeric();
            if (const auto texture_resource_data_section_size_original = stream.numeric(); texture_resource_data_section_size_original != k_none_size) {
                if constexpr (texture_compressed) {
                    texture.allocate(texture_resource_data_section_size_original);
                    auto compressed_data = stream.bytes<u8>(texture_resource_data_section_offset, texture_resource_data_section_size);
                    Compression::Zlib::Uncompress::process(compressed_data, texture);
                } else {
                    auto uncompressed_data = stream.bytes<u8>(texture_resource_data_section_offset, texture_resource_data_section_size_original);
                    texture.assign(uncompressed_data);
                }
            }
        }

        template <typename ResourceClass> requires is_class<ResourceClass>
        static auto exchange_write(ResourceClass const &value, Uint8ArrayView const &general, Uint8ArrayView const &texture, StringView const &resource_directory) -> void {
            const auto file_destination = Path::join(resource_directory, value.path);
            FileSystem::make_directory(Path::dirname(file_destination));
            const auto item_resource_offset = value._mapping.offset;
            const auto item_resource_size = value._mapping.size;
            if constexpr (std::is_same_v<ResourceClass, TextureResource>) {
                auto data_view = texture.subview(item_resource_offset, item_resource_size);
                FileSystem::write_file(file_destination, data_view);
            }
            else {
                auto data_view = general.subview(item_resource_offset, item_resource_size);
                FileSystem::write_file(file_destination, data_view);
            }
        }


        template<typename DestType> requires std::is_same_v<DestType, StringView> || std::is_same_v<DestType, Uint8Array>
        static auto process_package(Uint32ReadFileStream &stream, PacketInformation &model, DestType const &destination) -> void {
            exchange_null_block(stream, 2_size);
            exchange_numeric(stream, model.compression);
            assert_conditional(contain_compression(model.compression), fmt::format("{}", Language::get("popcap.rsg.invalid_rsg_compression")), "process_package");
            const auto information_section_size = stream.numeric();
            auto general_view_store = Uint8Array{};
            auto texture_view_store = Uint8Array{};
            debug("compression");
            debug(model.compression);
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
            exchange_null_block(stream, 5_size);
            compiled_map_decode(&exchange_packet_resource, stream, model.resource);
            if constexpr (std::is_same_v<DestType, StringView>) {
                auto pool = BS::thread_pool{};
                pool.detach_loop(k_begin_index, model.resource.size(),
                [&](const usize &i) {
                    if (auto const &resource = model.resource[i]; std::holds_alternative<TextureResource>(resource)) {
                        exchange_write(std::get<TextureResource>(resource), general_view_store.view(), texture_view_store.view(), destination);
                    }
                    else {
                        exchange_write(std::get<GeneralResource>(resource), general_view_store.view(), texture_view_store.view(), destination);
                    }
                });
                pool.wait();
            }
        }

    public:
        template <is_u32_read_stream ReadStream>
        static auto process_whole(Uint32ReadFileStream &stream, PacketInformation &model, StringView const &destination) -> void {
            assert_conditional(stream.numeric() == k_magic_identifier, fmt::format("{}", Language::get("popcap.rsg.unpack.invalid_rsg_magic")), "process_whole");
            exchange_numeric(stream, model.version);
            assert_conditional(contain_version(model.version), fmt::format("{}", Language::get("popcap.rsg.invalid_version")), "process_whole");
            FileSystem::make_directory(destination);
            const auto resource_directory = Path::join(destination, "resource"_s);
            process_package<StringView>(stream, model, resource_directory);
        }

        static auto process_fs(
            StringView const &source,
            StringView const &destination
        ) -> void {

        }
    };
}
