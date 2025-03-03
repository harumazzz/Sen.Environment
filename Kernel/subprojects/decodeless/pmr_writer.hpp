// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include "./pmr_allocator.hpp"
#include "./writer.hpp"

namespace decodeless {

// DANGER: Allocated objects must all be trivially copyable/destructible. Due to
// the std::pmr::memory_resource interface, there is no validation here to
// support that restriction.

class pmr_file_writer {
public:
    using memory_resource_type = memory_resource_adapter<linear_file_memory_resource>;
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    pmr_file_writer(const fs::path& path, size_t maxSize)
        : m_linearResource(mapped_file_memory_resource(path, maxSize)) {}
    pmr_file_writer(const fs::path& path, size_t maxSize, size_t initialSize)
        : m_linearResource(initialSize, mapped_file_memory_resource(path, maxSize)) {}
    [[nodiscard]] memory_resource_type& resource() { return m_linearResource; }
    [[nodiscard]] allocator_type        allocator() { return &m_linearResource; }
    [[nodiscard]] void*  data() const { return m_linearResource.backing_resource().data(); }
    [[nodiscard]] size_t size() const { return m_linearResource.backing_resource().size(); }

    template <class T, class... Args>
    T* create(Args&&... args) {
        return decodeless::create::object<T>(m_linearResource.backing_resource(),
                                             std::forward<Args>(args)...);
    }

    template <class T>
    std::span<T> createArray(size_t size) {
        return decodeless::create::array<T>(m_linearResource.backing_resource(), size);
    }

#ifdef __cpp_lib_ranges
    template <class T, std::ranges::input_range Range>
    std::span<T> createArray(Range&& range) {
        return decodeless::create::array<T>(m_linearResource.backing_resource(),
                                            std::forward<Range>(range));
    }

    template <std::ranges::input_range Range>
    std::span<std::ranges::range_value_t<Range>> createArray(Range&& range) {
        return decodeless::create::array(m_linearResource.backing_resource(),
                                         std::forward<Range>(range));
    }
#endif

private:
    memory_resource_type m_linearResource;
};

class pmr_memory_writer {
public:
    using memory_resource_type =
        memory_resource_adapter<linear_memory_resource<mapped_memory_memory_resource>>;
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    pmr_memory_writer(size_t maxSize)
        : m_linearResource(mapped_memory_memory_resource(maxSize)) {}
    pmr_memory_writer(size_t maxSize, size_t initialSize)
        : m_linearResource(initialSize, mapped_memory_memory_resource(maxSize)) {}
    [[nodiscard]] memory_resource_type& resource() { return m_linearResource; }
    [[nodiscard]] allocator_type        allocator() { return &m_linearResource; }
    [[nodiscard]] void*  data() const { return m_linearResource.backing_resource().data(); }
    [[nodiscard]] size_t size() const { return m_linearResource.backing_resource().size(); }

    template <class T, class... Args>
    T* create(Args&&... args) {
        return decodeless::create::object<T>(m_linearResource.backing_resource(),
                                             std::forward<Args>(args)...);
    }

    template <class T>
    std::span<T> createArray(size_t size) {
        return decodeless::create::array<T>(m_linearResource.backing_resource(), size);
    }

#ifdef __cpp_lib_ranges
    template <class T, std::ranges::input_range Range>
    std::span<T> createArray(Range&& range) {
        return decodeless::create::array<T>(m_linearResource.backing_resource(),
                                            std::forward<Range>(range));
    }

    template <std::ranges::input_range Range>
    std::span<std::ranges::range_value_t<Range>> createArray(Range&& range) {
        return decodeless::create::array(m_linearResource.backing_resource(),
                                         std::forward<Range>(range));
    }
#endif

private:
    memory_resource_type m_linearResource;
};

} // namespace decodeless
