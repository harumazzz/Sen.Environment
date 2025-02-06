#pragma once
#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Image::MaxRects {

    enum class LOGIC : u8
    {
        MAX_AREA,
        MAX_EDGE,
    };

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

        BasicOption(
            BasicOption&& other
        )= delete;

        BasicOption(
            const BasicOption& other
        )= delete;

        auto operator =(
            const BasicOption& other
        ) -> BasicOption& = delete;

        auto operator =(
            BasicOption&& other
        ) -> BasicOption& = delete;

        constexpr ~BasicOption(
        ) = default;

    };

    struct Common {

        inline static auto constexpr EDGE_MAX_VALUE = 4096_size;

        inline static auto constexpr EDGE_MIN_VALUE = 128_size;

        inline static auto constexpr k_score = 1.7976931348623157e+308;

        inline static auto constexpr k_log_2e = 1.4426950408889634;

        inline static auto constexpr option = Option{};

    };

}
