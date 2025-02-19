#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {

    enum ResourceTypeEnumeration : uint8_t {
        image = 0x01,
        pop_anim ,
        sound_bank,
        file,
        prime_font,
        render_effect,
        decoded_sound_bank
    };

    enum GroupType : uint8_t {
        composite = 0x01,
        simple
    };

    using ResType = ResourceTypeEnumeration;

    struct TypeInfo {
        ResType type;
        u32 slot;
        Optional<i32> x;
        Optional<i32> y;
        Optional<i32> ax;
        Optional<i32> ay;
        Optional<i32> aw;
        Optional<i32> ah;
        Optional<i32> cols;
        Optional<i32> rows;
        String id;
        String path;
        Optional<String> parent;
        Optional<bool> forceOriginalVectorSymbolSize;

        constexpr explicit TypeInfo() = default;

        ~TypeInfo() = default;
    };

    struct AtlasInfo {
        ResType type;
        u32 slot;
        u32 width;
        u32 height;
        bool atlas;
        bool runtime;
        String id;
        String path;

        constexpr explicit AtlasInfo() = default;

        ~AtlasInfo() = default;
    };

    using SimpleInfo = std::variant<AtlasInfo, TypeInfo>;

    struct SimpleResource {
        GroupType type;
        Optional<String> res;
        String id;
        Optional<String> parent;
        List<SimpleInfo> resources;

        constexpr explicit SimpleResource() = default;

        ~SimpleResource() = default;
    };

    struct CompositeSubgroup {
        Optional<String> res;
        String id;

        constexpr explicit CompositeSubgroup() = default;

        ~CompositeSubgroup() = default;
    };

    struct CompositeGroup {
        GroupType type;
        String id;
        List<CompositeSubgroup> subgroups;

        constexpr explicit CompositeGroup() = default;

        ~CompositeGroup() = default;
    };

    using GroupInfo = std::variant<CompositeGroup, SimpleResource>;

	struct ResourcesModel {
        u32 version;
        u32 content_version;
        u32 slot_count;
        List<GroupInfo> groups;

	    constexpr explicit ResourcesModel() = default;

         ~ResourcesModel() = default;
     };
}

using namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation;

JSONCONS_ENUM_NAME_TRAITS(GroupType, (simple, "simple"), (composite, "composite"));

JSONCONS_ENUM_NAME_TRAITS(ResourceTypeEnumeration, (image, "Image"), (pop_anim, "PopAnim"), (sound_bank, "SoundBank"), (file, "File"), (prime_font, "PrimeFont"), (render_effect, "RenderEffect"), (decoded_sound_bank, "DecodedSoundBank"));

JSONCONS_N_MEMBER_TRAITS(TypeInfo, 2, type, slot, x, y, ax, ay, aw, ah, cols, rows, id, path, parent, forceOriginalVectorSymbolSize);

JSONCONS_ALL_MEMBER_TRAITS(AtlasInfo, type, slot, width, height, atlas, runtime, id, path);

JSONCONS_N_MEMBER_TRAITS(SimpleResource, 1, type, res, id, parent, resources);

JSONCONS_N_MEMBER_TRAITS(CompositeSubgroup, 0, res, id);

JSONCONS_ALL_MEMBER_TRAITS(CompositeGroup, type, id, subgroups);

JSONCONS_ALL_MEMBER_TRAITS(ResourcesModel, version, content_version, slot_count, groups);

