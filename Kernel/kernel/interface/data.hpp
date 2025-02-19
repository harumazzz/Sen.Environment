#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Interface {

	struct Message {

		uint8_t* value{nullptr};

		size_t size{0};

	};

	inline constexpr auto total_size(
        CList<String>& strings
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
        CList<String>& strings
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
            .value = new uint8_t[size],
            .size = size,
        });
    }

    inline constexpr auto new_message (
        Pointer<Message> message,
        const size_t& size
    ) -> void
    {
        message->value = new uint8_t[size];
        message->size = size;
    }

    template <typename MessagePtr>
    inline auto construct (
        CList<String>& strings,
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
        CList<String>& strings,
        std::unique_ptr<Message, Deleter> &message
    ) -> void {
        new_message(message, total_size(strings));
        construct(strings, message);
    }

    inline auto construct_message (
        CList<String>& strings,
        Pointer<Message> message
    ) -> void {
        new_message(message, total_size(strings));
        construct(strings, message);
    }

    inline constexpr auto destruct_proxy(
        Pointer<uint8_t> ptr,
        const uint32_t& count,
        List<String>& result,
        const Pointer<Message> message
    ) -> void
    {
        for (auto i = u32{0}; i < count; ++i) {
            auto size = u32{};
            std::memcpy(&size, ptr, sizeof(u32));
            ptr += sizeof(u32);
            result.append(String{reinterpret_cast<const char*>(ptr), size});
            ptr += size;
        }
    }

    inline auto destruct_message (
        const Pointer<Message> & message,
        CList<String>& destination
    ) -> void {
	    if (message->size == 0) {
	        return;
	    }
        auto ptr = message->value;
        auto count = u32{};
        std::memcpy(&count, ptr, sizeof(u32));
        ptr += sizeof(u32);
	    destination.allocate(count);
        destruct_proxy(ptr, count, destination, message);
    }

    inline constexpr auto free_message (
        Pointer<Message> message
    ) -> void {
        if (message != nullptr) {
            delete[] message->value;
            message->value = nullptr;
            message->size = 0;
            delete message;
        }
    }

    inline constexpr auto total_size(
        const String& str
    ) -> usize {
        return sizeof(u32) + str.size();
    }

    template <typename Message>
    inline auto construct(
        const String& str,
        Message& message
    ) -> void {
        auto ptr = message->value;
        const auto size = static_cast<u32>(str.size());
        std::memcpy(ptr, &size, sizeof(u32));
        ptr += sizeof(u32);
        if (size > 0) {
            std::memcpy(ptr, str.begin(), size);
        }
    }
	

}