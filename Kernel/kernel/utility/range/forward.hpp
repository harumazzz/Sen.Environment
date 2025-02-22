#pragma once

#include "kernel/utility/range/iterator.hpp"

namespace Sen::Kernel {

	template <typename T> requires std::is_arithmetic_v<T>
	class Range {

		public:

			constexpr explicit Range(
				const T& end
			) : start_{0}, end_{end}, step_{1} 
			{

			}

			constexpr explicit Range(
				const T& end,
				const T& step
			) : start_{0}, end_{end}, step_{step} 
			{

			}

			constexpr explicit Range(
				const T& start, 
				const T& end, 
				const T& step
			) : start_{start}, end_{end}, step_{step} 
			{

			}

			constexpr auto begin(

			) noexcept -> Iterator<T>
			{ 
				return Iterator<T>{start_, step_};
			}

			constexpr auto end(

			) noexcept -> Iterator<T>
			{ 
				return Iterator<T>{end_, step_};
			}

			friend auto operator << (
				std::ostream& os,
				const Range& other
			) -> std::ostream& {
				os << "Range(" << other.start_ << ", " << other.end_ << ", " << other.step_ << ")";
				return os;
			}

		private:

			T start_, end_, step_;
	};

}
