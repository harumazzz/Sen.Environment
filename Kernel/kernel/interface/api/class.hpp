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

	}



}