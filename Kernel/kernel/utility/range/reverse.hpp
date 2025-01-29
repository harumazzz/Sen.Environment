#pragma once

#include <type_traits>

namespace Sen::Kernel {

	template <typename T> requires std::is_arithmetic_v<T>
	class ReverseRange {

	protected:
		class iterator {
			T i;
			T step;

		public:
			constexpr iterator(const T& start, const T& step) noexcept
				: i{ start }, step{ step } {
			}

			constexpr auto operator!=(const iterator& that) const noexcept -> bool {
				return (step > 0) ? (i < that.i) : (i > that.i);
			}

			constexpr auto operator++() noexcept -> const iterator& {
				i += step;
				return *this;
			}

			constexpr auto operator*() const noexcept -> T {
				return i;
			}
		};

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

		constexpr auto begin() const noexcept -> iterator {
			return iterator{ end_, -step_ };
		}

		constexpr auto end() const noexcept -> iterator {
			return iterator{ start_, -step_ };
		}

	private:
		T start_, end_, step_;
	};

}
