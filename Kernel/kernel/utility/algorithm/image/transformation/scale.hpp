#pragma once

#include "kernel/utility/algorithm/image/transformation/common.hpp"

namespace Sen::Kernel::Image::Transformation {

    struct Scale : Common {

    protected:

        using Common = Common;

    public:

        static auto process(
            Image& source,
            Image& destination
        ) -> void {
            auto source_buffer = Array<Color>{source.area()};
            auto destination_buffer = Array<Color>{destination.area()};
            auto source_width_size = compute_width_size(source.width);
            auto destination_width_size = compute_width_size(destination.width);
            for (auto y : Range{source.height}) {
                std::memcpy(&source_buffer[y * source.width], &source[y], source_width_size);
            }
            Subprojects::avir::CImageResizer<>{8}.resizeImage(
                reinterpret_cast<uint8_t*>(source_buffer.begin()),
                static_cast<int>(source.width),
                static_cast<int>(source.height),
                0,
                reinterpret_cast<uint8_t*>(destination_buffer.begin()),
                static_cast<int>(destination.width),
                static_cast<int>(destination.height),
                4,
                0.0,
                nullptr
            );
            for (auto y : Range{destination.height}) {
                std::memcpy(&destination[y], &destination_buffer[y * destination.width], destination_width_size);
            }
        }

    };

}
