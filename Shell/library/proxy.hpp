#pragma once

#include "bridge/data.hpp"
#include "library/builtin.hpp"
#include "library/standard.hpp"

namespace Sen::Shell {

    inline constexpr auto total_size(
        const std::vector<std::string>& strings
    ) -> usize
    {
        auto result = sizeof(u32);
        for (auto& e : strings) {
            result += sizeof(u32) + e.size();
        }
        return result;
    }

    template <typename Deleter>
    inline constexpr auto new_message (
        std::unique_ptr<Message, Deleter> &message,
        const size_t& size
    ) -> void
    {
        message.reset(new Message{
            .value = reinterpret_cast<uint8_t*>(std::malloc(size * sizeof(uint8_t))),
            .size = size,
        });
    }

    inline constexpr auto construct_proxy(
        Pointer<uint8_t> ptr,
        const std::vector<std::string>& strings
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

    template <typename MessagePtr>
    inline auto construct (
        const std::vector<std::string>& strings,
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
        const std::vector<std::string>& strings,
        std::unique_ptr<Message, Deleter> &message
    ) -> void {
        new_message(message, total_size(strings));
        construct(strings, message);
    }

    inline auto construct_message (
        Service* service,
        const std::vector<std::string>& strings,
        Pointer<Message> message
    ) -> void {
        auto size = total_size(strings);
        service->allocate(message, &size);
        construct(strings, message);
    }

    inline constexpr auto destruct_proxy(
        Pointer<uint8_t> ptr,
        const uint32_t& count,
        std::vector<std::string>& strings,
        const Pointer<Message> message
    ) -> void
    {
        const auto end = message->value + message->size;
        for (auto i = u32{0}; i < count; ++i) {
            if (ptr + sizeof(u32) > end) {
                break;
            }
            auto size = u32{};
            std::memcpy(&size, ptr, sizeof(u32));
            ptr += sizeof(u32);
            if (ptr + size > end) {
                break;
            }
            strings.emplace_back(reinterpret_cast<const char*>(ptr), size);
            ptr += size;
        }
    }

    inline auto destruct_message (
        const Pointer<Message> & message
    ) -> std::vector<std::string> {
        auto strings = std::vector<std::string>{};
        if (message->size == sizeof(u32)) {
            return strings;
        }
        auto ptr = message->value;
        auto count = u32{};
        std::memcpy(&count, ptr, sizeof(u32));
        ptr += sizeof(u32);
        destruct_proxy(ptr, count, strings, message);
        return strings;
    }

    inline constexpr auto free_message (
        Pointer<Message> message
    ) -> void {
        if (message != nullptr) {
            std::free(message->value);
            message->value = nullptr;
            message->size = 0;
            delete message;
        }
    }

    inline constexpr auto free_service (
        Pointer<Service> service
    ) -> void {
        delete service;
    }

    inline constexpr auto total_size(
        const std::string& str
    ) -> usize {
        return sizeof(u32) + str.size();
    }

    template <typename... Args>
    inline auto make_list(Args&&... args) -> std::vector<std::string> {
        return std::vector<std::string>{std::forward<Args>(args)...};
    }


}