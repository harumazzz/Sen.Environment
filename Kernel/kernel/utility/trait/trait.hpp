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

	template <typename Key, typename Value, typename Compare, typename Alloc>
	struct is_map<std::map<Key, Value, Compare, Alloc>> : std::false_type {
		static_assert(false, "Do not use std::map");
	};

	template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Alloc>
	struct is_map<std::unordered_map<Key, Value, Hash, KeyEqual, Alloc>> : std::false_type {
		static_assert(false, "Do not use std::unordered_map");
	};

	template <typename Key, typename Value>
	struct is_map<tsl::ordered_map<Key, Value>> : std::true_type {};

	template <typename T>
	struct get_type;

	template <typename U>
	struct get_type<List<U>> {
		using type = U;
	};

	template <typename T>
	using get_type_t = typename get_type<T>::type;

	template <typename T>
	concept IsClass = (std::is_class<T>::value && !std::is_pointer<T>::value);

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
	struct is_std_array : std::false_type {};

	template <typename T, std::size_t Size>
	struct is_std_array<std::array<T, Size>> : std::true_type {};

	template <typename T>
	constexpr auto is_std_array_v = is_std_array<T>::value;

}