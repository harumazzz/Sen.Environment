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

        friend auto operator << (
            std::ostream& os,
            const TransformDesktop& other
        ) -> std::ostream& {
            os << "TransformDesktop(" << other.x << ", " << other.y << ", " << other.kx << ", " << other.ky << ", " <<
                other.sx << ", " << other.sy << ", " << other.f << ", " << other.a << ", " << other.image << ", " <<
                other.font << ", " << other.text << ")";
            return os;
        }
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

        friend auto operator << (
            std::ostream& os,
            const TransformMobile& other
        ) -> std::ostream& {
            os << "TransformMobile(" << other.x << ", " << other.y << ", " << other.kx << ", " << other.ky << ", " <<
                other.sx << ", " << other.sy << ", " << other.f << ", " << other.a << ", " << other.image << ", " <<
                other.font << ", " << other.text << ")";
            return os;
        }
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

        friend auto operator << (
            std::ostream& os,
            const TransformTelevision& other
        ) -> std::ostream& {
            os << "TransformTelevision(" << other.x << ", " << other.y << ", " << other.kx << ", " << other.ky << ", " <<
                other.sx << ", " << other.sy << ", " << other.f << ", " << other.a << ", " << other.image << ", " <<
                other.image_path << ", " << other.image_another << ", " << other.image_path_another << ", " <<
                other.font << ", " << other.text << ")";
            return os;
        }
    };

    using Transform = std::variant<TransformDesktop, TransformMobile, TransformTelevision>;

    struct Track
    {
        String name;
        List<Transform> transform;

        constexpr explicit Track() = default;

        ~Track() = default;

        friend auto operator << (
            std::ostream& os,
            const Track& other
        ) -> std::ostream& {
            os << "Track(" << other.name << ", ";
            for (const auto& transform : other.transform) {
                if (std::holds_alternative<TransformDesktop>(transform)) {
                    os << std::get<TransformDesktop>(transform);
                } else if (std::holds_alternative<TransformMobile>(transform)) {
                    os << std::get<TransformMobile>(transform);
                } else {
                    os << std::get<TransformTelevision>(transform);
                }
            }
            os << ")";
            return os;
        }
    };

    struct ReanimInfo
    {
        float frame_rate;
        List<Track> track;

        constexpr explicit ReanimInfo() = default;

        ~ReanimInfo() = default;

        friend auto operator << (
            std::ostream& os,
            const ReanimInfo& other
        ) -> std::ostream& {
            os << "ReanimInfo(" << other.frame_rate << ", " << other.track << ")";
            return os;
        }
    };
}

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ReAnimation::TransformDesktop, 0, x, y, kx, ky, sx, sy, f, a, image, font, text);

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ReAnimation::TransformMobile, 0, x, y, kx, ky, sx, sy, f, a, image, font, text);

JSONCONS_N_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ReAnimation::TransformTelevision, 0, x, y, kx, ky, sx, sy, f, a, image, image_path, image_another, image_path_another, font, text);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ReAnimation::Track, name, transform);

JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Support::PopCap::ReAnimation::ReanimInfo, frame_rate, track);