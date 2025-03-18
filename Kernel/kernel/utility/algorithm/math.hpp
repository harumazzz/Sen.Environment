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

	inline constexpr auto length_of (
		const char* str
	) -> usize {
		assert(str != nullptr);
		auto s = str;
		while (s != nullptr) {
			++s;
		}
		return static_cast<usize>(s - str);
	}

	template <typename T>
	inline constexpr auto is_greater (
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
	inline constexpr auto is_smaller (
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
	inline constexpr auto is_equal (
		const T& a,
		const T& b
	) -> bool
	{
		if (a == b) {
			return true;
		}
		return false;
	}

	template <typename T>
	inline constexpr auto is_between (
		const T& target,
		const T& a,
		const T& b
	) -> bool
	{
		if (target >= a && target <= b) {
			return true;
		}
		return false;
	}

	template <auto Index, typename F>
	inline constexpr auto apply(
		F&& func
	) -> void {
		func.template operator()<Index>();
	}

	template <typename F, auto... Indices>
	inline constexpr auto generate_each(
		F&& func, 
		std::index_sequence<Indices...>
	) -> void {
		(apply<Indices>(func), ...);
	}

	template <auto N, typename F> requires std::is_same_v<type_of<N>, usize>
	inline constexpr auto iterate_each(F&& func) -> void {
		return generate_each(std::forward<F>(func), std::make_index_sequence<N>{});
	}

	template <auto N, typename Conditional, typename Callable>
	inline constexpr auto match_each(
		const Conditional& condition, 
		Callable&& callback
	) -> void {
		return iterate_each<N>([&]<auto index>() -> void {
			if (index == condition) {
				callback();
			}
		});
	}

	template <auto N, typename F> requires (std::is_same_v<type_of<N>, size_t>)
	inline constexpr auto for_each (
		F&& func
	) -> void {
		if constexpr (N > 0) {
			func();
			for_each<N - 1>(std::forward<F>(func));
		}
	}

	template <typename Container, typename Compare, typename Projection>
	inline constexpr auto sort(
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

	template <typename Container, std::ranges::range R>
	requires std::constructible_from<Container, std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>>
	inline auto to(
		R&& range
	) -> Container {
		return Container{std::ranges::begin(range), std::ranges::end(range)};
	}

	template <auto A, auto B, typename F, auto... Indices>
	inline constexpr auto generate_each_range(
		F&& func,
		std::integer_sequence<std::underlying_type_t<decltype(A)>, Indices...>
	) -> void {
		((Indices >= static_cast<std::underlying_type_t<decltype(A)>>(A) &&
		  Indices < static_cast<std::underlying_type_t<decltype(B)>>(B)
		  ? apply<Indices>(func)
		  : void()), ...);
	}


	template <auto A, auto B, typename F>
	inline constexpr auto for_each_range(
		F&& func
	) -> void {
		using Integer = std::underlying_type_t<decltype(A)>;
		constexpr auto start = static_cast<Integer>(A);
		constexpr auto end = static_cast<Integer>(B);
		static_assert(end >= start, "B must be greater than or equal to A");
		return generate_each_range<A, B>(
			std::forward<F>(func),
			std::make_integer_sequence<Integer, end - start>{}
		);
	}


	template <auto A, auto B, typename Conditional, typename Callable>
	inline constexpr auto match_range(
		const Conditional& condition,
		Callable&& callback
	) -> void {
		return for_each_range<A, B>([&]<auto index>() -> void {
			if (index == static_cast<std::underlying_type_t<Conditional>>(condition)) {
				callback.template operator()<index>();
			}
		});
	}

}