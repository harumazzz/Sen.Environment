#pragma once

#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

	inline static auto constexpr free_memory = [](auto p)
	{
		if (p != nullptr) {
			delete p;
		}
	};
	
	template <typename T>
	inline static auto constexpr free_array = [](T* p)
	{
		if (p != nullptr) {
			delete[] p;
		}
	};

	template <typename T>
	constexpr auto as_lvalue(
		T&& value
	)  noexcept -> std::add_lvalue_reference_t<T>
	{
		return value; 
	}

	template <typename T>
	constexpr auto  as_rvalue(
		T&& value
	) noexcept -> std::remove_reference_t<T>&&
	{
		return std::forward<T>(value);
	}

	template <typename To, typename From>
	auto steal_reference(From& from) -> To& {
		static_assert(sizeof(To) == sizeof(From), "The sizeof the destination and source is not the same size, unsafe cast!");
		return *reinterpret_cast<To*>(&from);
	}

	template <typename To, typename From>
	auto steal_pointer(From* from) -> To* {
		static_assert(sizeof(To) == sizeof(From), "The sizeof the destination and source is not the same size, unsafe cast!");
		return reinterpret_cast<To*>(from);
	}
}