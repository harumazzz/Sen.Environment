#pragma once

#include "kernel/utility/container/base/common.hpp"

namespace Sen::Kernel {

    template <typename T>
    class BaseContainer {
    public:
        using Type = T;
        using Size = std::size_t;

        template <typename U>
        using Pointer = U*;
        
        template <typename U>
        using ConstPointer = const Pointer<U>;

        using value_type = Type;

    protected:

        Pointer<T> value{ nullptr };

        Size _size{0};

    public:

        constexpr explicit BaseContainer() {

        }

        constexpr explicit BaseContainer(
            const Pointer<T> value,
            const Size& size
        ) : value{ value }, _size{ size } {}

        BaseContainer(
            const BaseContainer& other
        ) = delete;

        constexpr constexpr BaseContainer(
            BaseContainer&& other
        ) noexcept : value{ other.value }, _size{ other._size } {
            other.value = nullptr;
            other._size = 0;
        }

        auto operator=(
            const BaseContainer& other
        ) -> BaseContainer& = delete;

        constexpr auto operator=(
            BaseContainer&& other
        ) noexcept -> BaseContainer& {
            value = other.value;
            _size = other._size;
            other.value = nullptr;
            other._size = 0;
            return thiz;
        }

        virtual constexpr ~BaseContainer() {

        }

        virtual constexpr auto take_ownership (
            BaseContainer<T>& other
        ) -> void {
            thiz.value = other.value;
            thiz._size = other._size;
            other.value = nullptr;
            other._size = 0;
        }

        static constexpr auto max_size(
        ) -> Size {
            return std::numeric_limits<Size>::max();
        }

        constexpr auto data(

        ) -> Pointer<T>
        {
            return thiz.value;
        }

        constexpr auto begin(
        ) const -> Pointer<T> {
            return thiz.value;
        }

        constexpr auto cbegin(
        ) const -> ConstPointer<T> {
            return thiz.value;
        }

        constexpr auto cend(
        ) const -> ConstPointer<T> {
            return thiz.value + thiz._size;
        }

        constexpr auto rbegin(
        ) const -> Pointer<T> {
            return thiz.value + thiz._size - 1;
        }

        constexpr auto rend(
        ) const -> Pointer<T> {
            return thiz.value + thiz._size - 1;
        }

        constexpr auto rcbegin(
        ) const -> ConstPointer<T> {
            return thiz.value + thiz._size - 1;
        }

        constexpr auto rcend(
        ) const -> ConstPointer<T> {
            return thiz.value + thiz._size - 1;
        }

        virtual constexpr auto release(
        ) -> std::tuple<Pointer<T>, Size> {
            auto raw = thiz.value;
            auto size = thiz._size;
            thiz.value = nullptr;
            thiz._size = 0_size;
            return std::make_tuple(raw, size);
        }

        constexpr auto end(
        ) const -> Pointer<T> {
            return thiz.value + thiz._size;
        }

        virtual constexpr auto operator [](
            Size const& index
        ) -> T&
        {
            assert_index(index < thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("operator[]({})", index));
            return thiz.value[index];
        }

        virtual constexpr auto size(

        ) const -> Size
        {
            return thiz._size;
        }

        constexpr auto empty(
        ) const -> bool {
            return thiz._size == 0;
        }

        constexpr auto front(
        ) const -> Pointer<char> {
            return thiz.value;
        }

        constexpr auto back(

        ) const -> Pointer<char>
        {
            return thiz.value + thiz._size;
        }

        constexpr auto last (
        ) -> T& {
            return thiz.value[thiz._size - 1];
        }

        constexpr auto last_iterator (
        ) -> Pointer<T> {
            return thiz.value + (thiz._size - 1);
        }

        constexpr auto total_bytes (
        ) const -> Size {
            return thiz._size * sizeof(T);
        }


    };

}