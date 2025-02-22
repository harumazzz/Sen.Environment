#pragma once

#include "kernel/utility/trait/utility.hpp"

namespace Sen::Kernel {

    template <typename T, T Value>
    struct is_greater_than_zero {
        static constexpr auto value = (Value > 0);
    };

    template <auto Value>
    constexpr auto is_greater_than_zero_v = is_greater_than_zero<type_of<Value>, Value>::value;

    template <typename T>
    concept is_numeric_v = (std::is_floating_point_v<T> || std::is_integral_v<T>) && (!std::is_same_v<T, bool> && !std::is_pointer_v<T>);


    template <auto Value, auto Min, auto Max>
    struct is_between : std::bool_constant<(Min <= Value && Value <= Max)> {};

    template <auto Value, auto Min, auto Max>
    constexpr auto is_between_v = is_between<Value, Min, Max>::value;

    template <typename T>
    struct is_boolean : std::false_type {
        static constexpr auto value_type = false;
    };

    template <>
    struct is_boolean<bool> : std::true_type {
        static constexpr auto value_type = true;
    };

    template <typename T>
    constexpr auto is_boolean_v = is_boolean<T>::value;

    template<typename T>
    concept is_integer = std::is_arithmetic_v<T> && (!std::is_floating_point_v<T>);

}