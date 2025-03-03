// Copyright (c) 2024 Pyarelal Knowles, MIT License

// Inspiration:
// - https://nfrechette.github.io/2015/06/11/vmem_linear_allocator/
// -
// https://stackoverflow.com/questions/55066654/resize-a-memory-mapped-file-on-windows-without-invalidating-pointers
// - https://stackoverflow.com/questions/44101966/adding-new-bytes-to-memory-mapped-file
// - https://devblogs.microsoft.com/oldnewthing/20150130-00/?p=44793

#pragma once

#include "./detail/mappedfile_common.hpp"
#if defined(_WIN32)
    #include "./detail/mappedfile_windows.hpp"
#else
    #include "./detail/mappedfile_linux.hpp"
#endif

namespace decodeless {

// May throw std::bad_alloc or std::runtime_error (mapped_file_error or
// mapping_error)
using file = detail::MappedFile<false>;
using writable_file = detail::MappedFile<true>;
using resizable_file = detail::ResizableMappedFile;
using resizable_memory = detail::ResizableMappedMemory;

template <class T>
concept move_only =
    std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T> &&
    !std::is_copy_constructible_v<T> && !std::is_copy_assignable_v<T>;

template <class T>
concept mapped_file = std::is_constructible_v<T, fs::path> && move_only<T> && requires(T t) {
    { t.data() } -> std::same_as<const void*>;
    { t.size() } -> std::same_as<size_t>;
};

template <class T>
concept writable_mapped_file =
    std::is_constructible_v<T, fs::path> && move_only<T> && requires(T t) {
        { t.data() } -> std::same_as<void*>;
        { t.size() } -> std::same_as<size_t>;
    };

template <class T>
concept resizable_mapped_memory = move_only<T> && requires(T t) {
    { t.data() } -> std::same_as<void*>;
    { t.size() } -> std::same_as<size_t>;
    { t.capacity() } -> std::same_as<size_t>;
    { t.resize(std::declval<size_t>()) } -> std::same_as<void>;
};

static_assert(mapped_file<file>);
static_assert(writable_mapped_file<writable_file>);
static_assert(resizable_mapped_memory<resizable_file>);
static_assert(std::is_constructible_v<resizable_file, fs::path, size_t>);
static_assert(resizable_mapped_memory<resizable_memory>);
static_assert(std::is_constructible_v<resizable_memory, size_t, size_t>);

} // namespace decodeless
