#pragma once

#include "kernel/utility/container/base/base.hpp"
#include "kernel/utility/container/list/common.hpp"

namespace Sen::Kernel {

    template<typename T>
    class CList;

    template <typename T>
	class CList : public Common, public BaseContainer<T> {

		protected:

    		using Common = Common;


		public:

			using Size = std::size_t;

			template <typename U>
			using Pointer = U*;

		protected:

    		Size _capacity{};

		public:

			constexpr explicit CList(
				const Size& size
			) : BaseContainer<T>{new T[size], size}, _capacity{size}
			{

			}

			constexpr explicit CList(
				Pointer<T> const& source,
				const Size& size
			) : BaseContainer<T>{source, size}, _capacity{size}
			{

			}

			constexpr explicit CList(

			) : BaseContainer<T>{new T[64_size], 0_size}, _capacity{64_size}
    		{

			}

    		explicit constexpr CList(
				const std::span<T>& init
			) : BaseContainer<T>{new T[init.size()], init.size()}, _capacity{init.size()} {
				std::uninitialized_copy(init.begin(), init.end(), thiz.value);
			}

    		constexpr auto release(
			) -> std::tuple<Pointer<T>, Size> override {
				auto raw = thiz.value;
				auto size = thiz._size;
				thiz.value = nullptr;
				thiz._size = 0_size;
				thiz._capacity = 0_size;
				return std::make_tuple(raw, size);
			}

		    auto allocate(
				Size const& size
			) -> void override
			{
				if (thiz.value != nullptr) {
					delete[] thiz.value;
				}
				thiz.value = new T[size];
				thiz._size = 0_size;
				thiz._capacity = size;
			}

			~CList(

			) override {
				if (thiz.value != nullptr) {
					delete[] thiz.value;
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
			) noexcept : BaseContainer<T>{std::move(other)}, _capacity{other._capacity}
			{
				other._capacity = 0;
			}

			constexpr auto operator=(
				CList&& other
			) noexcept -> CList& {
				if (this != &other) {
					delete[] thiz.value;
					thiz._size = other._size;
					thiz.value = other.value;
					thiz._capacity = other._capacity;
					other.value = nullptr;
					other._size = 0;
					other._capacity = 0;
				}
				return thiz;
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

    		constexpr auto take_ownership (
				CList& other
			) -> void {
    			thiz.value = other.value;
    			thiz._size = other._size;
    			other.value = nullptr;
    			other._size = 0;
    		}

    		constexpr auto take_ownership (
				CArray<T>& other
			) -> void {
    			thiz.value = other.value;
    			thiz._size = other._size;
    			other.value = nullptr;
    			other._size = 0;
    		}

		    constexpr auto capacity(

			) -> Size
			{
				return thiz._capacity;
			}

    		constexpr auto size(

			) const -> Size override {
				return thiz._size;
			}

    		constexpr auto size(
				const Size& new_size
			) -> void
		    {
		    	assert_conditional(new_size <= thiz._capacity, "Size must be smaller than current capacity", "size");
		    	thiz._size = new_size;
		    }

    		auto clear(

			) -> void override {
				thiz._capacity = 0;
		    	BaseContainer<T>::clear();
			}

    		auto reallocate (
				const Size& size
			) -> void {
				if (thiz._capacity < size) {
					auto new_value = new T[size];
					if constexpr (is_numeric_v<T>) {
						std::memcpy(new_value, thiz.value, thiz._size * sizeof(T));
					}
					else {
						std::memmove(new_value, thiz.value, thiz._size * sizeof(T));
					}
					delete[] thiz.value;
					thiz.value = new_value;
					thiz._capacity = size;
				}
			}

    		template <typename U>
    		auto append (
    			U&& value
    		) -> void {
				if (thiz._size >= thiz._capacity) {
					thiz.reallocate(thiz._capacity * 4);
				}
				thiz.value[thiz._size] = std::forward<T>(value);
				++thiz._size;
			}

    		auto append (
    		) -> void {
    			if (thiz._size >= thiz._capacity) {
    				thiz.reallocate(thiz._capacity * 4);
    			}
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
					auto index = std::get<0>(std::make_tuple<>(args...));
					assert_conditional(index < thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("pop", index));
					if constexpr (is_numeric_v<T>) {
						std::memcpy(thiz.value + index, thiz.value + index + 1, (thiz._size - index - 1) * sizeof(T));
					}
					else {
						std::memmove(thiz.value + index, thiz.value + index + 1, (thiz._size - index - 1) * sizeof(T));
					}
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
				if constexpr (is_numeric_v<T>) {
					std::memcpy(thiz.value + index + 1, thiz.value + index, (thiz._size - index) * sizeof(T));
				}
		    	else {
		    		std::memmove(thiz.value + index + 1, thiz.value + index, (thiz._size - index) * sizeof(T));
		    	}
				thiz.value[index] = std::move(element);
				++thiz._size;
			}

    		friend class CArray<T>;

    		static auto make_list (
				const std::span<T>& init
			) -> CList {
    			return CList{init};
    		}
	};

}