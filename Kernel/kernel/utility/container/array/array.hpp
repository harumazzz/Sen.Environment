#pragma once

#include "kernel/utility/container/array/common.hpp"

namespace Sen::Kernel {

	template <typename T>
	class CArray {

		private:

			using Size = std::size_t;

			template <typename U>
			using Pointer = U*;

		protected:

			Size _size{0};

			Pointer<T> value{ nullptr };

		public:

			constexpr explicit CArray(
				const Size& size
			) : _size{size}, value{new T[size]}
			{

			}

			constexpr explicit CArray(
				Pointer<T> const& source,
				const Size& size
			) : _size{size}, value{source}
			{

			}

			constexpr explicit CArray(

			) = default;

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
			) -> Pointer<T> {
				auto raw = thiz.value;
				thiz.value = nullptr;
				thiz._size = 0;
				return raw;
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
				thiz._size = size;
			}

			~CArray(

			)
			{
				if (thiz.value != nullptr) {
					delete[] value;
					thiz.value = nullptr;
				}
			}

			explicit CArray(
				const CArray& other
			) = delete;

			auto operator =(
				const CArray& other
			) -> CArray& = delete;

			constexpr explicit CArray(
				CArray&& other
			) noexcept : _size{ other._size }, value{ other.value }
			{
				other.value = nullptr;
				other._size = 0;
			}

			constexpr auto operator=(
				CArray&& other
			) noexcept -> CArray& {
				if (this != &other) {
					delete[] value;
					_size = other._size;
					value = other.value;
					other.value = nullptr;
					other._size = 0;
				}
				return *this;
			}

			constexpr auto operator [](
				Size const& index
			) -> T&
			{
				return thiz.value[index];
			}

			auto operator == (
				const CArray& other
			) -> bool const = delete;

			auto operator != (
				const CArray& other
			) -> bool const = delete;

			auto operator < (
				const CArray& other
			) -> bool const = delete;

			auto operator > (
				const CArray& other
			) = delete;

			auto operator <= (
				const CArray& other
			) -> bool const = delete;

			auto operator >= (
				const CArray& other
			) -> bool const = delete;

			constexpr auto size(

			) -> Size
			{
				return thiz._size;
			}
	};

}