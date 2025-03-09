#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/trait/utility.hpp"

namespace Sen::Kernel {

    template<typename T>
    struct Allocator;

    template<typename T>
    struct Allocator {
        inline constexpr static auto allocate(const usize& size) -> T* {
            return new T[size];
        }

        inline constexpr static auto deallocate(T* ptr) -> void {
            delete[] ptr;
        }

        inline constexpr static auto copy(T* dest, const T* src, const usize& size) -> void {
            std::uninitialized_copy(src, src + size, dest);
        }

        inline constexpr static auto move(T* dest, T* src, const usize& size) -> void {
            std::uninitialized_move(src, src + size, dest);
        }
    };

    template<typename T> requires is_trivially_copyable_v<T>
    struct Allocator<T> {
        inline constexpr static auto allocate(const usize& size) -> T* {
            return static_cast<T*>(std::malloc(size * sizeof(T)));
        }

        inline constexpr static auto deallocate(T* ptr) -> void {
            std::free(ptr);
        }

        inline constexpr static auto copy(T* dest, const T* src, const usize& size) -> void {
            std::memcpy(dest, src, size * sizeof(T));
        }

        inline constexpr static auto move(T* dest, T* src, const usize& size) -> void {
            std::memcpy(dest, src, size * sizeof(T));
        }
    };

}
