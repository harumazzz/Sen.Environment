// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

namespace decodeless {

template <class Resource>
concept memory_resource = std::move_constructible<Resource> && requires(Resource& resource) {
    {
        // allocate(size, alignment)
        resource.allocate(std::declval<std::size_t>(), std::declval<std::size_t>())
    } -> std::same_as<void*>;
    {
        // deallocate(ptr, size)
        resource.deallocate(std::declval<void*>(), std::declval<std::size_t>())
    } -> std::same_as<void>;
};

template <class Resource>
concept realloc_memory_resource = memory_resource<Resource> && requires(Resource& resource) {
    {
        // reallocate(ptr, size, alignment)
        resource.reallocate(std::declval<void*>(), std::declval<std::size_t>(),
                            std::declval<std::size_t>())
    } -> std::same_as<void*>;
};

template <class Resource>
concept nonrealloc_memory_resource =
    memory_resource<Resource> && !realloc_memory_resource<Resource>;

// TODO: does an allocator really need a value_type?
template <class Allocator>
concept allocator = std::copy_constructible<Allocator> &&
                    requires(Allocator& allocator, typename Allocator::value_type) {
                        {
                            // allocate(size)
                            allocator.allocate(std::declval<std::size_t>())
                        } -> std::same_as<typename Allocator::value_type*>;
                        {
                            // deallocate(ptr, size)
                            allocator.deallocate(std::declval<typename Allocator::value_type*>(),
                                                 std::declval<std::size_t>())
                        } -> std::same_as<void>;
                    };

template <class Allocator>
concept realloc_allocator =
    allocator<Allocator> && requires(Allocator& allocator, typename Allocator::value_type) {
        {
            // reallocate(ptr, size)
            allocator.reallocate(std::declval<typename Allocator::value_type*>(),
                                 std::declval<std::size_t>())
        } -> std::same_as<typename Allocator::value_type*>;
    };

template <class Allocator>
concept nonrealloc_allocator = allocator<Allocator> && !realloc_allocator<Allocator>;

// Facilitate decodeless::linear_memory_resource backed by either a memory
// resource or a STL style allocator
template <class ResOrAlloc>
concept memory_resource_or_allocator = (memory_resource<ResOrAlloc> || allocator<ResOrAlloc>);

template <class ResOrAlloc>
concept realloc_resource_or_allocator =
    realloc_memory_resource<ResOrAlloc> || realloc_allocator<ResOrAlloc>;

template <class ResOrAlloc>
concept nonrealloc_resource_or_allocator =
    nonrealloc_memory_resource<ResOrAlloc> || nonrealloc_allocator<ResOrAlloc>;

template <class ResOrAlloc>
concept has_max_size = memory_resource_or_allocator<ResOrAlloc> && requires(ResOrAlloc allocator) {
    { allocator.max_size() } -> std::same_as<std::size_t>;
};

template <typename T>
concept trivially_destructible = std::is_trivially_destructible_v<T>;

} // namespace decodeless
