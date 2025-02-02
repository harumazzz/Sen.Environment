#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundle
{

    

    struct Common
    {
        inline static constexpr auto k_magic_identifier = 0x72736231_ui;

        inline static constexpr auto k_version_list = std::array<int, 3>{1, 3, 4};

        inline static constexpr auto k_texture_resource_information_section_block_size_list = std::array<size_t, 3>{16, 20, 24};

        inline static constexpr auto k_texture_resource_information_section_block_size_version_0 = 16_size;

        inline static constexpr auto k_texture_resource_information_section_block_size_version_1 = 20_size;

        inline static constexpr auto k_texture_resource_information_section_block_size_version_2 = 24_size;

        inline static constexpr auto k_suffix_of_composite_shell = "_CompositeShell"_sv;

        inline static constexpr auto k_suffix_of_automation_pool = "_AutoPool"_sv;

        inline static constexpr auto information_header_section_size = 112_size;

        inline static constexpr auto resource_basic_detail_section_size = 28_size;

        inline static constexpr auto k_begin_index = 0_size;

        inline static constexpr auto k_none_size = 0_size;

        inline static constexpr auto k_subgroup_name_string_block_size = 128_size;

        inline static auto fourcc_from_integer(
            uint32_t const &data,
            std::string &locale) -> void
        {
            locale += static_cast<char>(clip_bit(data, 24_size, 8_size));
            locale += static_cast<char>(clip_bit(data, 16_size, 8_size));
            locale += static_cast<char>(clip_bit(data, 8_size, 8_size));
            locale += static_cast<char>(clip_bit(data, 0_size, 8_size));
            assert_conditional(locale.size() == 4_size, fmt::format("{}", Language::get("popcap.rsb.invalid_locale_size")), "fourcc_from_integer");
            return;
        }

        inline static auto fourcc_to_integer(
            std::string const &locale,
            uint32_t &data) -> void
        {
            assert_conditional(locale.size() == 4_size, fmt::format("{}", Language::get("popcap.rsb.invalid_locale_size")), "fourcc_to_integer");
            data |= static_cast<uint32_t>(locale[0] << 0);
            data |= static_cast<uint32_t>(locale[1] << 8);
            data |= static_cast<uint32_t>(locale[2] << 16);
            data |= static_cast<uint32_t>(locale[3] << 24);
            return;
        }

        enum TextureInformationVersion {
            SectionVersion0,
            SectionVersion1,
            SectionVersion2
        };

        inline static auto exchange_texture_information_version(
            size_t const &texture_information_section_size
        ) -> TextureInformationVersion
        {
            switch(texture_information_section_size) {
                case k_texture_resource_information_section_block_size_version_0: 
                {
                    return TextureInformationVersion::SectionVersion0;
                }
                case k_texture_resource_information_section_block_size_version_1:
                {
                    return TextureInformationVersion::SectionVersion1;
                }
                case k_texture_resource_information_section_block_size_version_2: {
                    return TextureInformationVersion::SectionVersion2;
                }
                default: {
                    assert_conditional(false, format(fmt::format("{}", Language::get("popcap.rsb.invalid_texture_information_section_size")), std::to_string(texture_information_section_size)), "exchange_texture_information_version");
                }
            }
        }

        inline static auto exchange_texture_information_version(
            TextureInformationVersion const &texture_information_version
        ) -> size_t 
        {
            switch(texture_information_version) {
                case TextureInformationVersion::SectionVersion0: 
                {
                    return k_texture_resource_information_section_block_size_version_0;
                }
                case TextureInformationVersion::SectionVersion1:
                {
                    return k_texture_resource_information_section_block_size_version_1;
                }
                case TextureInformationVersion::SectionVersion2: {
                    return k_texture_resource_information_section_block_size_version_2;
                }
                default: {
                    assert_conditional(false, format(fmt::format("{}", Language::get("popcap.rsb.popcap.rsb.texture_information_version")), std::to_string(static_cast<int>(texture_information_version))), "exchange_texture_information_version");
                }
            }
        }

        struct HeaderInformaiton
        {
            uint32_t magic;
            uint32_t version;
            uint32_t unknown_1;
            uint32_t information_section_size;
            uint32_t resource_path_section_size;
            uint32_t resource_path_section_offset;
            uint32_t subgroup_id_section_size;
            uint32_t subgroup_id_section_offset;
            uint32_t subgroup_information_section_block_count;
            uint32_t subgroup_information_section_offset;
            uint32_t subgroup_information_section_block_size;
            uint32_t group_information_section_block_count;
            uint32_t group_information_section_offset;
            uint32_t group_information_section_block_size;
            uint32_t group_id_section_size;
            uint32_t group_id_section_offset;
            uint32_t pool_information_section_block_count;
            uint32_t pool_information_section_offset;
            uint32_t pool_information_section_block_size;
            uint32_t texture_resource_information_section_block_count;
            uint32_t texture_resource_information_section_offset;
            uint32_t texture_resource_information_section_block_size;
            uint32_t group_manifest_information_section_offset;
            uint32_t resource_manifest_information_section_offset;
            uint32_t string_manifest_information_section_offset;
            uint32_t information_without_manifest_section_size;
        };
    };
}
