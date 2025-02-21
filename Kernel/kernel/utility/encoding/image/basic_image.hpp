#pragma once

#include "kernel/utility/encoding/image/color.hpp"
#include "kernel/utility/encoding/image/rectangle.hpp"

namespace Sen::Kernel::Encoding::Image
{
    struct BasicImage;

    using CImage = BasicImage;

    using ImageView = CImage;

    struct BasicImage : Rectangle
    {
    protected:
        using Rectangle = Rectangle;

        using Color = Color;

        using Pixel = Color;

        template <typename T>
        using Array = CArray<T>;

    public:
        using usize = usize;

    protected:
        Array<Array<Color>> m_color{};

    public:
        constexpr explicit BasicImage(

        ) = default;

        explicit BasicImage(
            const u32& width,
            const u32& height
        ) : Rectangle{width, height}, m_color{}
        {
            thiz.allocate(width, height);
        }

        BasicImage(
            const BasicImage& image
        ) = delete;

        BasicImage(
            BasicImage&& other
        ) noexcept : Rectangle{other.width, other.height}
        {
            thiz.m_color.take_ownership(other.m_color);
            other.width = 0;
            other.height = 0;
        }

        auto operator=(
            const BasicImage& image
        ) -> BasicImage& = delete;

        auto operator=(
            const BasicImage&& image
        ) -> BasicImage& = delete;

        ~BasicImage(
        ) = default;

        auto allocate(
            const u32& width,
            const u32& height
        ) -> void
        {
            thiz.m_color.allocate(height);
            for (auto& row : thiz.m_color)
            {
                row.allocate(width);
            }
            thiz.width = width;
            thiz.height = height;
        }

        auto reallocate(
            const u32& width,
            const u32& height
        ) -> void
        {
            thiz.m_color.clear();
            thiz.allocate(width, height);
        }

        auto operator+(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator-(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator*(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator/(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator&(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator|(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator^(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator~(
        ) const -> BasicImage = delete;

        auto operator<<(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator>>(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator<<=(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator>>=(
            const BasicImage& other
        ) const -> BasicImage = delete;

        auto operator [](
            const usize& index
        ) -> Array<Color>&
        {
            return thiz.m_color[index];
        }

        constexpr auto data(
        ) -> Array<Array<Color>>&
        {
            return thiz.m_color;
        }

        constexpr auto color_size(
        ) const -> usize
        {
            return thiz.area() * 4;
        }

        constexpr auto new_rectangle (

        ) -> Rectangle {
            return Rectangle{thiz.width, thiz.height};
        }
    };
}
