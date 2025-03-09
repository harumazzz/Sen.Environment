#pragma once

#include "kernel/utility/container/base/base.hpp"
#include "kernel/utility/trait/container.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel {

	template <typename T, auto size>
	using Array = std::array<T, size>;

	template <typename T>
	class CArray : public BaseContainer<T> {

		public:

			using Size = std::size_t;

			template <typename U>
			using Pointer = U*;

			using Alloc = Allocator<T>;

		private:

			constexpr explicit CArray(
				const std::span<T>& init
			) : BaseContainer<T>{Alloc::allocate(init.size()), init.size()} {
				Alloc::copy(thiz.value, init.data(), init.size());
			}

		public:

			constexpr explicit CArray(
				const Size& size
			) : BaseContainer<T>{Alloc::allocate(size), size}
			{

			}

			constexpr explicit CArray(
				Pointer<T> const& source,
				const Size& size
			) : BaseContainer<T>{source, size}
			{

			}

			template <typename Iterator>
			constexpr explicit CArray(Iterator first, Iterator last)
			: BaseContainer<T>{Alloc::allocate(std::distance(first, last)), static_cast<Size>(std::distance(first, last))} {
				std::move(first, last, thiz.value);
			}


			constexpr explicit CArray(

			) = default;

			constexpr ~CArray(

			) override
			{
				if (thiz.value != nullptr) {
					Alloc::deallocate(thiz.value);
					thiz.value = nullptr;
				}
			}

			[[nodiscard]] static constexpr auto sizeof_value() noexcept -> Size {
				return sizeof(T);
			}

	        constexpr auto allocate(
	            Size const& size
	        ) -> void
	        {
	            if (thiz.value != nullptr) {
	            	Alloc::deallocate(thiz.value);
	            }
	            thiz.value = new T[size];
	            thiz._size = size;
	        }

			constexpr auto operator [](
				Size const& index
			) const -> T&
    		{
    			assert_index(index < thiz._size, fmt::format("Accessed index is larger than the size of the array"), fmt::format("operator[]({})", index));
    			return thiz.value[index];
    		}

			explicit CArray(
				const CArray& other
			) = delete;

			auto operator =(
				const CArray& other
			) -> CArray& = delete;

			constexpr CArray(
				CArray&& other
			) noexcept : BaseContainer<T>{std::move(other)}
			{
			}

			constexpr auto operator=(
				CArray&& other
			) noexcept -> CArray& {
				if (this != &other) {
					Alloc::deallocate(thiz.value);
					thiz._size = other._size;
					thiz.value = other.value;
					other.value = nullptr;
					other._size = 0;
				}
				return thiz;
			}

			auto operator == (
				const CArray& other
			) -> bool = delete;

			auto operator != (
				const CArray& other
			) -> bool = delete;

			auto operator < (
				const CArray& other
			) -> bool = delete;

			auto operator > (
				const CArray& other
			) = delete;

			auto operator <= (
				const CArray& other
			) -> bool = delete;

			auto operator >= (
				const CArray& other
			) -> bool = delete;

			constexpr auto take_ownership (
				CArray& other
			) -> void {
				thiz.value = other.value;
				thiz._size = other._size;
				other.value = nullptr;
				other._size = 0;
			}

			template <typename U> requires std::is_base_of_v<BaseContainer<extract_container_t<U>>, U>
			constexpr auto take_ownership (
				U& other
			) -> void {
				thiz.value = other.value;
				thiz._size = other._size;
				other.value = nullptr;
				other._size = 0;
			}

			explicit operator CArrayView<T>(
			) const {
				return CArrayView<T>{ thiz.value, thiz._size };
			}

			constexpr auto view (
			) -> CArrayView<T> {
				return CArrayView<T>{ thiz.value, thiz._size };
			}

			constexpr auto assign (
				CArray& other
			) -> void {
				if (thiz.value != nullptr) {
					Alloc::deallocate(thiz.value);
				}
				thiz.value = other.value;
				thiz._size = other._size;
				other.value = nullptr;
				other._size = 0;
			}

			constexpr auto assign (
				CList<T>& other
			) -> void {
				if (thiz.value != nullptr) {
					Alloc::deallocate(thiz.value);
				}
				thiz.value = other.value;
				thiz._size = other._capacity;
				other.value = nullptr;
				other._size = 0;
				other._capacity = 0;
			}

			friend class CList<T>;

			constexpr static auto make_array (
				const std::span<T>& init
			) -> CArray {
				return CArray{init};
			}

			constexpr auto clear (
			) -> void {
				Alloc::deallocate(thiz.value);
				thiz.value = nullptr;
				thiz._size = 0_size;
			}
	};

	template <typename Value>
	struct is_array<CArray<Value>> : std::true_type {

	};

	template <typename T>
	inline auto operator<<(
		std::ostream& os,
		const CArray<T>& array
	) -> std::ostream& {
		os << "[";
		for (auto index : Range{array.size()}) {
			os << array[index];
			if (index < array.size() - 1) {
				os << ", ";
			}
		}
		os << "]";
		return os;
	}

	template <typename T, typename... Args>
	inline constexpr auto make_array (
		Args&&... args
	) -> CArray<T> {
		auto value = CArray<T>{};
		value.allocate(sizeof...(args));
		{
			auto index = 0;
			((value[index++] = std::forward<Args>(args)), ...);
		}
		return value;
	}

}