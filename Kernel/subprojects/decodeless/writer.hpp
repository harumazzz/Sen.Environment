// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include "./allocator.hpp"
#include "./allocator_construction.hpp"
#include "./mappedfile.hpp"
#include <filesystem>

namespace decodeless {

namespace fs = std::filesystem;

// Adapts decodeless::resizable_file and decodeless::resizable_memory to a
// resizable decodeless::memory_resource
template <resizable_mapped_memory ResizableMemory>
class growable_memory_resource : public ResizableMemory {
public:
    using ResizableMemory::ResizableMemory;

    [[nodiscard]] void* allocate(std::size_t bytes, std::size_t align) {
        if (ResizableMemory::size() != 0)
            throw std::bad_alloc();
        ResizableMemory::resize(bytes);
        // m_mappedFile mapping alignment should easily be big enough
        if ((reinterpret_cast<uintptr_t>(ResizableMemory::data()) & (align - 1)) != 0)
            throw std::bad_alloc();
        return ResizableMemory::data();
    }

    [[nodiscard]] void* reallocate(void* ptr, std::size_t bytes,
                                   [[maybe_unused]] std::size_t align) {
        if (ptr != ResizableMemory::data())
            throw std::bad_alloc();
        ResizableMemory::resize(bytes);
        return ResizableMemory::data();
    }

    constexpr void deallocate(void* p, std::size_t bytes) {
        // Do nothing
        (void)p;
        (void)bytes;
    }

    size_t max_size() const { return ResizableMemory::capacity(); }
    void   resize(size_t size) { ResizableMemory::resize(size); }
};

// The concrete resizable_file memory resource
// Note: truncates the file/memory on construction!
class mapped_file_memory_resource : public growable_memory_resource<resizable_file> {
public:
    mapped_file_memory_resource(const fs::path& path, size_t maxSize)
        : growable_memory_resource<resizable_file>(path, maxSize) {
        // Truncate the file if it already exists
        if (this->size() > 0)
            this->resize(0);
    }
};

static_assert(realloc_memory_resource<mapped_file_memory_resource>);
static_assert(has_max_size<mapped_file_memory_resource>);

// The concrete resizable_memory memory resource with default zero size to match
// mapped_file_memory_resource.
class mapped_memory_memory_resource : public growable_memory_resource<resizable_memory> {
public:
    mapped_memory_memory_resource(size_t maxSize)
        : growable_memory_resource<resizable_memory>(0, maxSize) {}
};

static_assert(realloc_memory_resource<mapped_memory_memory_resource>);
static_assert(has_max_size<mapped_memory_memory_resource>);

// Wrapper around a linear_memory_resource to truncate the
// mapped_file_memory_resource on destruction
class linear_file_memory_resource : public linear_memory_resource<mapped_file_memory_resource> {
public:
    using linear_memory_resource<mapped_file_memory_resource>::linear_memory_resource;
    ~linear_file_memory_resource() {
        // Truncate down to what was allocated
        if (m_truncateBackingOnDestruct)
            this->truncate();
    }
    linear_file_memory_resource(linear_file_memory_resource&& other) noexcept
        : linear_memory_resource<mapped_file_memory_resource>(std::move(other)) {
        other.m_truncateBackingOnDestruct = false;
    }
    linear_file_memory_resource& operator=(linear_file_memory_resource&& other) noexcept {
        linear_memory_resource<mapped_file_memory_resource>::operator=(std::move(other));
        other.m_truncateBackingOnDestruct = false;
        return *this;
    }

private:
    // Avoid resizing invalid objects after std::move()
    bool m_truncateBackingOnDestruct = true;
};

// Combines decodeless::resizable_file with decodeless::linear_memory_resource.
// This can be useful for creating a file with random access and without knowing
// the size up front. The virtual address space is reserved at initialization so
// pointers into the file remain valid.
class file_writer {
public:
    using memory_resource_type = linear_file_memory_resource;
    using allocator_type = memory_resource_ref<std::byte, memory_resource_type>;
    file_writer(const fs::path& path, size_t maxSize)
        : m_linearResource(mapped_file_memory_resource(path, maxSize)) {}
    file_writer(const fs::path& path, size_t maxSize, size_t initialSize)
        : m_linearResource(initialSize, mapped_file_memory_resource(path, maxSize)) {}
    [[nodiscard]] memory_resource_type& resource() { return m_linearResource; }
    [[nodiscard]] allocator_type        allocator() { return m_linearResource; }
    [[nodiscard]] void*                 data() const { return m_linearResource.data(); }
    [[nodiscard]] size_t                size() const { return m_linearResource.size(); }

    template <class T, class... Args>
    T* create(Args&&... args) {
        return decodeless::create::object<T>(resource(), std::forward<Args>(args)...);
    }

    template <class T>
    std::span<T> createArray(size_t size) {
        return decodeless::create::array<T>(resource(), size);
    }

#ifdef __cpp_lib_ranges
    template <class T, std::ranges::input_range Range>
    std::span<T> createArray(Range&& range) {
        return decodeless::create::array<T>(resource(), std::forward<Range>(range));
    }

    template <std::ranges::input_range Range>
    std::span<std::ranges::range_value_t<Range>> createArray(Range&& range) {
        return decodeless::create::array(resource(), std::forward<Range>(range));
    }
#endif

private:
    memory_resource_type m_linearResource;
};

// Provides  the same interface as file_writer, but backed by memory rather than
// a file. Just like the file, memory can grow and pointers remain valid.
class memory_writer {
public:
    using memory_resource_type = linear_memory_resource<mapped_memory_memory_resource>;
    using allocator_type = memory_resource_ref<std::byte, memory_resource_type>;
    memory_writer(size_t maxSize)
        : m_linearResource(mapped_memory_memory_resource(maxSize)) {}
    memory_writer(size_t maxSize, size_t initialSize)
        : m_linearResource(initialSize, mapped_memory_memory_resource(maxSize)) {}
    [[nodiscard]] memory_resource_type& resource() { return m_linearResource; }
    [[nodiscard]] allocator_type        allocator() { return m_linearResource; }
    [[nodiscard]] void*                 data() const { return m_linearResource.data(); }
    [[nodiscard]] size_t                size() const { return m_linearResource.size(); }

    template <class T, class... Args>
    T* create(Args&&... args) {
        return decodeless::create::object<T>(resource(), std::forward<Args>(args)...);
    }

    template <class T>
    std::span<T> createArray(size_t size) {
        return decodeless::create::array<T>(resource(), size);
    }

#ifdef __cpp_lib_ranges
    template <class T, std::ranges::input_range Range>
    std::span<T> createArray(Range&& range) {
        return decodeless::create::array<T>(resource(), std::forward<Range>(range));
    }

    template <std::ranges::input_range Range>
    std::span<std::ranges::range_value_t<Range>> createArray(const Range& range) {
        return decodeless::create::array(resource(), std::forward<Range>(range));
    }
#endif

private:
    memory_resource_type m_linearResource;
};

// STL compatible allocator for a linear_file_memory_resource
template <trivially_destructible T = std::byte>
using mapped_file_allocator = memory_resource_ref<T, linear_file_memory_resource>;

// STL compatible allocator for a linear_memory_resource<mapped_memory_memory_resource>>
template <trivially_destructible T = std::byte>
using mapped_memory_allocator =
    memory_resource_ref<T, linear_memory_resource<mapped_memory_memory_resource>>;

} // namespace decodeless
