#pragma once

#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/encoding/image/image_position.hpp"

namespace Sen::Kernel::Image::MaxRects {

    using Encoding::Image::ImagePosition;

    struct ExtendedImage : ImagePosition {

        bool rect_used{false};

        String source{};

        explicit ExtendedImage(
        ) = default;

        explicit ExtendedImage(
                const u32& width,
                const u32& height,
                const u32& x,
                const u32& y,
                const String &source
        ) : ImagePosition{width, height, x, y}, rect_used{true}, source{source}
        {
        }

        explicit ExtendedImage(
                const u32& width,
                const u32& height,
                const u32& x,
                const u32& y
        ) : ImagePosition{width, height, x, y}, rect_used{true}
        {
        }

        explicit ExtendedImage(
                const u32& width,
                const u32& height,
                const String &source
        ) : ImagePosition{width, height}, rect_used{true}, source{source}
        {
        }

        explicit ExtendedImage(
                const u32& width,
                const u32& height
        ) : ImagePosition{width, height}, rect_used{true}, source{}
        {
        }

        ExtendedImage(
                ExtendedImage&& other
        ) noexcept : rect_used{other.rect_used}, source{as_move(other.source)} {
                thiz.width = other.width;
                thiz.height = other.height;
                thiz.m_color.take_ownership(other.m_color);
                other.width = 0;
                other.height = 0;
        }

        auto operator =(
                ExtendedImage&& other
        ) noexcept -> ExtendedImage& {
                thiz.width = other.width;
                thiz.height = other.height;
                thiz.x = other.x;
                thiz.y = other.y;
                thiz.rect_used = other.rect_used;
                thiz.source.take_ownership(other.source);
                thiz.m_color.take_ownership(other.m_color);
                other.width = 0;
                other.height = 0;
                other.x = 0;
                other.y = 0;
                other.rect_used = false;
                return thiz;
        }

        ~ExtendedImage(
        ) = default;

    };


}
