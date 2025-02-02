#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_bundle/definition.hpp"
#include "kernel/support/popcap/resource_stream_bundle/unpack.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamBundlePatch
{
    

    struct Common
    {
        inline static constexpr auto k_magic_identifier = uint32_t{0x52534250};

        inline static constexpr auto k_magic_version = uint32_t{1};

        inline static constexpr auto k_unknown_section_1 = uint32_t{2};

        inline static constexpr auto k_hash_section_size = size_t{16};

        inline static constexpr auto k_begin_index = size_t{0};

        inline static constexpr auto information_header_section_size = size_t{48};

        struct PackageInformation
        {
            uint32_t magic;
            uint32_t version;
            uint32_t unknown_1;
            uint32_t all_after_size;
            uint32_t patch_size;
            std::string before_hash;
            uint32_t packet_count;
            uint32_t patch_exist;
        };

        inline static auto bytes_to_hex_string(
            List<uint8_t> const &data) -> std::string
        {
            auto result = std::stringstream{};
            for (auto &v : data)
            {
                result
                    << std::setfill('0') << std::setw(sizeof(v) * 2)
                    << std::hex << +v;
            }
            return result.str();
        }

        inline static auto hex_string_to_bytes(const std::string &data) -> List<uint8_t>
        {
            auto result = List<uint8_t>{};
            result.reserve(static_cast<size_t>(data.length() / 2));
            for (auto i = 0; i < data.length(); i += 2)
            {
                auto byte_string = data.substr(i, 2);
                auto byte_value = static_cast<uint8_t>(std::stoi(byte_string, nullptr, 16));
                result.emplace_back(byte_value);
            }

            return result;
        }

        struct PacketInformation
        {
            uint32_t patch_exist;
            uint32_t patch_size;
            std::string name;
            std::string before_hash;
        };
    };
}