#pragma once

#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

	template <typename T>
	inline constexpr auto as_lvalue(
		T&& value
	)  noexcept -> std::add_lvalue_reference_t<T>
	{
		return value; 
	}

	template <typename T>
	inline constexpr auto as_rvalue(
		T&& value
	) noexcept -> std::remove_reference_t<T>&&
	{
		return std::forward<T>(value);
	}

	template <typename To, typename From>
	inline constexpr auto steal_reference(
		From& from
	) -> To& {
		static_assert(sizeof(To) == sizeof(From), "The sizeof the destination and source is not the same size, unsafe cast!");
		return *reinterpret_cast<To*>(&from);
	}

	template <typename To, typename From>
	inline constexpr auto steal_pointer(
		From* from
	) -> To* {
		static_assert(sizeof(To) == sizeof(From), "The sizeof the destination and source is not the same size, unsafe cast!");
		return reinterpret_cast<To*>(from);
	}

	template <typename T>
	inline constexpr auto take_address(
		T& obj
	) -> T* {
		return std::addressof(obj);
	}

	template <typename Base, typename Derived>
	inline constexpr auto up_cast_pointer(
		Derived* derived
	) -> Base* {
		static_assert(std::is_base_of_v<Base, Derived>, "Derived must inherit from Base");
		return derived;
	}

	template <typename Derived, typename Base>
	inline constexpr auto down_cast_pointer(
		Base* base
	) -> Derived* {
		static_assert(std::is_base_of_v<Base, Derived>, "Derived must inherit from Base");
		return static_cast<Derived*>(base);
	}

	template <typename Base, typename Derived>
	inline constexpr auto up_cast(
		Derived& derived
	) -> Base& {
		static_assert(std::is_base_of_v<Base, Derived>, "Derived must inherit from Base");
		return derived;
	}

	template <typename Derived, typename Base>
	inline constexpr auto down_cast(
		Base& base
	) -> Derived& {
		static_assert(std::is_base_of_v<Base, Derived>, "Derived must inherit from Base");
		return static_cast<Derived&>(base);
	}

	template <typename T>
	inline constexpr auto remove_pointer(
		T* ptr
	) -> T& {
		return *ptr;
	}

	template <typename T>
	inline constexpr auto remove_pointer(
		const T* ptr
	) -> const T& {
		return *ptr;
	}

	template <typename T>
	inline constexpr auto as_move (
		T& value
	) -> std::remove_cvref_t<T>&& {
		return static_cast<std::remove_cvref_t<T>&&>(value);
	}

	template <typename T>
	inline constexpr auto as_const (
		T& value
	) -> std::remove_cvref_t<T> const & {
		return static_cast<std::remove_cvref_t<T> const &>(value);
	}

}