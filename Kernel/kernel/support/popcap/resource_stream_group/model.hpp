#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{
    enum struct PacketCompressionEnumeration : u32 {
        none = 0_u32,
        general,
        texture,
        all,
    };

    using Dimension = Encoding::Image::Rectangle;

    struct WorkOption {
        size_t inherit_length;
        size_t parent_offset;
        bool has;

        friend auto operator << (
            std::ostream& os,
            const WorkOption& other
        ) -> std::ostream& {
            os << "WorkOption(" << other.inherit_length << ", " << other.parent_offset <<  ", " << other.has << ")";
            return os;
        }
    };

    struct DataMapPosition {
        u32 index;
        u32 offset;
        u32 size;

        constexpr explicit DataMapPosition() = default;

        ~DataMapPosition() = default;

        friend auto operator << (
            std::ostream& os,
            const DataMapPosition& other
        ) -> std::ostream& {
            os << "DataMapPosition("  << other.index << other.offset << ", " << other.size << ")";
            return os;
        }

    };

    struct GeneralResource {
        String path;
        DataMapPosition _mapping;

        constexpr explicit GeneralResource() = default;

        ~GeneralResource() = default;

        friend auto operator << (
            std::ostream& os,
            const GeneralResource& other
        ) -> std::ostream& {
            os << "GeneralResource(" << other.path << ", " << other._mapping << ")";
            return os;
        }
    };


    struct TextureResource {
        String path;
        Optional<u32> format;
        Dimension dimension;
        DataMapPosition _mapping;

        constexpr explicit TextureResource() = default;

        ~TextureResource() = default;

        friend auto operator << (
            std::ostream& os,
            const TextureResource& other
        ) -> std::ostream& {
            os << "TextureResource(" << other.path << ", " << other.dimension << ", " << other._mapping << ")";
            return os;
        }

    };

    using ItemResource = std::variant<TextureResource, GeneralResource>;

    struct PacketInformation {
        u32 version;
        u32 compression;
        List<ItemResource> resource;

        constexpr explicit PacketInformation() = default;

        ~PacketInformation() = default;

        friend auto operator << (
            std::ostream& os,
            const PacketInformation& other
        ) -> std::ostream& {
            os << "PacketInformation(" << other.version << ", " << other.compression << ", ";
            for (const auto& resource : other.resource) {
                if (std::holds_alternative<GeneralResource>(resource)) {
                    os << std::get<GeneralResource>(resource);
                } else {
                    os << std::get<TextureResource>(resource);
                }
            }
            os << ")";
            return os;
        }
    };
}

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamGroup::GeneralResource, path);

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamGroup::TextureResource, 2, path, dimension, format);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamGroup::PacketInformation, version, compression, resource);