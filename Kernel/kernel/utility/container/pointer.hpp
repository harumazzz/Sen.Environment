#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel {

	inline static auto constexpr free_memory = [](auto p)
	{
		if (p != nullptr) {
			delete p;
		}
		return;
	};
	
	template <typename T>
	inline static auto constexpr free_array = [](T* p)
	{
		if (p != nullptr) {
			delete[] p;
		}
		return;
	};

	template <typename T>
	constexpr std::add_lvalue_reference_t<T> as_lvalue(
		T&& value
	) noexcept 
	{
		return value; 
	}

	template <typename T>
	constexpr std::remove_reference_t<T>&&  as_rvalue(
		T&& value
	) noexcept 
	{
		return std::move(value); 
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