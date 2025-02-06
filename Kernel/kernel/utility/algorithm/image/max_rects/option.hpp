#pragma once

#include "kernel/utility/algorithm/image/max_rects/common.hpp"

namespace Sen::Kernel::Image::MaxRects {

    template <typename T> requires std::is_integral_v<T>
    struct BasicOption;

    using Option = BasicOption<u32>;

    template <typename T> requires std::is_integral_v<T>
    struct BasicOption {

        bool smart;

        bool pot;

        bool square;

        T border;

        T padding;

        LOGIC logic;

        constexpr explicit BasicOption() : smart{true}, pot{true}, square{false}, border{0}, padding{1}, logic{LOGIC::MAX_AREA} {
        }

        constexpr ~BasicOption(
        ) = default;

    };

    inline static auto constexpr option = Option{};

}