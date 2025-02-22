#pragma once

#include "kernel/utility/range/iterator.hpp"

namespace Sen::Kernel {

	template <typename T> requires std::is_arithmetic_v<T>
	class ReverseRange {

		public:

			constexpr explicit ReverseRange(const T& end) noexcept
				: start_{ 0 }, end_{ end }, step_{ 1 } {
			}

			constexpr explicit ReverseRange(const T& end, const T& step) noexcept
				: start_{ 0 }, end_{ end }, step_{ step } {
			}

			constexpr explicit ReverseRange(const T& start, const T& end, const T& step) noexcept
				: start_{ start }, end_{ end }, step_{ step } {
			}

			constexpr auto begin() const noexcept -> Iterator<T> {
				return Iterator<T>{ end_, -step_ };
			}

			constexpr auto end() const noexcept -> Iterator<T> {
				return Iterator<T>{ start_, -step_ };
			}

			friend auto operator << (
				std::ostream& os,
				const ReverseRange& other
			) -> std::ostream& {
				os << "ReverseRange(" << other.start_ << ", " << other.end_ << ", " << other.step_ << ")";
				return os;
			}

		private:
			T start_, end_, step_;
	};

}
