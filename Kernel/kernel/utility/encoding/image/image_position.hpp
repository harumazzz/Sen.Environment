#pragma once

#include "kernel/utility/encoding/image/basic_image.hpp"

namespace Sen::Kernel::Encoding::Image {

    struct ImagePosition : BasicImage {

        u32 x;

        u32 y;

        explicit ImagePosition(
            const u32& width,
            const u32& height,
            const u32& x,
            const u32& y
        ) : BasicImage{width, height}, x{x}, y{y} {}

        explicit ImagePosition(
            const u32& width,
            const u32& height
        ) : BasicImage{width, height}, x{0}, y{0} {

        }

        constexpr explicit ImagePosition() : BasicImage{}, x{0}, y{0} {

        }

        ImagePosition(
            const ImagePosition& image
        ) = delete;

        ImagePosition(
            ImagePosition&& other
        ) noexcept : BasicImage{}, x{other.x}, y{other.y} {
            thiz.m_color.take_ownership(other.m_color);
            thiz.width = other.width;
            thiz.height = other.height;
            other.width = 0;
            other.height = 0;
        }

        auto operator=(
            const ImagePosition& image
        ) -> ImagePosition& = delete;

        auto operator=(
            const ImagePosition&& image
        ) -> ImagePosition& = delete;

        ~ImagePosition(
        ) = default;

        constexpr auto collide(
            const ImagePosition &other
        ) const -> bool
        {
            return other.x < thiz.x + thiz.width && other.x + other.width > thiz.x && other.y < thiz.y + thiz.height && other.y + other.height > thiz.y;
        }

        constexpr auto contains(
            const ImagePosition &other
        ) const -> bool
        {
            return other.x >= thiz.x && other.y >= thiz.y && other.x + other.width <= thiz.x + thiz.width && other.y + other.height <= thiz.y + thiz.height;
        }
    };

}