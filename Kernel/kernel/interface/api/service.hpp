#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/container/set/common.hpp"

namespace Sen::Kernel::Interface::API {

    class Client {

    protected:

        Pointer<Service> m_service;

        HashSet<String> m_providers;

    public:

        explicit Client(
            const Pointer<Service>& service
        ) : m_service{service}, m_providers{} {

        }

        Client(const Client& other) = delete;

        Client(Client&& other) = delete;

        auto operator = (
            const Client& other
        ) -> Client& = delete;

        auto operator = (
            Client&& other
        ) -> Client& = delete;

        ~Client() = default;

        auto has (
            const StringView& provider
        ) -> bool {
            return std::ranges::any_of(thiz.m_providers, [&](const String& e) {
                return provider == e;
            });
        }

        auto add (
            const String& provider
        ) -> void {
            thiz.m_providers.insert(provider);
        }

        auto service (
        ) -> Pointer<Service>& {
            return thiz.m_service;
        }

        auto providers (
        ) -> HashSet<String>& {
            return thiz.m_providers;
        }

    };

}
