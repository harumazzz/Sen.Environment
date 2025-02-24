#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {

    enum struct ResourceTypeEnumeration : uint8_t {
        image = 0x01,
        pop_anim ,
        sound_bank,
        file,
        prime_font,
        render_effect,
        decoded_sound_bank
    };

    inline auto operator<<(
        std::ostream& os,
        const ResourceTypeEnumeration& type
    ) -> std::ostream& {
        switch (type) {
            case ResourceTypeEnumeration::image: {
                os << "Image";
                break;
            }
            case ResourceTypeEnumeration::pop_anim: {
                os << "PopAnim";
                break;
            }
            case ResourceTypeEnumeration::sound_bank: {
                os << "SoundBank";
                break;
            }
            case ResourceTypeEnumeration::file: {
                os << "File";
                break;
            }
            case ResourceTypeEnumeration::prime_font: {
                os << "PrimeFont";
                break;
            }
            case ResourceTypeEnumeration::render_effect: {
                os << "RenderEffect";
                break;
            }
            case ResourceTypeEnumeration::decoded_sound_bank: {
                os << "DecodedSoundBank";
                break;
            }
        }
        return os;
    }

    enum struct GroupType : uint8_t {
        composite = 0x01,
        simple
    };

    inline auto operator<<(
        std::ostream& os,
        const GroupType &type
    ) -> std::ostream& {
        switch (type) {
            case GroupType::composite: {
                os << "composite";
                break;
            }
            case GroupType::simple: {
                os << "simple";
                break;
            }
        }
        return os;
    }

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

        friend auto operator << (
            std::ostream& os,
            const TypeInfo& other
        ) -> std::ostream& {
            os << "TypeInfo(" << other.type << ", " << other.slot << ", " << other.x << ", " << other.y << ", "
                << other.ax << ", " << other.ay << ", " << other.aw << ", " << other.ah << ", " << other.cols <<
                ", " << other.rows << ", " << other.id << ", " << other.path << ", " << other.parent << ", "
            << other.forceOriginalVectorSymbolSize << ")";
            return os;
        }
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

        friend auto operator << (
            std::ostream& os,
            const AtlasInfo& other
        ) -> std::ostream& {
            os << "AtlasInfo(" << other.type << ", " << other.slot << ", " <<
                other.width << ", " << other.height << ", " << other.atlas << ", " <<
                    other.runtime << ", " << other.id << ", " << other.path << ")";
            return os;
        }
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

        friend auto operator << (
            std::ostream& os,
            const SimpleResource& other
        ) -> std::ostream& {
            os << "SimpleResource(" << other.type << ", " << other.res << ", " <<
                other.parent << ", ";
            for (const auto& resource : other.resources) {
                if (std::holds_alternative<AtlasInfo>(resource)) {
                    os << std::get<AtlasInfo>(resource);
                } else {
                    os << std::get<TypeInfo>(resource);
                }
            }
            os <<")";
            return os;
        }
    };

    struct CompositeSubgroup {
        Optional<String> res;
        String id;

        constexpr explicit CompositeSubgroup() = default;

        ~CompositeSubgroup() = default;

        friend auto operator << (
            std::ostream& os,
            const CompositeSubgroup& other
        ) -> std::ostream& {
            os << "CompositeSubgroup(" << other.res << ", " << other.id <<")";
            return os;
        }
    };

    struct CompositeGroup {
        GroupType type;
        String id;
        List<CompositeSubgroup> subgroups;

        constexpr explicit CompositeGroup() = default;

        ~CompositeGroup() = default;

        friend auto operator << (
            std::ostream& os,
            const CompositeGroup& other
        ) -> std::ostream& {
            os << "CompositeGroup(" << other.type << ", " << other.id << ", " << other.subgroups <<")";
            return os;
        }
    };

    using GroupInfo = std::variant<CompositeGroup, SimpleResource>;

	struct ResourcesModel {
        u32 version;
        u32 content_version;
        u32 slot_count;
        List<GroupInfo> groups;

	    constexpr explicit ResourcesModel() = default;

         ~ResourcesModel() = default;

	    friend auto operator << (
            std::ostream& os,
            const ResourcesModel& other
        ) -> std::ostream& {
	        os << "ResourcesModel(" << other.version << ", " << other.content_version << ", " << other.slot_count << ", ";
            for (const auto& group : other.groups) {
                if (std::holds_alternative<CompositeGroup>(group)) {
                    os << std::get<CompositeGroup>(group);
                } else {
                    os << std::get<SimpleResource>(group);
                }
            }
	        os <<")";
	        return os;
	    }
     };
}

JSONCONS_ENUM_NAME_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::GroupType, (simple, "simple"), (composite, "composite"));

JSONCONS_ENUM_NAME_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::ResourceTypeEnumeration, (image, "Image"), (pop_anim, "PopAnim"), (sound_bank, "SoundBank"), (file, "File"), (prime_font, "PrimeFont"), (render_effect, "RenderEffect"), (decoded_sound_bank, "DecodedSoundBank"));

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::TypeInfo, 2, type, slot, x, y, ax, ay, aw, ah, cols, rows, id, path, parent, forceOriginalVectorSymbolSize);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::AtlasInfo, type, slot, width, height, atlas, runtime, id, path);

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::SimpleResource, 1, type, res, id, parent, resources);

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::CompositeSubgroup, 0, res, id);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::CompositeGroup, type, id, subgroups);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::NewTypeObjectNotation::ResourcesModel, version, content_version, slot_count, groups);

