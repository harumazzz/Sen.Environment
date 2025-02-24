#pragma once
#include "library/service.hpp"
#include "library/version.hpp"
#include "bridge/executor.hpp"
#include "library/loader.hpp"
#include "library/proxy.hpp"

namespace Sen::Shell {

    class Library {

    private:

        inline static auto constexpr function_name = std::string_view{"execute"};

    protected:

        Loader m_loader;

        Execute m_execute;

    public:

        explicit Library(
            const std::string_view& path
        ) : m_loader{}, m_execute{nullptr} {
            thiz.m_loader.open(path);
            thiz.m_execute = thiz.m_loader.lookup<Execute>(function_name);
        }

        ~Library(
        ) noexcept {
            thiz.m_loader.close();
            thiz.m_execute = nullptr;
        }

    private:

        inline static auto callback (
            Service* service,
            Pointer<Message> source,
            Pointer<Message> destination
        ) -> int {
            const auto arguments = destruct_message(source);
            switch (hash_string(arguments[0])) {
                case hash_string("display"):
                    print_arguments(arguments);
                    break;
                case hash_string("input"): {
                    print(color_of(std::string{"● "}, std::string{"cyan"}));
                    const auto value = readline();
                    construct_message(service, make_list(value), destination);
                    break;
                }
                case hash_string("is_gui"): {
                    construct_message(service, make_list(std::to_string(is_gui)), destination);
                    break;
                }
                case hash_string("version"): {
                    construct_message(service, make_list(std::to_string(Shell::version)), destination);
                    break;
                }
                case hash_string("pick_file"): {
                    const auto value = open_pick_dialog<StorageType::pick_file>();
                    construct_message(service, make_list(value.value()), destination);
                    break;
                }
                case hash_string("pick_directory"): {
                    const auto value = open_pick_dialog<StorageType::pick_directory>();
                    construct_message(service, make_list(value.value()), destination);
                    break;
                }
                case hash_string("save_file"): {
                    const auto value = open_pick_dialog<StorageType::save_file>();
                    construct_message(service, make_list(value.value()), destination);
                    break;
                }
                default: {
                    return 1;
                }
            }
            return 0;
        }

    public:

        auto execute (
            const std::vector<std::string>& arguments
        ) const -> void {
            auto message = std::unique_ptr<Message, decltype(&free_message)>{nullptr, free_message};
            const auto service = std::unique_ptr<Service, decltype(&free_service)>(new Service {
                .callback = [](Service* service, Message* source, Message* destination) -> int {
                    return callback(service, source, destination);
                },
                .allocate = nullptr,
            }, free_service);
            construct_message(arguments, message);
            std::invoke(*thiz.m_execute, message.get(), service.get());
        }

    };

}
