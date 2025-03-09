#pragma once

#include "kernel/utility/container/allocator.hpp"

namespace Sen::Kernel {

    template <typename T>
    class BaseContainer;

    class BasicString;

    template <typename T>
    class CArray;

    template <typename T>
    class CList;

    template <typename T>
    class CArrayView;

    template <typename T>
    class CListView;

    struct StringView;

}

namespace std {

    template <typename T, size_t size>
    inline auto operator<<(
        std::ostream& os,
        const std::array<T, size>& array
    ) -> std::ostream& {
        os << "[";
        for (auto index = size_t{0}; index < array.size(); ++index) {
            os << array[index];
            if (index < array.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

}