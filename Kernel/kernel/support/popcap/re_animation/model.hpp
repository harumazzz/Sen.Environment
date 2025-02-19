#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation {

    struct TransformDesktop
    {
        Optional<float> x;
        Optional<float> y;
        Optional<float> kx;
        Optional<float> ky;
        Optional<float> sx;
        Optional<float> sy;
        Optional<float> f;
        Optional<float> a;
        Optional<String> image;
        Optional<String> font;
        Optional<String> text;

        constexpr explicit TransformDesktop() = default;

        ~TransformDesktop() = default;
    };

    struct TransformMobile
    {
        Optional<float> x;
        Optional<float> y;
        Optional<float> kx;
        Optional<float> ky;
        Optional<float> sx;
        Optional<float> sy;
        Optional<float> f;
        Optional<float> a;
        Optional<i32> image;
        Optional<String> font;
        Optional<String> text;

        constexpr explicit TransformMobile() = default;

        ~TransformMobile() = default;
    };

    struct TransformTelevision
    {
        Optional<float> x;
        Optional<float> y;
        Optional<float> kx;
        Optional<float> ky;
        Optional<float> sx;
        Optional<float> sy;
        Optional<float> f;
        Optional<float> a;
        Optional<String> image;
        Optional<String> image_path;
        Optional<String> image_another;
        Optional<String> image_path_another;
        Optional<String> font;
        Optional<String> text;

        constexpr explicit TransformTelevision() = default;

        ~TransformTelevision() = default;
    };

    using Transform = std::variant<TransformDesktop, TransformMobile, TransformTelevision>;

    struct Track
    {
        String name;
        List<Transform> transform;

        constexpr explicit Track() = default;

        ~Track() = default;
    };

    struct ReanimInfo
    {
        float frame_rate;
        List<Track> track;

        constexpr explicit ReanimInfo() = default;

        ~ReanimInfo() = default;
    };
}

using namespace Sen::Kernel::Support::PopCap::ReAnimation;

JSONCONS_N_MEMBER_TRAITS(TransformDesktop, 0, x, y, kx, ky, sx, sy, f, a, image, font, text);

JSONCONS_N_MEMBER_TRAITS(TransformMobile, 0, x, y, kx, ky, sx, sy, f, a, image, font, text);

JSONCONS_N_MEMBER_TRAITS(TransformTelevision, 0, x, y, kx, ky, sx, sy, f, a, image, image_path, image_another, image_path_another, font, text);

JSONCONS_ALL_MEMBER_TRAITS(Track, name, transform);

JSONCONS_ALL_MEMBER_TRAITS(ReanimInfo, frame_rate, track);