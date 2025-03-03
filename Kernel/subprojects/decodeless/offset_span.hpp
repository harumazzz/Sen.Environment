// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <cstdint>
#include "./offset_ptr.hpp"
#include <type_traits>

#if __has_include(<span>)
    #include <span>
#endif

#if __has_include(<ranges>)
    #include <ranges>
#endif

#ifndef ENABLE_OFFSET_PTR_TRANSLATE
    #define ENABLE_OFFSET_PTR_TRANSLATE 0
#endif

namespace decodeless {

// std::span equivalent but implemented with an offset_ptr
// Uses a fixed x64 size type for binary stability in files. May conflict with
// std::span conversion on x32 systems
template <class T> class offset_span {
public:
    using value_type = T;
    using size_type = std::uint64_t;
    using iterator = T*;
    using reference = T&;
    offset_span() = default;
    offset_span(T* ptr, size_type count) : m_data(ptr), m_size(count) {}

#ifdef __cpp_lib_span
    operator std::span<T>() const { return {m_data.get(), m_size}; }
    [[nodiscard]] std::span<T> subspan(size_type offset) const {
        return {m_data.get() + offset, m_size - offset};
    }
    #if ENABLE_OFFSET_PTR_TRANSLATE
    [[nodiscard]] std::span<T> translate(const void* srcBase,
                                         void* dstBase) const {
        return {m_data.translate(srcBase, dstBase), m_size};
    }
    [[nodiscard]] std::span<const T> translate(const void* srcBase,
                                               const void* dstBase) const {
        return {m_data.translate(srcBase, dstBase), m_size};
    }
    #endif
#endif

#ifdef __cpp_lib_ranges
    template <std::ranges::contiguous_range Range>
    offset_span(Range&& range)
        : offset_span(std::ranges::data(range), std::ranges::size(range)) {}
    template <std::ranges::contiguous_range Range>
    offset_span& operator=(Range&& range) {
        return *this = offset_span(std::ranges::data(range),
                                   std::ranges::size(range));
    }
#endif

    [[nodiscard]] iterator data() const { return m_data.get(); }
    [[nodiscard]] const size_type& size() const { return m_size; }
    [[nodiscard]] bool empty() const { return size() == 0; }
    [[nodiscard]] iterator begin() const { return data(); }
    [[nodiscard]] iterator end() const { return data() + m_size; }
    [[nodiscard]] reference operator[](size_type pos) const {
        return begin()[pos];
    }
    [[nodiscard]] reference front() const { return *begin(); }
    [[nodiscard]] reference back() const { return *std::prev(end()); }

private:
    offset_ptr<T> m_data;
    size_type m_size = 0;
};

template <typename Range>
offset_span(Range&& range)
    -> offset_span<std::remove_reference_t<
        decltype(*std::ranges::data(std::forward<Range>(range)))>>;

} // namespace decodeless
