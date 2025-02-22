#pragma once

#include "kernel/utility/trait/pointer.hpp"
#include "kernel/utility/trait/utility.hpp"

namespace Sen::Kernel {

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

	template <auto Callable>
	constexpr auto is_global_function_v = is_global_function<std::decay_t<type_of<Callable>>>::value;

	template <typename... Ts>
	constexpr auto total_sizeof(
	) -> size_t {
		return (sizeof(Ts) + ... + 0_size);
	}

	template <typename T>
	struct callable_traits;

	/// Note : Functions

	template <typename Ret, typename... Args>
	struct callable_traits<Ret(*)(Args...)> {
		using return_type = Ret;
		using args_tuple = std::tuple<std::decay_t<Args>...>;
		static constexpr auto arg_count = sizeof...(Args);
	};

	/// Note : Lambda

	template <typename F>
	struct callable_traits : callable_traits<decltype(&F::operator())> {
	};

	/// Note : Class or Struct

	template <typename Ret, typename Class, typename... Args>
	struct callable_traits<Ret(Class::*)(Args...) const> {
		using return_type = Ret;
		using args_tuple = std::tuple<std::decay_t<Args>...>;
		static constexpr auto arg_count = sizeof...(Args);
		using class_type = Class;
	};

	template <typename Ret, typename Class, typename... Args>
	struct callable_traits<Ret(Class::*)(Args...)> {
		using return_type = Ret;
		using args_tuple = std::tuple<std::decay_t<Args>...>;
		static constexpr auto arg_count = sizeof...(Args);
		using class_type = Class;
	};

	template <typename Ret, typename Class, typename... Args>
	struct callable_traits<Ret(Class::*)(Args...) volatile> {
		using return_type = Ret;
		using args_tuple = std::tuple<std::decay_t<Args>...>;
		static constexpr auto arg_count = sizeof...(Args);
		using class_type = Class;
	};

	template <typename Ret, typename Class, typename... Args>
	struct callable_traits<Ret(Class::*)(Args...) const volatile> {
		using return_type = Ret;
		using args_tuple = std::tuple<std::decay_t<Args>...>;
		static constexpr auto arg_count = sizeof...(Args);
		using class_type = Class;
	};

	template <typename F>
	using function_return = typename callable_traits<F>::return_type;

	template <auto member>
	struct member_class;

	template <typename Class, typename Type, Type Class::* member>
	struct member_class<member> {
		using type = Class;
	};

	template <auto member>
	using member_class_t = typename member_class<member>::type;

	template<typename T>
	concept is_exchanger = std::is_same_v<T, std::remove_cvref_t<T>> && (!std::is_void_v<T>) && (!std::is_reference_v<T>);

}