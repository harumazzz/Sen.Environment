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

			explicit CArray(
				const Size& size
			) : _size{size}, value{new T[size]}
			{

			}

			explicit CArray(

			) = default;

			auto data(

			) -> Pointer<T>&
			{
				return thiz.value;
			}

			auto begin(
			) -> Pointer<T> {
				return thiz.value;
			}

			auto end(
			) -> Pointer<T> {
				return thiz.value + thiz._size;
			}

			auto allocate(
				Size const& size
			) -> void
			{
				if (thiz.value != nullptr) {
					delete[] thiz.value;
					thiz.value = new T[size];
					thiz._size = size;
				}
			}

			~CArray(

			)
			{
				delete[] value;
			}

			explicit CArray(
				const CArray& other
			) = delete;

			auto operator =(
				const CArray& other
			) -> CArray& = delete;

			explicit CArray(
				CArray&& other
			) noexcept : _size{ other._size }, value{ other.value }
			{
				other.value = nullptr;
				other._size = 0;
			}

			auto operator=(
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

			auto operator [](
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

			auto size(

			) -> Size
			{
				return thiz._size;
			}
	};

}