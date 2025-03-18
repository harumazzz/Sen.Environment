#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Interface {

	struct Message {

		uint8_t* value{nullptr};

		size_t size{0};

	};

	inline constexpr auto total_size(
        CArray<String>& strings
    ) -> usize
    {
        auto result = sizeof(u32);
        for (auto& e : strings) {
            result += sizeof(u32) + e.size();
        }
        return result;
    }

    inline auto construct_proxy(
        Pointer<uint8_t> ptr,
        CArray<String>& strings
    ) -> void
    {
        for (auto& str : strings) {
            auto size = static_cast<uint32_t>(str.size());
            std::memcpy(ptr, &size, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
            if (size > 0) {
                std::memcpy(ptr, str.data(), size);
                ptr += size;
            }
        }
    }

    template <typename Deleter>
    inline constexpr auto new_message (
        std::unique_ptr<Message, Deleter> &message,
        const size_t& size
    ) -> void
    {
        message.reset(new Message{
            .value = Allocator<uint8_t>::allocate(size),
            .size = size,
        });
    }

    inline constexpr auto new_message (
        Pointer<Message> message,
        const size_t& size
    ) -> void
    {
        message->value = Allocator<uint8_t>::allocate(size);
        message->size = size;
    }

    template <typename MessagePtr>
    inline auto construct (
        CArray<String>& strings,
        MessagePtr& message
    ) -> void {
        auto ptr = message->value;
        const auto count = static_cast<u32>(strings.size());
        std::memcpy(ptr, &count, sizeof(count));
        ptr += sizeof(u32);
        construct_proxy(ptr, strings);
    }

    template <typename Deleter>
    inline auto construct_message (
        CArray<String>& strings,
        std::unique_ptr<Message, Deleter> &message
    ) -> void {
        new_message(message, total_size(strings));
        construct(strings, message);
    }

    inline auto construct_message (
        CArray<String>& strings,
        Pointer<Message> message
    ) -> void {
        new_message(message, total_size(strings));
        construct(strings, message);
    }

    inline constexpr auto destruct_proxy(
        Pointer<uint8_t> ptr,
        const uint32_t& count,
        CArray<String>& result
    ) -> void
    {
        for (auto i = u32{0}; i < count; ++i) {
            auto size = u32{};
            std::memcpy(&size, ptr, sizeof(u32));
            ptr += sizeof(u32);
            auto temporary = String{reinterpret_cast<const char*>(ptr), size};
            result[i].take_ownership(temporary);
            ptr += size;
        }
    }

    inline auto destruct_message (
        const Pointer<Message> & message,
        CArray<String>& destination
    ) -> void {
	    if (message->size == 0) {
	        return;
	    }
        auto ptr = message->value;
        auto count = u32{};
        std::memcpy(&count, ptr, sizeof(u32));
        ptr += sizeof(u32);
	    destination.allocate(count);
        destruct_proxy(ptr, count, destination);
    }

    inline constexpr auto free_message (
        Pointer<Message> message
    ) -> void {
        if (message != nullptr) {
            Allocator<uint8_t>::deallocate(message->value);
            message->value = nullptr;
            message->size = 0;
            delete message;
        }
    }
	

}