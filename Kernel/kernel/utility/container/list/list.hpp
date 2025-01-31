#pragma once

#include "kernel/utility/container/list/common.hpp"

namespace Sen::Kernel {

    template<typename T>
    class CList;

    template <typename T>
	class CList : public Common {

		private:

    		using Common = Common;


		public:

			using Size = std::size_t;

			template <typename U>
			using Pointer = U*;

		protected:

			Pointer<T> value{ nullptr };

			Size _size{};

    		Size _capacity{};

		public:

			constexpr explicit CList(
				const Size& size
			) : value{new T[size]}, _size{0_size}, _capacity{size}
			{

			}

			constexpr explicit CList(
				Pointer<T> const& source,
				const Size& size
			) : value{source}, _size{size}, _capacity{size}
			{

			}

			constexpr explicit CList(

			) : value{new T[64_size]}, _size{0_size}, _capacity{64_size}
    		{

			}

			constexpr auto data(

			) -> Pointer<T>&
			{
				return thiz.value;
			}

			constexpr auto begin(
			) -> Pointer<T> {
				return thiz.value;
			}

			constexpr auto rbegin(
			) -> Pointer<T> {
				return thiz.value + thiz._size - 1;
			}

			constexpr auto cbegin(
			) -> Pointer<T> {
				return thiz.value;
			}

			constexpr auto cend(
			) -> Pointer<T> {
				return thiz.value + thiz._size;
			}

			constexpr auto rend(
			) -> Pointer<T> {
				return thiz.value + thiz._size - 1;
			}

			constexpr auto release(
			) -> std::tuple<Pointer<T>, Size> {
				auto raw = thiz.value;
				auto size = thiz._size;
				thiz.value = nullptr;
				thiz._size = 0_size;
				thiz._capacity = 0_size;
				return std::make_tuple(raw, size);
			}

			constexpr auto end(
			) -> Pointer<T> {
				return thiz.value + thiz._size;
			}

			auto allocate(
				Size const& size
			) -> void
			{
				if (thiz.value != nullptr) {
					delete[] thiz.value;
				}
				thiz.value = new T[size];
				thiz._size = 0_size;
				thiz._capacity = size;
			}

			~CList(

			)
			{
				if (thiz.value != nullptr) {
					delete[] value;
					thiz.value = nullptr;
				}
			}

			explicit CList(
				const CList& other
			) = delete;

			auto operator =(
				const CList& other
			) -> CList& = delete;

			constexpr CList(
				CList&& other
			) noexcept : value{ other.value }, _size{ other._size }, _capacity{other._capacity}
			{
				other.value = nullptr;
				other._size = 0;
				other._capacity = 0;
			}

			constexpr auto operator=(
				CList&& other
			) noexcept -> CList& {
				if (this != &other) {
					delete[] value;
					thiz._size = other._size;
					thiz.value = other.value;
					thiz._capacity = other._capacity;
					other.value = nullptr;
					other._size = 0;
					other._capacity = 0;
				}
				return thiz;
			}

			constexpr auto operator [](
				Size const& index
			) -> T&
			{
				assert_conditional(index < thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("access_index{}", index));
				return thiz.value[index];
			}

			auto operator == (
				const CList& other
			) -> bool const = delete;

			auto operator != (
				const CList& other
			) -> bool const = delete;

			auto operator < (
				const CList& other
			) -> bool const = delete;

			auto operator > (
				const CList& other
			) = delete;

			auto operator <= (
				const CList& other
			) -> bool const = delete;

			auto operator >= (
				const CList& other
			) -> bool const = delete;

			constexpr auto size(

			) -> Size
			{
				return thiz._size;
			}

    	constexpr auto size(
			const Size& new_size
		) -> void
		{
			assert_conditional(new_size <= thiz._capacity, "Size must be smaller than current capacity", "size");
			thiz._size = new_size;
		}

		    constexpr auto capacity(

			) -> Size
			{
				return thiz._capacity;
			}

			auto clone (

			) -> CList<T>
			{
				auto new_instance = new T[thiz._size];
				std::memcpy(new_instance, thiz.value, thiz._size);
				return CList<T>{new_instance, thiz._size};
			}

    		auto clear(

			) -> void {
				thiz._size = 0;
				thiz._capacity = 0;
				if (thiz.value != nullptr) {
					delete[] thiz.value;
				}
				thiz.value = nullptr;
			}

    		auto reallocate (
				const Size& size
			) -> void {
				if (thiz._capacity < size) {
					auto new_value = new T[size];
					std::memmove(new_value, thiz.value, thiz._size * sizeof(T));
					delete[] thiz.value;
					thiz.value = new_value;
					thiz._capacity = size;
				}
			}

    		auto append (
    			T&& value
    		) -> void {
				if (thiz._size >= thiz._capacity) {
					thiz.reallocate(thiz._capacity * 4);
				}
				thiz.value[thiz._size] = std::move(value);
				++thiz._size;
			}

    		auto append (
				const T& value
			) -> void {
				if (thiz._size >= thiz._capacity) {
					thiz.reallocate(thiz._capacity * 4);
				}
				thiz.value[thiz._size] = value;
				++thiz._size;
			}

    		auto assign (
				CArray<T>& other
			) -> void {
				if (thiz.value != nullptr) {
					delete[] thiz.value;
				}
				thiz.value = other.value;
				thiz._size = other._size;
				thiz._capacity = other._size;
				other.value = nullptr;
				other._size = 0;
			}

    		auto assign (
				CList& other
			) -> void {
				if (thiz.value != nullptr) {
					delete[] thiz.value;
				}
				thiz.value = other.value;
				thiz._size = other._size;
				thiz._capacity = other._size;
				other.value = nullptr;
				other._size = 0;
				other._capacity = 0;
			}

    		template <typename... Args> requires (std::is_same_v<Args, Size> && ... )
    		auto pop(
    			 const Args&... args
			) -> void {
				assert_conditional(thiz._size > 0, fmt::format("No argument to pop, the current list is empty"), "pop");
				if constexpr (sizeof...(args) == 0) {
					--thiz._size;
				}
				else {
					static_assert(sizeof...(args) == 1, "Expected 1 argument only");
					auto index = (std::forward<Args>(args), ...);
					assert_conditional(index < _size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("pop", index));
					std::memmove(thiz.value + index, thiz.value + index + 1, (thiz._size - index - 1) * sizeof(T));
					--thiz._size;
				}
			}

    		auto insert(
				const Size& index,
				T&& element
			) -> void {
				assert_conditional(index <= thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("insert", index));
				if (thiz._size >= thiz._capacity) {
					thiz.reallocate(_capacity * 2);
				}
				std::memmove(thiz.value + index + 1, thiz.value + index, (thiz._size - index) * sizeof(T));
				thiz.value[index] = std::move(element);
				++thiz._size;
			}
	};

}