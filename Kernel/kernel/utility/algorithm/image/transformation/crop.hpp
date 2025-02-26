#pragma once

#include "kernel/utility/algorithm/image/transformation/common.hpp"

namespace Sen::Kernel::Image::Transformation {

    struct Crop : Common {

    protected:

        using Common = Common;

    public:

        static auto process(
            Image& source,
            ImagePosition& destination
        ) -> void {
            if (destination.x + destination.width > source.width || destination.y + destination.height > source.height) {
                throw RuntimeException{"Crop area exceeds image bounds", std::source_location::current(), "process"};
            }
            auto x = destination.x;
            auto y = destination.y;
            for (auto row : Range{destination.height}) {
                for (auto col : Range{destination.width}) {
                    std::memcpy(&destination[row][col], &source[y + row][x + col], k_color_size);
                }
            }
        }

    };

}
