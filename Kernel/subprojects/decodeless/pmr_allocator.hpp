// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include "./allocator.hpp"
#include <memory_resource>

namespace decodeless {

template <class BackingResource>
class memory_resource_adapter : public std::pmr::memory_resource {
public:
    template <class... Args>
    memory_resource_adapter(Args&&... args)
        : m_resource(std::forward<Args>(args)...) {}
    void* do_allocate(std::size_t bytes, std::size_t align) override {
        return m_resource.allocate(bytes, align);
    }
    void do_deallocate(void* p, std::size_t bytes, [[maybe_unused]] std::size_t align) override {
        m_resource.deallocate(p, bytes);
    }
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }
    BackingResource&       backing_resource() { return m_resource; }
    const BackingResource& backing_resource() const { return m_resource; }

private:
    BackingResource m_resource;
};

template <memory_resource_or_allocator ParentAllocator = std::allocator<std::byte>>
class pmr_linear_memory_resource
    : public memory_resource_adapter<linear_memory_resource<ParentAllocator>> {
public:
    using base_type = memory_resource_adapter<linear_memory_resource<ParentAllocator>>;
    pmr_linear_memory_resource(size_t                 initialSize,
                               const ParentAllocator& parentAllocator = ParentAllocator())
        requires allocator<ParentAllocator>
        : base_type(initialSize, parentAllocator) {}
    pmr_linear_memory_resource(size_t initialSize, ParentAllocator&& parentAllocator)
        requires memory_resource<ParentAllocator>
        : base_type(initialSize, std::move(parentAllocator)) {}
    pmr_linear_memory_resource()
        requires realloc_allocator<ParentAllocator>
    = default;
    pmr_linear_memory_resource(const ParentAllocator& parent)
        requires realloc_allocator<ParentAllocator>
        : base_type(parent) {}
    pmr_linear_memory_resource(ParentAllocator&& parent)
        requires realloc_memory_resource<ParentAllocator>
        : base_type(std::move(parent)) {}
    void   reset() { this->backing_resource().reset(); }
    void   truncate() { this->backing_resource().truncate(); }
    void*  data() const { return this->backing_resource().data(); }
    size_t size() const { return this->backing_resource().size(); }
    size_t capacity() const { return this->backing_resource().capacity(); }
};

} // namespace decodeless
