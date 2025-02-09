#pragma once

#include "kernel/subprojects/quickjs.hpp"
#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Javascript {

    template <typename T>
    using List = CList<T>;

    template <typename T>
    using Array = CArray<T>;

    struct JSString;

    struct Runtime;

    struct Context;

    struct Value;

    template <typename T>
    struct Trait {

        using Type = T;

        using value_type = Type;

        static auto from_value(Value& source, T& destination) -> void = delete;

        template <typename U>
        static auto to_value(U&& source, Value& value) -> void = delete;

    };

}