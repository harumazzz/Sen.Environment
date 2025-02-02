#pragma once

#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Encoding::Image {

    template <typename T> requires std::is_integral_v<T>
    struct BasicColor;

    using Color = BasicColor<u8>;

    template <typename T> requires std::is_integral_v<T>
    struct BasicColor {

    protected:

        inline static auto constexpr min_value = std::numeric_limits<T>::min();

        inline static auto constexpr max_value = std::numeric_limits<T>::max();

        T m_red;

        T m_green;

        T m_blue;

        T m_alpha;

    public:

        constexpr explicit BasicColor(
        ) : m_red{0}, m_green{0}, m_blue{0}, m_alpha{0} {

        }

        constexpr explicit BasicColor(
            const T& red,
            const T& green,
            const T& blue,
            const T& alpha
        ) : m_red{red}, m_green{green}, m_blue{blue}, m_alpha{alpha} {
        }

        constexpr BasicColor(
            const BasicColor& other
        ) = default;

        constexpr BasicColor(
            const BasicColor&& other
        ) noexcept : m_red{other.m_red}, m_green{other.m_green}, m_blue{other.m_blue}, m_alpha{other.m_alpha} {
            other.m_red = 0;
            other.m_green = 0;
            other.m_blue = 0;
            other.m_alpha = 0;
        }

        ~BasicColor(
        ) = default;

        auto operator=(
            const BasicColor& other
        ) -> BasicColor& = default;

        auto operator=(
            BasicColor&& other
        ) -> BasicColor& = default;

        auto operator +(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{m_red + other.m_red, m_green + other.m_green, m_blue + other.m_blue, m_alpha + other.m_alpha};
        }

        auto operator -(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{m_red - other.m_red, m_green - other.m_green, m_blue - other.m_blue, m_alpha - other.m_alpha};
        }

        auto operator *(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{m_red * other.m_red, m_green * other.m_green, m_blue * other.m_blue, m_alpha * other.m_alpha};
        }

        auto operator /(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{m_red / other.m_red, m_green / other.m_green, m_blue / other.m_blue, m_alpha / other.m_alpha};
        }

        auto operator&(
            const BasicColor& other
        ) const -> BasicColor {
            return BasicColor{m_red & other.m_red, m_green & other.m_green, m_blue & other.m_blue, m_alpha & other.m_alpha};
        }

        auto operator|(
            const BasicColor& other
        ) const -> BasicColor {
            return BasicColor{m_red | other.m_red, m_green | other.m_green, m_blue | other.m_blue, m_alpha | other.m_alpha};
        }

        auto operator^(
            const BasicColor& other
        ) const -> BasicColor {
            return BasicColor{m_red ^ other.m_red, m_green ^ other.m_green, m_blue ^ other.m_blue, m_alpha ^ other.m_alpha};
        }

        auto operator~(

        ) const -> BasicColor {
            return BasicColor{~m_red, ~m_green, ~m_blue, ~m_alpha};
        }

    protected:

        auto clamp (
            const T& value
        ) -> T {
            return std::max(thiz.min_value, std::min(value, max_value));
        }

    public:

        auto red(
        ) const -> T {
            return thiz.m_red;
        }

        auto green(
        ) const -> T {
            return thiz.m_green;
        }

        auto blue(
        ) const -> T {
            return thiz.m_blue;
        }

        auto alpha(
        ) const -> T {
            return thiz.m_alpha;
        }

        auto red (
            const T& value
        ) -> void
        {
            thiz.m_red = thiz.clamp(value);
        }

        auto green (
            const T& value
        ) -> void
        {
            thiz.m_green = thiz.clamp(value);
        }

        auto blue (
            const T& value
        ) -> void
        {
            thiz.m_blue = thiz.clamp(value);
        }

        auto alpha (
            const T& value
        ) -> void
        {
            thiz.m_alpha = thiz.clamp(value);
        }

        friend auto operator <<(
            std::ostream& os,
            const BasicColor& color
        ) -> std::ostream& {
            os << "Color" << "(" << color.m_red << ", " << color.m_green << ", " << color.m_blue << ", " << color.m_alpha << ")";
            return os;
        }
    };

}
