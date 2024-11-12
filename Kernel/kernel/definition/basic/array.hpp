#pragma once

#include "kernel/definition/library.hpp"
#include "kernel/definition/assert.hpp"
#include "kernel/definition/macro.hpp"

namespace Sen::Kernel {

	// Vector -> List

	template <typename T>
	using List = std::vector<T>;

	// Array -> Array

	template <typename T, auto size>
	using Array = std::array<T, size>;
	
	template <typename T> requires std::is_integral<T>::value
	inline static auto _make_list (
		T* data, 
		std::size_t size, 
		std::function<void(T* data)> finalizer
	) -> List<T> 
	{
		auto result = _make_list(data, size);  
		finalizer(data);
		return result;
	}

	template <typename T> requires std::is_integral<T>::value
	inline static auto _make_list (
		T* data, 
		std::size_t size
	) -> List<T> 
	{
		auto result = List<T>{};
		result.assign(data, data + size);  
		return result;
	}

	template <typename T>
	inline static auto make_list(
		T* data, 
		std::size_t size,
		std::function<void(T* data)> finalizer
	) -> List<T>
	{
		if constexpr (std::is_same<T, char>::value or std::is_same<T, uint8_t>::value) {
			return _make_list(data, size, finalizer);
		}
		else {
			auto result = make_list(data, size);
			finalizer(data);
			return result;
		}
	}

	template <typename T>
	inline static auto make_list(
		T* data, 
		std::size_t size
	) -> List<T>
	{
		if constexpr (std::is_same<T, char>::value or std::is_same<T, uint8_t>::value) {
			return _make_list(data, size);
		}
		else {
			auto result = List<T>{};
			result.reserve(size);
			for (auto i : Range<std::size_t>(size)) {
				result.push_back(data[i]);  
			}
			return result;
		}
	}

}