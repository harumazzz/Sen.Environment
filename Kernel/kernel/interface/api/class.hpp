#pragma once

#include "kernel/interface/shell.hpp"

namespace Sen::Kernel::Interface {

	namespace Detail {

		template <typename T, typename... Args> requires std::is_class_v<T>
		inline constexpr auto make_constructor (
			Args&&... args
		) -> Pointer<T> {
			return new T{std::forward<Args>(args)...};
		}

		template <auto method, typename... Args> requires std::is_member_function_pointer_v<decltype(method)> && (!std::is_void_v<Args> && ...)
		inline constexpr auto make_member_function (
			Pointer<typename extract_class<decltype(method)>::type> instance,
			Args&&... args
		) -> decltype(std::invoke(method, instance, std::forward<Args>(args)...)) {
			return std::invoke(method, instance, std::forward<Args>(args)...);
		}

		template <auto member>
		inline constexpr auto make_setter (
			Pointer<member_class_t<member>> instance,
			std::remove_reference_t<decltype(instance->*member)>&& value
		) -> void {
			std::invoke(member, *instance) = std::forward<decltype(value)>(value);
		}

		template <auto member>
		inline constexpr auto make_getter (
			Pointer<member_class_t<member>> instance
		) -> decltype(std::invoke(member, *instance)) {
			return std::invoke(member, *instance);
		}

	}



}