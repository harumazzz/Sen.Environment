#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/common.hpp"
#include "kernel/support/popcap/resource_stream_bundle/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{
    enum struct PacketCompressionEnumeration : u32 {
        none = 0_u32,
        general,
        texture,
        all,
    };

    using Dimension = Encoding::Image::Rectangle;

    struct DataMapPosition {
        u32 offset;
        u32 size;

        constexpr explicit DataMapPosition() = default;

        ~DataMapPosition() = default;

        friend auto operator << (
            std::ostream& os,
            const DataMapPosition& other
        ) -> std::ostream& {
            os << "DataMapPosition(" << other.offset << ", " << other.size << ")";
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

    using ItemResource = std::variant<GeneralResource, TextureResource>;

    struct PacketInformation {
        u32 version;
        u32 compression;
        List<ItemResource> resources;

        constexpr explicit PacketInformation() = default;

        ~PacketInformation() = default;

        friend auto operator << (
            std::ostream& os,
            const PacketInformation& other
        ) -> std::ostream& {
            os << "PacketInformation(" << other.version << ", " << other.compression << ", " << other.resources << ")";
            return os;
        }
    };
}

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamGroup::GeneralResource, path);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamGroup::TextureResource, path, dimension);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ResourceStreamGroup::PacketInformation, version, compression, resources);