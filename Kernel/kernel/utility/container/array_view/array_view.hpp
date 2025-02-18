#pragma once

#include "kernel/utility/container/base/base.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel {

    template <typename T>
    class CArrayView : public BaseContainer<T> {

    public:

        constexpr CArrayView(
        ) noexcept : BaseContainer<T>{nullptr, 0_size} {}

        constexpr CArrayView(
            const T* &data,
            const std::size_t& size
        ) noexcept : BaseContainer<T>{data, size} {}

        explicit constexpr CArrayView(const CArray<T>& other) noexcept : BaseContainer<T>{other.cbegin(), other.size()} {

        }

        template <auto N> requires std::is_same_v<type_of<N>, size_t>
        explicit constexpr CArrayView(const std::array<T, N>& other) noexcept : BaseContainer<T>{other.data(), other.size()} {

        }

        constexpr auto subview(
            const std::size_t& offset,
            const std::size_t& count = std::dynamic_extent
        ) const -> CArrayView {
            assert_conditional(offset <= thiz.size(), "Accessed index is larger than the size of the array", "subview");
            auto new_size = count == std::dynamic_extent || offset + count > thiz.size() ? thiz.size() - offset : count;
            return CArrayView{thiz.data() + offset, new_size};
        }

        constexpr auto as_bytes(
        ) const noexcept -> CArrayView<uint8_t> {
            return CArrayView<uint8_t>{reinterpret_cast<uint8_t*>(thiz.data()), thiz.total_bytes()};
        }

        constexpr auto as_writable_bytes(
        ) noexcept -> CArrayView<uint8_t> {
            return CArrayView<uint8_t>(reinterpret_cast<uint8_t*>(thiz.data()), thiz.total_bytes());
        }


    };

}