#pragma once

#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Encoding::Image {

    template <typename T> requires std::is_integral_v<T>
    struct BasicRectangle;

    using Rectangle = BasicRectangle<u32>;

    template <typename T> requires std::is_integral_v<T>
    struct BasicRectangle {

        T width;

        T height;

        constexpr explicit BasicRectangle(
            const T& width,
            const T& height
        ) : width{width}, height{height} {

        }

        constexpr explicit BasicRectangle(
        ) : width{0}, height{0} {

        }

        constexpr BasicRectangle(
            const BasicRectangle& other
        ) = default;

        constexpr BasicRectangle(
            BasicRectangle&& other
        ) noexcept : width{other.width}, height{other.height} {

        }

        constexpr ~BasicRectangle(
        ) = default;

        auto operator=(
            const BasicRectangle& other
        ) -> BasicRectangle& {
            thiz.width = other.width;
            thiz.height = other.height;
            return thiz;
        }

        auto operator=(
            BasicRectangle&& other
        ) noexcept -> BasicRectangle& {
            thiz.width = other.width;
            thiz.height = other.height;
            return thiz;
        }

        constexpr auto operator+(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width + other.width, height + other.height};
        }

        constexpr auto operator-(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width - other.width, height - other.height};
        }

        constexpr auto operator*(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width * other.width, height * other.height};
        }

        constexpr auto operator/(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width / other.width, height / other.height};
        }

        constexpr auto operator&(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width & other.width, height & other.height};
        }

        constexpr auto operator|(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width | other.width, height | other.height};
        }

        constexpr auto operator^(
            const BasicRectangle& other
        ) const -> BasicRectangle {
            return BasicRectangle{width ^ other.width, height ^ other.height};
        }

        constexpr auto operator~(
        ) const -> BasicRectangle {
            return BasicRectangle{~width, ~height};
        }

        friend auto operator << (
            std::ostream& os,
            const BasicRectangle& rect
        ) -> std::ostream& {
            os << "Rectangle" << "(" << rect.width << ", " << rect.height << ")";
            return os;
        }

        constexpr auto area(
        ) const -> T {
            return width * height;
        }

        constexpr auto circumference(
        ) const -> T {
            return 2 * (width + height);
        }


    };
}


JSONCONS_ALL_MEMBER_TRAITS(Sen::Kernel::Encoding::Image::Rectangle, width, height);