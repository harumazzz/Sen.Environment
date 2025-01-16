#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"
#include "kernel/utility/javascript/converter.hpp"
#include <typeinfo>

namespace Sen::Kernel::JavaScript {

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

}