#pragma once

#include "kernel/utility/container/base/base.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel {

    template <typename T>
    class CListView : public BaseContainer<T> {

     protected:

       size_t _capacity;

    public:

        constexpr CListView(
        ) noexcept : BaseContainer<T>{nullptr, 0_size}, _capacity{0_size} {
        }

        constexpr CListView(
            const T* &data,
            const std::size_t& size
        ) noexcept : BaseContainer<T>{data, size}, _capacity{size} {}

        constexpr CListView(
            const T* &data,
            const std::size_t& size,
            const std::size_t& capacity
        ) noexcept : BaseContainer<T>{data, size}, _capacity{capacity} {}

        explicit constexpr CListView(const CList<T>& other) noexcept : BaseContainer<T>{other.begin(), other.size()}, _capacity{other.capacity()} {

        }

        constexpr auto subview(
            const std::size_t& offset,
            const std::size_t& count = std::dynamic_extent
        ) const -> CListView {
            assert_conditional(offset <= thiz.size(), "Accessed index is larger than the size of the list", "subview");
            auto new_size = count == std::dynamic_extent || offset + count > thiz.size() ? thiz.size() - offset : count;
            return CListView{thiz.data() + offset, new_size};
        }

        [[nodiscard]] constexpr auto as_bytes(
        ) const noexcept -> CListView<uint8_t> {
            return CListView<uint8_t>{reinterpret_cast<uint8_t*>(thiz.data()), thiz.total_bytes()};
        }

        constexpr auto as_writable_bytes(
        ) noexcept -> CListView<uint8_t> {
            return CListView<uint8_t>(reinterpret_cast<uint8_t*>(thiz.data()), thiz.total_bytes());
        }


    };

}