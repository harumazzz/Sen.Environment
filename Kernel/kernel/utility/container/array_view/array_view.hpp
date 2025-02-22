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
            Pointer<T> data,
            const std::size_t& size
        ) noexcept : BaseContainer<T>{data, size} {}

        explicit constexpr CArrayView(const CArray<T>& other) noexcept : BaseContainer<T>{other.begin(), other.size()} {

        }

        template <auto N> requires std::is_same_v<type_of<N>, size_t>
        explicit constexpr CArrayView(const std::array<T, N>& other) noexcept : BaseContainer<T>{other.data(), other.size()} {

        }

        constexpr auto subview(
            const std::size_t& offset,
            const std::size_t& count = std::dynamic_extent
        ) const -> CArrayView {
            assert_conditional(offset <= thiz.size(), "Accessed index is larger than the size of the array", "subview");
            const auto new_size = count == std::dynamic_extent || offset + count > thiz.size() ? thiz.size() - offset : count;
            return CArrayView{thiz.begin() + offset, new_size};
        }

        constexpr auto as_bytes(
        ) const noexcept -> CArrayView<uint8_t> {
            return CArrayView<uint8_t>{reinterpret_cast<uint8_t*>(thiz.begin()), thiz.total_bytes()};
        }

        constexpr auto as_writable_bytes(
        ) noexcept -> CArrayView<uint8_t> {
            return CArrayView<uint8_t>(reinterpret_cast<uint8_t*>(thiz.begin()), thiz.total_bytes());
        }


    };

    template <typename Value>
    struct is_array<CArrayView<Value>> : std::true_type {

    };

    template <typename T>
    inline auto operator<<(
        std::ostream& os,
        const CArrayView<T>& array
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

}