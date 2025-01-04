#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel {

	// free c memory allocation

	inline static auto constexpr free_c_memory = [](auto p)
	{
		free(p);
		return;
	};

	// free c++ memory allocation

	inline static auto constexpr free_memory = [](auto p)
	{
		delete p;
		return;
	};

	// free c++ memory allocation

	inline static auto constexpr free_array = [](auto p)
	{
		delete[] p;
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
}