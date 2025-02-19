#pragma once

#include "kernel/utility/library.hpp"

namespace Sen::Kernel::Math {

	using ::acos;

    using ::asin;

    using ::atan;

    using ::atan2;

    using ::ceil;

    using ::cos;

    using ::cosh;

    using ::exp;

    using ::fabs;

    using ::floor;

    using ::fmod;

    using ::frexp;

    using ::ldexp;

    using ::log;

    using ::log10;

    using ::modf;

    using ::pow;

    using ::sin;

    using ::sinh;

    using ::sqrt;

    using ::tan;

    using ::tanh;

	using std::log2;

	using ::log2;

	using std::min;

	using std::max;

	using std::rotl;

	using std::rotr;


	template <typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	inline auto compare(
		const T & a,
		const T & b
	) -> T
	{
		if(a >= b){
			return a;
		}
		return b;
	}

	constexpr auto length_of (
		const char* str
	) -> usize {
		auto s = str;
		while (*s) {
			++s;
		}
		return static_cast<usize>(s - str);
	}

	template <typename T>
	constexpr static auto is_greater (
		const T& a,
		const T& b
	) -> bool
	{
		if (a >= b) {
			return false;
		}
		return true;
	}

	template <typename T>
	constexpr static auto is_smaller (
		const T& a,
		const T& b
	) -> bool
	{
		if (a > b) {
			return true;
		}
		return false;
	}

	template <typename T>
	constexpr static auto is_equal (
		const T& a,
		const T& b
	) -> bool
	{
		if (a == b) {
			return true;
		}
		return false;
	}

	template <typename Container, typename Compare, typename Projection>
	constexpr static auto sort(
		Container& arr,
		Compare&& comp,
		Projection&& proj
	) -> Container {
		constexpr auto size = array_size_v<Container>;
		for (auto i = size_t{0}; i < size - 1; ++i) {
			auto min_index = i;
			for (auto j = i + 1; j < size; ++j) {
				if (comp(proj(arr[j]), proj(arr[min_index]))) {
					min_index = j;
				}
			}
			if (min_index != i) {
				std::swap(arr[i], arr[min_index]);
			}
		}
		return arr;
	}
}