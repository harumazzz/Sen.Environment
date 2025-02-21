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

    namespace Detail {

        inline static auto constexpr k_invalid_class_id = u32{Subprojects::quickjs::$JS_INVALID_CLASS_ID};

        template <typename T> requires std::is_class_v<T>
        inline static thread_local auto class_id = u32{0};

        struct Promise {

            JSValue resolve;

            JSValue reject;

        };

    }

    enum class ValueType : u8 {

        undefined,

        null,

        boolean,

        number,

        bigint,

        string,

        object,

        array,

        unknown,

    };


}