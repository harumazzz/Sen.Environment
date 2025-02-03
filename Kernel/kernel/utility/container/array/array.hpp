#pragma once

#include "kernel/utility/container/array/common.hpp"
#include "kernel/utility/container/base_container/base.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel {

	template <typename T>
	class CArray : public BaseContainer<T> {

		public:

			using Size = std::size_t;

			template <typename U>
			using Pointer = U*;

		public:

			constexpr explicit CArray(
				const Size& size
			) : BaseContainer<T>{new T[size], size}
			{

			}

			constexpr explicit CArray(
				Pointer<T> const& source,
				const Size& size
			) : BaseContainer<T>{source, size}
			{

			}

			constexpr explicit CArray(

			) = default;

			~CArray(

			) override
			{
				if (thiz.value != nullptr) {
					delete[] thiz.value;
					thiz.value = nullptr;
				}
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
					delete[] thiz.value;
					thiz._size = other._size;
					thiz.value = other.value;
					other.value = nullptr;
					other._size = 0;
				}
				return thiz;
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

			auto clone (

			) -> CArray
			{
				auto new_instance = new T[thiz._size];
				if constexpr (is_numeric_v<T>) {
					std::memcpy(new_instance, thiz.value, thiz._size * sizeof(T));
				}
				else {
					std::memmove(new_instance, thiz.value, thiz._size * sizeof(T));
				}
				return CArray{new_instance, thiz._size};
			}

			auto assign (
				CArray& other
			) -> void {
				if (thiz.value != nullptr) {
					delete[] thiz.value;
				}
				thiz.value = other.value;
				thiz._size = other._size;
				other.value = nullptr;
				other._size = 0;
			}
	};

}
