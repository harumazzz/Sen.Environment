#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/encoding/image/image_position.hpp"

namespace Sen::Kernel::Image::Transformation {

    inline auto constexpr color_size (
    ) -> usize {
        return sizeof(u8) * 4;
    }

    struct Common {

    protected:

        using Image = Encoding::Image::CImage;

        using ImagePosition = Encoding::Image::ImagePosition;

        using Color = Encoding::Image::Color;

        inline static auto constexpr k_color_size = color_size();

        inline static auto constexpr k_pi = 3.14159265358979323846;

        inline static auto constexpr k_half_angle = 180.0;

        inline static auto constexpr k_amount = 2.0;

        inline static auto constexpr k_default_color = 0_ui;

        template <typename T>
        using List = CList<T>;

        template <typename T>
        using Array = CArray<T>;

    public:

        template <typename T> requires is_numeric_v<T>
        static auto constexpr calculate_ratio (
            const T& old_size,
            const T& new_size
        ) -> usize {
            return static_cast<usize>(static_cast<double>(old_size) / static_cast<double>(new_size));
        }

        static auto constexpr lerp_color (
            const u8 c1,
            const u8 c2,
            const double& t
        ) -> u8 {
            return static_cast<u8>(c1 * (1 - t) + c2 * t);
        }

        static auto constexpr lerp (
            const Color& c1,
            const Color& c2,
            const double& t,
            Color& result
        ) -> void {
            result.red(lerp_color(c1.red(), c2.red(), t));
            result.green(lerp_color(c1.green(), c2.green(), t));
            result.blue(lerp_color(c1.blue(), c2.blue(), t));
            result.alpha(lerp_color(c1.alpha(), c2.alpha(), t));
        }

        template <typename T>
        static auto constexpr compute_width_size (
            const T& size
        ) -> usize {
            return static_cast<usize>(sizeof(Color) * size);
        }

    };

}
