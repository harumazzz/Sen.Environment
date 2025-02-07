#pragma once
#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

	template <typename T>
	using Pointer = T*;

	template <typename T>
	using List = std::vector<T>;

    template <typename Type>
    concept CharacterOnView = std::is_same_v<Type, char> || std::is_same_v<Type, unsigned char>;

    template <typename Type>
    concept IsValidArgument = std::is_same<Type, std::size_t>::value && true;

	template <typename Value>
    concept IsValidConstArgument = (std::is_same<Value, bool>::value || std::is_enum<Value>::value);

	template <typename Type>
	concept Integral = std::is_integral_v<Type>;

	inline static auto constexpr IsCategoryOfInteger = true;

	template <typename T>
	concept CharacterBufferView = std::is_same_v<T, char> || std::is_same_v<T, unsigned char>;

	template <typename T>
	concept HasEmptyMethod = requires(T a) { { a.empty() } -> std::same_as<void>; };

	template <typename T, typename Context>
	concept CanBeAllocated = requires(T t, Context* context) {
		{ t.allocate_new(context) } -> std::same_as<void>; 
	};

	template <typename Type>
	concept SpaceX = std::is_same_v<Type, int32_t> || std::is_same_v<Type, uint32_t>
	|| std::is_same_v<Type, int64_t> || std::is_same_v<Type, uint64_t> || std::is_same_v<Type, float> || std::is_same_v<Type, double> || std::is_same_v<Type, std::string> || std::is_same_v<Type, bool>;

	template <typename Type>
	concept PrimitiveJSValue = std::is_same_v<Type, bool> or std::is_same_v<Type, std::string_view> or
	std::is_integral<Type>::value or std::is_floating_point<Type>::value;

	template <typename T>
    struct function_traits;

    template <typename Ret, typename... Args>
    struct function_traits<Ret(Args...)> {
        using return_type = Ret;
        using args_tuple = std::tuple<Args...>;
    };

    template <typename Ret, typename... Args>
    struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

    template <typename T>
    struct function_traits : function_traits<decltype(&T::operator())> {};

    template <typename ClassType, typename Ret, typename... Args>
    struct function_traits<Ret(ClassType::*)(Args...) const> : function_traits<Ret(Args...)> {};

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
		static_assert(false, "Do not use std::unordered_map");
	};

	template <typename T>
	struct get_type;

	template <typename U>
	struct get_type<List<U>> {
		using type = U;
	};

	template <typename T>
	using get_type_t = typename get_type<T>::type;

	template <typename T>
	concept IsClass = (std::is_class_v<T> && !std::is_pointer_v<T>);

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
	struct is_optional : std::false_type {};

	template <typename T>
	struct is_optional<std::optional<T>> : std::true_type {};

	template <typename T>
	constexpr auto is_optional_v = is_optional<T>::value;

	template <typename... T>
	constexpr auto count_optionals() {
		return (is_optional_v<T> + ... + 0); 
	}

	template <typename T>
	struct map_traits;

	template <typename Key, typename Value>
	struct map_traits<std::unordered_map<Key, Value>> {
		using key_type = Key;
		using value_type = Value;
	};

	template <auto T>
	struct TypeOf {
		using type = decltype(T);
	};

	template <auto T>
	using type_of = typename TypeOf<T>::type;

	template <typename T, T Value>
	struct is_greater_than_zero {
		static constexpr auto value = (Value > 0);
	};

	template <auto Value>
	constexpr auto is_greater_than_zero_v = is_greater_than_zero<type_of<Value>, Value>::value;

	template <typename T>
	concept is_numeric_v = (std::is_floating_point_v<T> || std::is_integral_v<T>) && (!std::is_same_v<T, bool> && !std::is_pointer_v<T>);

	template <typename Container>
	using extract_container_t = std::decay_t<typename Container::value_type>;

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

	template <typename Result, typename... Args>
	struct is_callable : std::false_type {

		using ReturnType = Result;

	};

	template <typename Result, typename... Args>
	struct is_callable<Result (*)(Args...)> : std::true_type {

		using ReturnType = Result;

	};

	template <typename Result, typename... Args>
	constexpr auto is_callable_v = is_callable<Result, Args...>::value;

	template <typename T, typename = void>
	struct is_global_function : std::false_type {};

	template <typename Result, typename... Args>
	struct is_global_function<Pointer<Result (Args...)>> : std::true_type {

		using ReturnType = Result;

		using Arguments = std::tuple<Args...>;

	};

	template <typename Callable>
	struct is_global_function<Callable, std::enable_if_t<!std::is_member_function_pointer_v<Callable>>> : std::true_type {
		using ReturnType = decltype(std::declval<Callable>()(std::declval<std::tuple_element_t<0, typename std::decay_t<Callable>::Arguments>>()));
	    using Arguments = typename std::decay_t<Callable>::Arguments;

		template <typename... Args>
		static auto call(Callable&& callable, Args&&... args) {
			return std::forward<Callable>(callable)(std::forward<Args>(args)...);
		}
	};

	template <auto Callable>
	constexpr auto is_global_function_v = is_global_function<std::decay_t<type_of<Callable>>>::value;

}