#pragma once
#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

    template<typename T>
    concept is_not_void = (!std::is_void_v<T>);

    template <typename T>
    using Pointer = T*;

    template <typename T>
    struct is_smart_pointer : std::false_type {

    };

    template <typename T>
    struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {

    };

    template <typename T>
    struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {

    };

    template <typename T>
    struct is_smart_pointer<std::weak_ptr<T>> : std::true_type {

    };

    template <typename T>
    inline constexpr bool is_smart_pointer_v = is_smart_pointer<std::decay_t<T>>::value;

    template <auto T>
    inline constexpr bool is_smart_pointer_of_v = is_smart_pointer<std::decay_t<decltype(T)>>::value;

    template <typename T>
    struct is_shared_pointer : std::false_type {

    };

    template <typename T>
    struct is_shared_pointer<std::shared_ptr<T>> : std::true_type {

    };

    template <typename T>
    inline constexpr bool is_shared_pointer_v = is_shared_pointer<std::decay_t<T>>::value;

    template <auto T>
    inline constexpr bool is_shared_pointer_of_v = is_shared_pointer<std::decay_t<decltype(T)>>::value;

    template <typename T>
    struct is_unique_pointer : std::false_type {

    };

    template <typename T>
    struct is_unique_pointer<std::unique_ptr<T>> : std::true_type {

    };

    template <typename T>
    inline constexpr bool is_unique_pointer_v = is_unique_pointer<std::decay_t<T>>::value;

    template <auto T>
    inline constexpr bool is_unique_pointer_of_v = is_unique_pointer<std::decay_t<decltype(T)>>::value;

    template <typename T>
    struct is_weak_pointer : std::false_type {

    };

    template <typename T>
    struct is_weak_pointer<std::weak_ptr<T>> : std::true_type {

    };

    template <typename T>
    inline constexpr bool is_weak_pointer_v = is_weak_pointer<std::decay_t<T>>::value;

    template <auto T>
    inline constexpr bool is_weak_pointer_of_v = is_weak_pointer<std::decay_t<decltype(T)>>::value;

}