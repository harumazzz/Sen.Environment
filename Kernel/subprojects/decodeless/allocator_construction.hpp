// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <memory>
#include <span>

#if __has_include(<ranges>)
    #include <ranges>
#endif

namespace decodeless {

namespace create {

// Utility calls to construct objects from a decodeless memory resource
namespace from_resource {

// Copy constructs an object with implicit type deduction.
template <trivially_destructible T, memory_resource MemoryResource>
T* object(MemoryResource& memoryResource, const T& init) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    return std::construct_at<T>(
        reinterpret_cast<T*>(memoryResource.allocate(sizeof(T), alignof(T))), init);
};

// Construct an explicitly typed object with any arguments.
template <trivially_destructible T, memory_resource MemoryResource, class... Args>
T* object(MemoryResource& memoryResource, Args&&... args) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    return std::construct_at<T>(
        reinterpret_cast<T*>(memoryResource.allocate(sizeof(T), alignof(T))),
        std::forward<Args>(args)...);
};

// Default construct an array of 'size' objects.
template <trivially_destructible T, memory_resource MemoryResource>
std::span<T> array(MemoryResource& memoryResource, size_t size) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    auto result = std::span(
        reinterpret_cast<T*>(memoryResource.allocate(sizeof(T) * size, alignof(T))), size);
    for (auto& obj : result)
        std::construct_at<T>(&obj);
    return result;
};

#ifdef __cpp_lib_ranges
// Copy construct an array of objects from a range.
template <trivially_destructible T, std::ranges::input_range Range = std::initializer_list<T>,
          memory_resource MemoryResource>
    requires std::convertible_to<std::ranges::range_value_t<Range>, T>
std::span<T> array(MemoryResource& memoryResource, Range&& range) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    auto size = std::ranges::size(range);
    auto result = std::span(
        reinterpret_cast<T*>(memoryResource.allocate(sizeof(T) * size, alignof(T))), size);
    auto out = result.begin();
    for (const auto& in : range)
        std::construct_at<T>(&*out++, in);
    return result;
};

// Overload to deduce T from the Range type
template <std::ranges::input_range Range, memory_resource MemoryResource>
auto array(MemoryResource& memoryResource, Range&& range) {
    return array<std::ranges::range_value_t<Range>, Range, MemoryResource>(
        memoryResource, std::forward<Range>(range));
}
#endif

} // namespace from_resource

// Utility calls to construct objects from an STL compatible allocator
namespace from_allocator {

// Shortcut to rebind an allocator to a different type
template <trivially_destructible T, allocator Allocator>
using allocator_rebind_t = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;

// Copy constructs an object with implicit type deduction.
template <trivially_destructible T, allocator Allocator>
T* object(const Allocator& allocator, const T& init) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    return std::construct_at<T>(allocator_rebind_t<T, Allocator>(allocator).allocate(1), init);
};

// Construct an explicitly typed object with any arguments.
template <trivially_destructible T, allocator Allocator, class... Args>
T* object(const Allocator& allocator, Args&&... args) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    return std::construct_at<T>(allocator_rebind_t<T, Allocator>(allocator).allocate(1),
                                std::forward<Args>(args)...);
};

// Default construct an array of 'size' objects.
template <trivially_destructible T, allocator Allocator>
std::span<T> array(const Allocator& allocator, size_t size) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    auto result = std::span(allocator_rebind_t<T, Allocator>(allocator).allocate(size), size);
    for (auto& obj : result)
        std::construct_at<T>(&obj);
    return result;
};

#ifdef __cpp_lib_ranges
// Copy construct an array of objects from a range.
template <trivially_destructible T, std::ranges::input_range Range = std::initializer_list<T>,
          allocator Allocator>
    requires std::convertible_to<std::ranges::range_value_t<Range>, T>
std::span<T> array(const Allocator& allocator, Range&& range) {
    static_assert(!std::is_const_v<T>, "const construction not allowed. cast instead");
    auto size = std::ranges::size(range);
    auto result = std::span(allocator_rebind_t<T, Allocator>(allocator).allocate(size), size);
    auto out = result.begin();
    for (const auto& in : range)
        std::construct_at<T>(&*out++, in);
    return result;
};

// Overload to deduce T from the Range type.
template <std::ranges::input_range Range, allocator Allocator>
auto array(const Allocator& allocator, Range&& range) {
    return array<std::ranges::range_value_t<Range>, Range, Allocator>(allocator,
                                                                      std::forward<Range>(range));
}
#endif

} // namespace from_allocator

// Add all construction calls to the decodeless::create namespace
using namespace from_resource;
using namespace from_allocator;

} // namespace create

} // namespace decodeless
