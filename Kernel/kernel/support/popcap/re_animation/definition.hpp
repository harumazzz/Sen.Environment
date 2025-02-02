#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    inline static constexpr auto transform_tfloat_end = -10000.0f;

    enum class ReanimPlatform : uint8_t
    {
        PC_Compile,
        TV_Compile,
        Phone32_Compile,
        Phone64_Compile,
        GameConsole_Compile,
        WP_XNB, // xnb extension; doesn't support 
    };

    struct ReanimTransform
    {
    public:
        float x = transform_tfloat_end;
        float y = transform_tfloat_end;
        float kx = transform_tfloat_end;
        float ky = transform_tfloat_end;
        float sx = transform_tfloat_end;
        float sy = transform_tfloat_end;
        float f = transform_tfloat_end;
        float a = transform_tfloat_end;
        std::string i;
        std::string resource;
        std::string i2;
        std::string resource2;
        std::string font;
        std::string text;
        explicit ReanimTransform(

            ) = default;

        explicit ReanimTransform(
            float x,
            float y,
            float kx,
            float ky,
            float sx,
            float sy,
            float f,
            float a) : x(x), y(y), kx(kx), ky(ky), sx(sx), sy(sy), f(f), a(a)
        {
        }

        ~ReanimTransform(

            ) = default;
    };

    struct ReanimTrack
    {
        std::string name;
        List<ReanimTransform> transforms;
    };

    struct ReanimInfo
    {
        int8_t do_scale;
        float fps;
        List<ReanimTrack> tracks;
    };
}