#pragma once

#include "kernel/utility/algorithm/image/transformation/common.hpp"

namespace Sen::Kernel::Image::Transformation {

    struct Rotate : Common {

    protected:

        using Common = Common;

    public:

        static auto process(
            Image& source,
            Image& destination,
            const double& angle
        ) -> void {
            auto radians = angle * k_pi / k_half_angle;
            auto cos_theta = std::cos(radians);
            auto sin_theta = std::sin(radians);
            auto cx = source.width / k_amount;
            auto cy = source.height / k_amount;
            auto new_width = std::ceil(std::abs(source.width * cos_theta) + std::abs(source.height * sin_theta));
            auto new_height = std::ceil(std::abs(source.width * sin_theta) + std::abs(source.height * cos_theta));
            auto new_cx = new_width / k_amount;
            auto new_cy = new_height / k_amount;
            for (auto y : Range{static_cast<usize>(new_height)}) {
                for (auto x : Range{static_cast<usize>(new_width)}) {
                    auto source_x = (x - new_cx) * cos_theta + (y - new_cy) * sin_theta + cx;
                    auto source_y = -(x - new_cx) * sin_theta + (y - new_cy) * cos_theta + cy;
                    if (source_x >= 0 && source_x < source.width - 1 && source_y >= 0 && source_y < source.height - 1) {
                        auto x1 = static_cast<u32>(source_x);
                        auto y1 = static_cast<u32>(source_y);
                        auto x2 = x1 + 1;
                        auto y2 = y1 + 1;
                        auto dx = source_x - x1;
                        auto dy = source_y - y1;
                        auto& c11 = source[y1][x1];
                        auto& c12 = source[y1][x2];
                        auto& c21 = source[y2][x1];
                        auto& c22 = source[y2][x2];
                        auto top = Color{};
                        lerp(c11, c12, dx, top);
                        auto bottom = Color{};
                        lerp(c21, c22, dx, bottom);
                        lerp(top, bottom, dy, destination[y][x]);
                    } else {
                        destination[y][x].red(k_default_color);
                        destination[y][x].green(k_default_color);
                        destination[y][x].blue(k_default_color);
                        destination[y][x].alpha(k_default_color);
                    }
                }
            }
        }

    };

}
