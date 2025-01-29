#pragma once

#include <type_traits>

namespace Sen::Kernel {

	template <typename T> requires std::is_arithmetic<T>::value
	class Range {

		protected:

			class iterator {

				T i;

				T step;

			public:

				constexpr iterator(
					const T& start, 
					const T& step
				) noexcept : i{start}, step{step} 
				{

				}

				constexpr auto operator != (
					const iterator& that
				) const noexcept -> bool 
				{ 
					return i != that.i; 
				}

				constexpr auto operator++ (

				) noexcept -> const iterator& 
				{ 
					this->i += step; 
					return *this; 
				}

				constexpr auto operator* (

				) noexcept -> T 
				{ 
					return this->i; 
				}
			};

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

			) noexcept  -> iterator 
			{ 
				return iterator{start_, step_}; 
			}

			constexpr auto end(

			) noexcept -> iterator 
			{ 
				return iterator{end_, step_}; 
			}

		private:

			T start_, end_, step_;
	};

}
