#pragma once

#include <type_traits>

namespace Sen::Kernel {

    template <typename T> requires std::is_arithmetic_v<T>
    class Iterator {

        T i;

        T step;

    public:

        constexpr Iterator(
            const T& start,
            const T& step
        ) noexcept : i{start}, step{step}
        {

        }

        constexpr auto operator != (
            const Iterator& that
        ) const noexcept -> bool
        {
            return i != that.i;
        }

        constexpr auto operator++ (

        ) noexcept -> const Iterator&
        {
            this->i += step;
            return *this;
        }

        constexpr auto operator* (

        ) noexcept -> T
        {
            return this->i;
        }
    };

}
