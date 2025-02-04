#pragma once

#include "kernel/utility/algorithm/image/transformation/common.hpp"

namespace Sen::Kernel::Image::Transformation {

    struct Merge : Common {

    protected:

        using Common = Common;

    public:

        static auto process(
            List<ImagePosition>& source,
            Image& destination
        ) -> void {
            for (auto & image : source) {
                for (auto row : Range{image.height}) {
                    for (auto col : Range{image.width}) {
                        assert_conditional(image.width + image.x <= destination.width and image.height + image.y <= destination.height, fmt::format("{}", Language::get("image.does_not_fit_current_image")), "process");
                        std::memcpy(&destination[image.y + row][image.x + col], &image[row][col], k_color_size);
                    }
                }
            }
        }

    };

}
