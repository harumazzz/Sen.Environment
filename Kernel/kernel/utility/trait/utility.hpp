#pragma once
#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

    template <auto T>
    struct TypeOf {
        using type = decltype(T);
    };

    template <auto T>
    using type_of = typename TypeOf<T>::type;

    template <typename T>
    struct get_type;

    template <typename T>
    using get_type_t = typename get_type<T>::type;

    template <typename T>
    concept is_class = (std::is_class_v<T> && !std::is_pointer_v<T>);

    template <typename T>
    struct is_optional : std::false_type {};

    template <typename T>
    struct is_optional<std::optional<T>> : std::true_type {};

    template <typename T>
    constexpr auto is_optional_v = is_optional<T>::value;

    template <typename... T>
    constexpr auto count_optionals() {
        return (is_optional_v<T> + ... + 0);
    }

    template <typename Container>
    using extract_container_t = std::decay_t<typename Container::value_type>;

    template <typename Result, typename... Args>
    struct is_callable : std::false_type {

        using ReturnType = Result;

    };

    template <typename T, auto... Indexes> requires (std::is_same_v<type_of<Indexes>, type_of<std::tuple_size_v<T>>> && ...)
    constexpr auto is_valid_tuple(std::index_sequence<Indexes...>) {
        return (!std::is_void_v<std::tuple_element_t<Indexes, T>> && ...);
    }

    template <typename T>
    using remove_reference = std::remove_cvref_t<T>;

    template <typename Method>
    struct extract_class;

    template <typename ClassType, typename ReturnType, typename... Args>
    struct extract_class<ReturnType(ClassType::*)(Args...)> {
        using type = ClassType;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct extract_class<ReturnType(ClassType::*)(Args...) const> {
        using type = ClassType;
    };

    template <auto function>
    constexpr auto extract_class_t = extract_class<decltype(function)>::type;



    template <typename T>
    struct Transform {
        using type = T;
    };

    template <typename T>
    struct TransformWrapper {
        using type = typename Transform<T>::type;
    };

    template <typename Tuple, template <typename> class Transformer>
    struct TupleConverter;

    template <template <typename> class Transformer, typename... Args>
    struct TupleConverter<std::tuple<Args...>, Transformer> {
        using type = std::tuple<typename Transformer<Args>::type...>;
    };

    template <typename Tuple, template <typename> class Transformer>
    using ConvertTuple = typename TupleConverter<Tuple, Transformer>::type;

    template <typename Tuple, template <typename> class Transformer>
    using ConvertTuple = typename TupleConverter<Tuple, Transformer>::type;

    template <typename... Args>
    using Tuple = std::tuple<Args...>;

    template <typename T>
    struct is_enum_class : std::bool_constant<std::is_enum_v<T> && !std::is_convertible_v<T, int>> {};

    template <typename T>
    constexpr auto is_enum_class_v = is_enum_class<T>::type;

    template <typename T>
    concept is_implemented_stream = requires(std::ostream& os, const T& value) {
            { os << value } -> std::same_as<std::ostream&>;
    };

    template <auto T>
    constexpr auto is_implemented_stream_v = is_implemented_stream<decltype(T)>;

    template <typename T>
    struct is_trivially_copyable
        : std::bool_constant<std::is_integral_v<T> ||
                             std::is_floating_point_v<T> ||
                             std::is_pointer_v<T>>
    {};

    template <typename T>
    inline constexpr auto is_trivially_copyable_v = is_trivially_copyable<T>::value;

}