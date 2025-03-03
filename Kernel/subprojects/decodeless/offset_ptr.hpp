// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#ifndef ENABLE_OFFSET_PTR_TRANSLATE
    #define ENABLE_OFFSET_PTR_TRANSLATE 0
#endif

namespace decodeless {

// Pointer with storage relative to its current memory location
template <class T> class offset_ptr {
public:
    using value_type = T;
    using offset_type = std::ptrdiff_t;
    using size_type = std::uint64_t;
    offset_ptr() = default;
    ~offset_ptr() noexcept = default;
    offset_ptr(const offset_ptr& other) : offset_ptr(other.get()) {}
    offset_ptr(offset_ptr&& other) noexcept : offset_ptr(other.get()) {}
    offset_ptr(T* ptr) { set(ptr); }
    [[nodiscard]] T* get() const {
        return m_offset == NullValue ? nullptr
                                     : reinterpret_cast<T*>(base_() + m_offset);
    }
    void set(T* ptr) {
        m_offset = (ptr == nullptr)
                       ? NullValue
                       : (reinterpret_cast<offset_type>(ptr) - base_());
    }
    [[nodiscard]] bool operator==(const offset_ptr& other) const {
        return get() == other.get();
    }
    [[nodiscard]] bool operator!=(const offset_ptr& other) const {
        return get() != other.get();
    }
    offset_ptr& operator=(const offset_ptr& other) {
        set(other.get());
        return *this;
    }
    offset_ptr& operator=(offset_ptr&& other) noexcept {
        set(other.get());
        return *this;
    }
    [[nodiscard]] T& operator[](size_type pos) const { return get()[pos]; }
    [[nodiscard]] T& operator*() const { return *get(); }
    [[nodiscard]] T* operator->() const { return get(); }
    [[nodiscard]] operator bool() const { return m_offset != NullValue; }

#if ENABLE_OFFSET_PTR_TRANSLATE
    [[nodiscard]] T* translate(const void* srcBase, void* dstBase) const {
        return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(get()) -
                                    reinterpret_cast<uintptr_t>(srcBase) +
                                    reinterpret_cast<uintptr_t>(dstBase));
    }
    [[nodiscard]] const T* translate(const void* srcBase,
                                     const void* dstBase) const {
        return reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(get()) -
                                          reinterpret_cast<uintptr_t>(srcBase) +
                                          reinterpret_cast<uintptr_t>(dstBase));
    }
#endif

private:
    [[nodiscard]] offset_type base_() const {
        return reinterpret_cast<offset_type>(this);
    }

    // Considered null when the offset is one byte, to allow pointing to self.
    static const offset_type NullValue = 1;

    offset_type m_offset = NullValue;
};

} // namespace decodeless
