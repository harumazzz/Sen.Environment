#pragma once

#include "kernel/utility/algorithm/image/transformation/common.hpp"

namespace Sen::Kernel::Image::Transformation {

    struct Resize : Common {

    protected:

        using Common = Common;

    public:

        static auto process(
            Image& source,
            Image& destination
        ) -> void {
            auto ratio_x = calculate_ratio(source.width, destination.width);
            auto ratio_y = calculate_ratio(source.height, destination.height);
            for (auto row : Range{destination.height}) {
                for (auto col : Range{destination.width}) {
                    auto x = col * ratio_x;
                    auto y = row * ratio_y;
                    std::memcpy(&destination[row][col], &source[y][x], k_color_size);
                }
            }
        }

    };

}
