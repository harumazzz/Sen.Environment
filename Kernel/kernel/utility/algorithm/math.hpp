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
}