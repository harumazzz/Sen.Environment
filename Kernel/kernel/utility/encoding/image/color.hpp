#pragma once

#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Encoding::Image {

    template <typename T> requires (std::is_integral_v<T> && !std::is_pointer_v<T>)
    inline static constexpr auto min_of_integer = std::numeric_limits<T>::min();

    template <typename T> requires (std::is_integral_v<T> && !std::is_pointer_v<T>)
    inline static constexpr auto max_of_integer = std::numeric_limits<T>::max();

    template <typename T> requires (std::is_integral_v<T> && !std::is_pointer_v<T>)
    static constexpr auto clamp(const T& value) -> T {
        return std::max(min_of_integer<T>, std::min(value, max_of_integer<T>));
    }

    template <typename T> requires (std::is_integral_v<T> && !std::is_pointer_v<T>)
    struct BasicColorRGB {
        
    protected:

        T m_red;

        T m_green;

        T m_blue;

    public:
        constexpr explicit BasicColorRGB() : m_red{0}, m_green{0}, m_blue{0} {}
        
        constexpr explicit BasicColorRGB(const T& red, const T& green, const T& blue)
            : m_red{red}, m_green{green}, m_blue{blue} {}

        constexpr BasicColorRGB(const BasicColorRGB& other) = default;

        constexpr BasicColorRGB(BasicColorRGB&& other) noexcept = default;

        constexpr ~BasicColorRGB() = default;

        constexpr auto operator=(const BasicColorRGB& other) -> BasicColorRGB& = default;

        constexpr auto operator=(BasicColorRGB&& other) -> BasicColorRGB& = default;

        constexpr auto red(

        ) const -> T { 
            return thiz.m_red; 
        }

        constexpr auto green(

        ) const -> T { 
            return m_green; 
        }

        constexpr auto blue(
            
        ) const -> T {
             return m_blue; 
        }

        constexpr auto red(
            const T& value
        ) -> void { 
            thiz.m_red = clamp(value); 
        }

        constexpr auto green(
            const T& value
        ) -> void { 
            thiz.m_green = clamp(value); 
        }

        constexpr auto blue(
            const T& value
        ) -> void { 
            thiz.m_blue = clamp(value); 
        }

        constexpr auto operator+(
            const BasicColorRGB& other
        ) -> BasicColorRGB {
            return BasicColorRGB{thiz.m_red + other.m_red, thiz.m_green + other.m_green, thiz.m_blue + other.m_blue};
        }

        constexpr auto operator-(
            const BasicColorRGB& other
        ) -> BasicColorRGB {
            return BasicColorRGB{thiz.m_red - other.m_red, thiz.m_green - other.m_green, thiz.m_blue - other.m_blue};
        }

        constexpr auto operator *(
            const BasicColorRGB& other
        ) -> BasicColorRGB {
            return BasicColorRGB{m_red * other.m_red, m_green * other.m_green, m_blue * other.m_blue};
        }

        constexpr auto operator /(
            const BasicColorRGB& other
        ) -> BasicColorRGB {
            return BasicColorRGB{m_red / other.m_red, m_green / other.m_green, m_blue / other.m_blue};
        }

        constexpr auto operator&(
            const BasicColorRGB& other
        ) const -> BasicColorRGB {
            return BasicColorRGB{m_red & other.m_red, m_green & other.m_green, m_blue & other.m_blue};
        }

        constexpr auto operator|(
            const BasicColorRGB& other
        ) const -> BasicColorRGB {
            return BasicColorRGB{m_red | other.m_red, m_green | other.m_green, m_blue | other.m_blue};
        }

        constexpr auto operator^(
            const BasicColorRGB& other
        ) const -> BasicColorRGB {
            return BasicColorRGB{m_red ^ other.m_red, m_green ^ other.m_green, m_blue ^ other.m_blue};
        }

        constexpr auto operator~(

        ) const -> BasicColorRGB {
            return BasicColorRGB{~m_red, ~m_green, ~m_blue};
        }

        friend auto operator<<(
            std::ostream& os, 
            const BasicColorRGB& color
        ) -> std::ostream& {
            os << "BasicColorRGB(" << static_cast<int>(color.m_red) << ", "
            << static_cast<int>(color.m_green) << ", "
            << static_cast<int>(color.m_blue) << ")";
            return os;
        }

    };

    template <typename T> requires (std::is_integral_v<T> && !std::is_pointer_v<T>)
    struct BasicColor : public BasicColorRGB<T> {

    protected:

        T m_alpha;

    public:
        constexpr explicit BasicColor(

        ) : BasicColorRGB<T>{}, m_alpha{0} {}

        constexpr explicit BasicColor(
            const T& red, 
            const T& green, 
            const T& blue, 
            const T& alpha
        )
            : BasicColorRGB<T>{red, green, blue}, m_alpha{alpha} {}

        constexpr BasicColor(
            const BasicColor& other
        ) = default;

        constexpr BasicColor(
            BasicColor&& other
        ) noexcept = default;

        constexpr ~BasicColor(

        ) = default;

        constexpr auto operator=(
            const BasicColor& other
        ) -> BasicColor& = default;

        constexpr auto operator=(
            BasicColor&& other
        ) -> BasicColor& = default;

        constexpr auto alpha(

        ) const -> T { 
            return thiz.m_alpha; 
        }

        constexpr auto alpha(const T& value) -> void { 
            thiz.m_alpha = clamp(value); 
        }

        constexpr auto operator+(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{
                thiz.m_red + other.m_red,
                thiz.m_green + other.m_green,
                thiz.m_blue + other.m_blue,
                thiz.m_alpha + other.m_alpha
            };
        }

        constexpr auto operator-(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{
                thiz.m_red - other.m_red,
                thiz.m_green - other.m_green,
                thiz.m_blue - other.m_blue,
                thiz.m_alpha - other.m_alpha
            };
        }

        constexpr auto operator *(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{thiz.m_red * other.m_red, thiz.m_green * other.m_green, thiz.m_blue * other.m_blue, m_alpha * other.m_alpha};
        }

        constexpr auto operator /(
            const BasicColor& other
        ) -> BasicColor {
            return BasicColor{thiz.m_red / other.m_red, thiz.m_green / other.m_green, thiz.m_blue / other.m_blue, m_alpha / other.m_alpha};
        }

        constexpr auto operator&(
            const BasicColor& other
        ) const -> BasicColor {
            return BasicColor{thiz.m_red & other.m_red, thiz.m_green & other.m_green, thiz.m_blue & other.m_blue, m_alpha & other.m_alpha};
        }

        constexpr auto operator|(
            const BasicColor& other
        ) const -> BasicColor {
            return BasicColor{thiz.m_red | other.m_red, thiz.m_green | other.m_green, thiz.m_blue | other.m_blue, m_alpha | other.m_alpha};
        }

        constexpr auto operator^(
            const BasicColor& other
        ) const -> BasicColor {
            return BasicColor{thiz.m_red ^ other.m_red, thiz.m_green ^ other.m_green, thiz.m_blue ^ other.m_blue, m_alpha ^ other.m_alpha};
        }

        constexpr auto operator~(

        ) const -> BasicColor {
            return BasicColor{~thiz.m_red, ~thiz.m_green, ~thiz.m_blue, ~m_alpha};
        }

        friend auto operator<<(
            std::ostream& os, 
            const BasicColor& color
        ) -> std::ostream& {
            os << "BasicColor(" << static_cast<int>(color.m_red) << ", "
            << static_cast<int>(color.m_green) << ", "
            << static_cast<int>(color.m_blue) << ", "
            << static_cast<int>(color.m_alpha) << ")";
            return os;
        }
    };

    using Color = BasicColor<u8>;

    using ColorRSB = BasicColorRGB<u8>;

}