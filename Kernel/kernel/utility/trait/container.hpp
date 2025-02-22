#pragma once
#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

    template <typename T>
    struct is_container : std::false_type {};

    template <typename T, typename Alloc>
    struct is_container<std::vector<T, Alloc>> : std::true_type {};

    template <typename T, std::size_t N>
    struct is_container<std::array<T, N>> : std::true_type {};

    template <typename T>
    struct is_map : std::false_type {};

    template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Alloc>
    struct is_map<std::unordered_map<Key, Value, Hash, KeyEqual, Alloc>> : std::false_type {
    };

    template <typename Container>
    struct container_traits {
        using value_type = void;
    };

    template <typename T, typename Alloc>
    struct container_traits<std::vector<T, Alloc>> {
        using value_type = T;
    };

    template <typename T, std::size_t N>
    struct container_traits<std::array<T, N>> {
        using value_type = T;
    };

    template <typename Container>
    using container_value_t = typename container_traits<Container>::value_type;

    template <typename T>
    struct is_array : std::false_type {};

    template <typename T, std::size_t Size>
    struct is_array<std::array<T, Size>> : std::true_type {};

    template <typename T>
    constexpr auto is_array_v = is_array<T>::value;

    template <typename T>
    struct map_traits;

    template <typename Key, typename Value>
    struct map_traits<std::unordered_map<Key, Value>> {
        using key_type = Key;
        using value_type = Value;
    };

    template <typename T>
    struct array_traits;

    template <typename T, std::size_t N>
    struct array_traits<std::array<T, N>> {

        using type = T;

        static constexpr auto size = N;
    };

    template <typename T>
    using array_value_t = typename array_traits<T>::type;

    template <typename T>
    inline constexpr auto array_size_v = array_traits<T>::size;

    template <typename T>
    struct is_list : std::false_type {};

    template <typename T>
    constexpr auto is_list_v = is_list<T>::value;

    template <auto T>
    constexpr auto is_list_of_v = is_list<decltype(T)>::value;


}