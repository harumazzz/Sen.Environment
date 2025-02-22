#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    enum struct VersionPlatform : uint8_t
    {
        desktop,
        mobile32,
        mobile64,
        television
    };

    using namespace Encoding::XML;

    struct Common
    {
        static constexpr auto k_desktop_magic_identifier = 0xB393B4C0_ui;

        static constexpr auto k_mobile32_magic_identifier = 0xFF2565B5_ui;

        static constexpr auto k_mobile64_magic_identifier = 0xC046E570_ui;

        static constexpr auto k_television_magic_identifier = 0x00000000_ui;

        static constexpr auto k_transform_float_end = -10000.0f;

        static constexpr auto k_image_null = -1;

        static constexpr auto k_x = "x"_sv;

        static constexpr auto k_y = "y"_sv;

        static constexpr auto k_kx = "kx"_sv;

        static constexpr auto k_ky = "ky"_sv;

        static constexpr auto k_sx = "sx"_sv;

        static constexpr auto k_sy = "sy"_sv;

        static constexpr auto k_f = "f"_sv;

        static constexpr auto k_a = "a"_sv;

        static constexpr auto k_i = "i"_sv;

        static constexpr auto k_resource = "resource"_sv;

        static constexpr auto k_i2 = "i2"_sv;

        static constexpr auto k_resource2 = "resource2"_sv;

        static constexpr auto k_font = "font"_sv;

        static constexpr auto k_text = "text"_sv;

        static constexpr auto k_t = "t"_sv;

        static constexpr auto k_track = "track"_sv;

        static constexpr auto k_name = "name"_sv;

        static constexpr auto k_fps = "fps"_sv;

        template <auto condition>
        using integer_or_platform = std::conditional_t<condition != VersionPlatform::mobile64, u32, u64>;

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static constexpr auto get_transform_data_size() -> size_t
        {
            auto size = k_none_size;
            size += sizeof(float) * 8;
            size += sizeof(integer_or_platform<platform>) * 3;
            if constexpr (platform == VersionPlatform::television)
            {
                size += sizeof(integer_or_platform<platform>);
            }
            return size;
        }

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static constexpr auto get_track_data_size() -> size_t
        {
            auto size = k_none_size;
            size += sizeof(integer_or_platform<platform>) * 3;
            if constexpr (platform == VersionPlatform::mobile32 || platform == VersionPlatform::mobile64)
            {
                size += sizeof(integer_or_platform<platform>);
            }
            if constexpr (platform == VersionPlatform::television)
            {
                size += sizeof(integer_or_platform<platform>);
            }
            return size;
        }
    };

}

template <>
struct Sen::Kernel::Javascript::Trait<Sen::Kernel::Support::PopCap::ReAnimation::VersionPlatform> {

    using VersionPlatform = Sen::Kernel::Support::PopCap::ReAnimation::VersionPlatform;

    inline static auto constexpr k_desktop = "desktop"_sv;

    inline static auto constexpr k_mobile32 = "mobile32"_sv;

    inline static auto constexpr k_mobile64 = "mobile64"_sv;

    inline static auto constexpr k_television = "television"_sv;

    inline static auto constexpr k_desktop_hash = hash_string(k_desktop);

    inline static auto constexpr k_mobile32_hash = hash_string(k_mobile32);

    inline static auto constexpr k_mobile64_hash = hash_string(k_mobile64);

    inline static auto constexpr k_television_hash = hash_string(k_television);

    static auto from_value(
        Value& source,
        VersionPlatform& destination
    ) -> void {
        assert_conditional(source.is_string(), "Expected the value to be number, but the actual type is not", "from_value");
        auto value = NativeString{};
        Trait<NativeString>::from_value(source, value);
        switch (hash_string(value.view())) {
            case k_desktop_hash:
                destination = VersionPlatform::desktop;
                break;
            case k_mobile32_hash:
                destination = VersionPlatform::mobile32;
                break;
            case k_mobile64_hash:
                destination = VersionPlatform::mobile64;
                break;
            case k_television_hash:
                destination = VersionPlatform::television;
                break;
            default:
                assert_conditional(false, "Invalid platform", "from_value");
        }
    }

};
