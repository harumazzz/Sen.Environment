#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/definition.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{
    

    struct Common
    {
        inline static constexpr auto k_magic_identifier = 0x72736770_ui;

        inline static constexpr auto k_version_list = std::array<int, 3>{1, 3, 4};

        inline static constexpr auto k_compression_list = std::array<int, 4>{0, 1, 2, 3};

        inline static constexpr auto k_general_type_string = "general"_sv;

        inline static constexpr auto k_texture_type_string = "texture"_sv;

        inline static constexpr auto k_packet_compression_flag_count = 2_size;

        inline static constexpr auto information_header_section_size = 92_size;

        inline static constexpr auto information_resource_general_additional_section_size = 12_size;

        inline static constexpr auto information_resource_texture_additional_section_size = 32_size;

        inline static constexpr auto information_resource_texture_unknown_bytes_size = 8_size;

        inline static constexpr auto k_begin_index = 0_size;

        struct PacketCompressionFlag
        {
            inline static constexpr auto texture = 0_size;

            inline static constexpr auto general = 1_size;
        };

        struct PacketCompression
        {
            bool general;
            bool texture;
        };

        struct HeaderInformaiton
        {
            uint32_t magic;
            uint32_t version;
            uint32_t resource_data_section_compression;
            uint32_t information_section_size;
            uint32_t general_resource_data_section_offset;
            uint32_t general_resource_data_section_size;
            uint32_t general_resource_data_section_size_original;
            uint32_t texture_resource_data_section_offset;
            uint32_t texture_resource_data_section_size;
            uint32_t texture_resource_data_section_size_original;
            uint32_t resource_information_section_size;
            uint32_t resource_information_section_offset;
        };

        struct TextureInfo
        {
            int index;
            int width;
            int height;
        };

        struct ResourceInformation
        {
            uint32_t resource_data_section_size;
            uint32_t resource_data_section_offset;
            bool read_texture_additional;
            TextureInfo texture_value;
        };

        inline static auto packet_compression_to_data(
            uint32_t &data,
            PacketCompression const &value) -> void
        {
            auto data_bit = std::bitset<k_packet_compression_flag_count>{};
            data_bit.set(PacketCompressionFlag::general, value.general);
            data_bit.set(PacketCompressionFlag::texture, value.texture);
            data = static_cast<uint32_t>(data_bit.to_ullong());
            auto index = std::find(k_compression_list.begin(), k_compression_list.end(), static_cast<uint32_t>(data));
            assert_conditional((index != k_compression_list.end()), format(fmt::format("{}", Language::get("popcap.rsg.invalid_rsg_compression")), std::to_string(static_cast<uint32_t>(data))), "packet_compression_to_data");
            return;
        }

        inline static auto packet_compression_from_data(
            uint32_t const &data,
            PacketCompression &value) -> void
        {
            auto index = std::find(Common::k_compression_list.begin(), Common::k_compression_list.end(), static_cast<uint32_t>(data));
            assert_conditional((index != Common::k_compression_list.end()), format(fmt::format("{}", Language::get("popcap.rsg.invalid_rsg_compression")), std::to_string(static_cast<uint32_t>(data))), "packet_compression_from_data");
            auto data_bit = std::bitset<k_packet_compression_flag_count>(static_cast<uint8_t>(data));
            value.general = data_bit.test(PacketCompressionFlag::general);
            value.texture = data_bit.test(PacketCompressionFlag::texture);
            return;
        }
    };
}
