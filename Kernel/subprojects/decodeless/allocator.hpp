// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <assert.h>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include "./allocator_concepts.hpp"
#include <memory>

namespace decodeless {

// Utility for a linear_memory_resource backed by either a memory resource or an
// allocator. Only allocates std::byte. Could rebind, but seems simpler to
// restrict linear_memory_resource to just std::byte.
template <memory_resource_or_allocator ResOrAlloc>
std::byte* allocate_bytes(ResOrAlloc& resOrAlloc, size_t bytes) {
    if constexpr (memory_resource<ResOrAlloc>)
        return static_cast<std::byte*>(resOrAlloc.allocate(bytes, 1u));
    else
        return resOrAlloc.allocate(bytes);
}

// Reallocate utility for a linear_memory_resource backed by either a memory
// resource or an allocator
template <realloc_resource_or_allocator ResOrAlloc>
std::byte* reallocate_bytes(ResOrAlloc& resOrAlloc, std::byte* original, size_t size) {
    if constexpr (memory_resource<ResOrAlloc>)
        return static_cast<std::byte*>(
            resOrAlloc.reallocate(static_cast<void*>(original), size, 1u));
    else
        return resOrAlloc.reallocate(original, size);
}

// A possibly-growable local linear arena allocator.
// - growable: The backing allocation may grow if it has reallocate() and the
//   call returns the same address.
// - local: Has per-allocator-instance state
// - linear: Gives monotonic/sequential but aligned allocations that cannot be
//   freed or reused. There is only a reset() call. Only trivially destructible
//   objects should be created from this.
// - arena: Allocations come from a single blob/pool and when it is exhausted
//   std::bad_alloc is thrown (unless a reallocate() is possible).
// Backed by either a STL style allocator or a concrete memory resource,
// although both need a reallocate() and max_size() call to enable growing.
// NOTE: currently expects std::byte allocators - use rebind_alloc from
// std::allocator_traits if needed
template <memory_resource_or_allocator ResOrAlloc = std::allocator<std::byte>>
    requires memory_resource<ResOrAlloc> ||
             std::same_as<typename ResOrAlloc::value_type,
                          std::byte> // allocators must be of type std::byte
class linear_memory_resource {
public:
    using parent_allocator = ResOrAlloc;

    // Non-reallocating parent allocator constructor must take an initial size
#if _MSC_VER < 1930
    // MSVC 2019 doesn't allow a trailing requires clause on a function of a
    // templated class. Adding a dummy template like this avoids error C7599.
    template<class U = ResOrAlloc>
#endif
    linear_memory_resource(size_t initialSize, const ResOrAlloc& parent = ResOrAlloc())
        requires allocator<ResOrAlloc>
        : m_parent(parent)
        , m_begin(initialSize != 0 ? allocate_bytes<ResOrAlloc>(m_parent, initialSize) : nullptr)
        , m_next(reinterpret_cast<uintptr_t>(m_begin))
        , m_end(reinterpret_cast<uintptr_t>(m_begin) + initialSize) {
        if constexpr (nonrealloc_allocator<ResOrAlloc>) {
            assert(initialSize != 0);
        };
    }

    // Non-reallocating parent memory_resource constructor must take an initial
    // size
#if _MSC_VER < 1930
    template<class U = ResOrAlloc>
#endif
    linear_memory_resource(size_t initialSize, ResOrAlloc&& parent)
        requires memory_resource<ResOrAlloc>
        : m_parent(std::move(parent))
        , m_begin(initialSize != 0 ? allocate_bytes(m_parent, initialSize) : nullptr)
        , m_next(reinterpret_cast<uintptr_t>(m_begin))
        , m_end(reinterpret_cast<uintptr_t>(m_begin) + initialSize) {
        if constexpr (nonrealloc_memory_resource<ResOrAlloc>) {
            assert(initialSize != 0);
        };
    }

    // Reallocating parent allocator may default construct
#if _MSC_VER < 1930
    template<class U = ResOrAlloc>
#endif
    linear_memory_resource()
        requires realloc_allocator<ResOrAlloc>
        : m_parent() {}

    // Reallocating parent allocator can be copied
#if _MSC_VER < 1930
    template<class U = ResOrAlloc>
#endif
    linear_memory_resource(const ResOrAlloc& parent)
        requires realloc_allocator<ResOrAlloc>
        : m_parent(parent) {}

    // Reallocating parent memory_resource must be moved into the linear
    // resource
#if _MSC_VER < 1930
    template<class U = ResOrAlloc>
#endif
    linear_memory_resource(ResOrAlloc&& parent)
        requires realloc_memory_resource<ResOrAlloc>
        : m_parent(std::move(parent)) {}

    linear_memory_resource(const linear_memory_resource& other) = delete;
    linear_memory_resource(linear_memory_resource&& other) noexcept
        : m_parent(std::move(other.m_parent))
        , m_begin(other.m_begin)
        , m_next(other.m_next)
        , m_end(other.m_end) {
        other.m_end = reinterpret_cast<uintptr_t>(other.m_begin);
    }
    ~linear_memory_resource() { free(); }
    linear_memory_resource& operator=(const linear_memory_resource& other) = delete;
    linear_memory_resource& operator=(linear_memory_resource&& other) noexcept {
        free();
        m_parent = std::move(other.m_parent);
        m_begin = other.m_begin;
        m_next = other.m_next;
        m_end = other.m_end;
        other.m_end = reinterpret_cast<uintptr_t>(other.m_begin);
        return *this;
    }

    [[nodiscard]] constexpr void* allocate(std::size_t bytes, std::size_t align) {
        // Align
        uintptr_t result = m_next + ((-static_cast<ptrdiff_t>(m_next)) & (align - 1));

        // Allocate
        uintptr_t newNext = result + bytes;

        // Check for overflow and attempt to reallocate if possible
        if (newNext > m_end) {
            if constexpr (realloc_resource_or_allocator<ResOrAlloc>) {
                // Allocate the larger of double the existing arena or enough to
                // fit what was just requested.
                size_t minSize = newNext - reinterpret_cast<uintptr_t>(m_begin);
                size_t newSize = std::max(minSize, 2 * capacity());

                // If double the reservation would overflow the backing
                // allocator, allocate exactly the maximum.
                if constexpr (has_max_size<ResOrAlloc>) {
                    if (newSize > m_parent.max_size() && size() < m_parent.max_size()) {
                        newSize = m_parent.max_size();
                    }
                }

                if (capacity() == 0) {
                    // Handle an empty initial allocation growing for the firs time
                    m_begin = allocate_bytes(m_parent, newSize);
                    newNext += reinterpret_cast<uintptr_t>(m_begin);
                    result += reinterpret_cast<uintptr_t>(m_begin);
                } else {
                    // Verify the reallocation produced the same address.
                    std::byte* addr = reallocate_bytes(m_parent, m_begin, newSize);
                    if (addr != m_begin) {
                        throw std::bad_alloc();
                    }
                }

                m_end = reinterpret_cast<uintptr_t>(m_begin) + newSize;
            } else {
                // Double check there was an initial backing allocating from the
                // non-reallocating parent and this is a real OOM
                assert(capacity() != 0);
                throw std::bad_alloc();
            }
        }

        // Safe to update m_next as no exceptions were thrown.
        m_next = newNext;

        return reinterpret_cast<void*>(result);
    }

    // Deallocates memory. This operation is a no-op for linear_memory_resource
    // as individual deallocations are not supported.
    constexpr void deallocate(void* p, std::size_t bytes) {
        // Do nothing
        (void)p;
        (void)bytes;
    }

    // Clear all allocations to begin allocating from scratch, invalidating all
    // previously allocated memory.
    void reset() { m_next = reinterpret_cast<uintptr_t>(m_begin); }

    // Reallocate the parent allocation to exactly the size of all current
    // allocations.
    void truncate()
        requires realloc_resource_or_allocator<ResOrAlloc>
    {
        if (size() == 0) {
            // Free/deallocate the current backing allocation and start again.
            // This would be a new object, if it weren't for keeping the backing
            // allocator/memory_resource
            free();
            m_begin = nullptr;
            m_next = 0;
            m_end = 0;
        } else {
            std::byte* addr = reallocate_bytes(m_parent, m_begin, size());
            if (addr != m_begin)
                throw std::bad_alloc();
            m_end = m_next;
        }
    }

    // Returns a pointer to the arena/parent allocation.
    [[nodiscard]] void* data() const { return reinterpret_cast<void*>(m_begin); }

    // Returns the total number of bytes allocated within the arena
    [[nodiscard]] size_t size() const { return m_next - reinterpret_cast<uintptr_t>(m_begin); }

    // Returns the size of the arena/parent allocation
    [[nodiscard]] size_t capacity() const { return m_end - reinterpret_cast<uintptr_t>(m_begin); }

    // Provide public access to parent allocator. Primarily used for testing.
    [[nodiscard]] ResOrAlloc& parent() { return m_parent; }

private:
    void free() {
        if (capacity() != 0)
            m_parent.deallocate(m_begin, capacity());
    }

    ResOrAlloc m_parent;
    std::byte* m_begin = nullptr;
    uintptr_t  m_next = 0;
    uintptr_t  m_end = 0;
};

// Stateful STL-compatible allocator adaptor that holds a pointer to the
// concrete memory resource
template <trivially_destructible T, memory_resource MemoryResource>
class memory_resource_ref {
public:
    using resource_type = MemoryResource;
    using value_type = T;

    memory_resource_ref(resource_type& resource)
        : m_resource(&resource) {}

    // Needed by msvc
    template <class U>
    memory_resource_ref(const memory_resource_ref<U, MemoryResource>& other)
        : m_resource(&other.resource()) {}

    [[nodiscard]] constexpr T* allocate(std::size_t n) {
        return static_cast<T*>(m_resource->allocate(n * sizeof(T), alignof(T)));
    }

    constexpr void deallocate(T* p, std::size_t n) {
        return m_resource->deallocate(static_cast<void*>(p), n);
    }

    [[nodiscard]] constexpr T* reallocate(T* ptr, std::size_t bytes)
        requires realloc_memory_resource<MemoryResource>
    {
        return static_cast<T*>(m_resource->reallocate(ptr, bytes));
    }

    [[nodiscard]] constexpr size_t max_size() const
        requires has_max_size<MemoryResource>
    {
        return m_resource->max_size();
    }

    template <trivially_destructible U>
    [[nodiscard]] bool operator==(const memory_resource_ref<U, MemoryResource>& other) const {
        return m_resource == other.m_resource;
    }

    template <trivially_destructible U>
    [[nodiscard]] bool operator!=(const memory_resource_ref<U, MemoryResource>& other) const {
        return m_resource != other.m_resource;
    }

    // Public access required for rebind
    [[nodiscard]] resource_type& resource() const { return *m_resource; }

    // Needed by msvc
    template <class U>
    struct rebind {
        using other = memory_resource_ref<U, MemoryResource>;
    };

private:
    resource_type* m_resource;
};

// STL compatible allocator with an implicit linear_memory_resource memory
// resource. The need for this emphasizes why std::pmr is a thing - the
// MemoryResource would ideally not affect the type.
template <trivially_destructible       T,
          memory_resource_or_allocator ParentAllocator = std::allocator<std::byte>>
using linear_allocator = memory_resource_ref<T, linear_memory_resource<ParentAllocator>>;

} // namespace decodeless
