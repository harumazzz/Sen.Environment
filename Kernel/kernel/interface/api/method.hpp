#pragma once

#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::Interface::API {

    template <typename T>
    using List = CList<T>;

    template <typename T>
    using Array = CArray<T>;

    inline constexpr auto version (
    ) -> usize {
        return Kernel::version;
    }

    namespace FileSystem {

        inline auto read_file (
            const String& source
        ) -> String {
            auto destination = String{};
            Kernel::FileSystem::read_file(source , destination);
            return destination;
        }

        inline auto write_file (
            const String& source,
            const String& destination
        ) -> void {
            Kernel::FileSystem::write_file(source, const_cast<String&>(destination));
        }

        template <auto mode> requires std::is_same_v<type_of<mode>, Path::PathType>
        inline auto read_current_directory (
            const String& source
        ) -> List<String> {
            auto destination = List<String>{};
            Kernel::FileSystem::read_directory<mode>(source , destination);
            return destination;
        }

        inline auto read_directory (
            const String& source
        ) -> List<String> {
            auto destination = List<String>{};
            Kernel::FileSystem::read_recursive_directory_file(source , destination);
            return destination;
        }

    }

    namespace Path {

        inline static auto join(
			List<String>& source
		) -> String
		{
			return Kernel::Path::join(source);
		}

    }

    namespace OperatingSystem {

        inline auto current(
        ) -> String {
            #if WINDOWS
            return String{ "Windows" };
            #elif ANDROID
            return String{ "Android" };
            #elif LINUX
            return String{ "Linux" };
            #elif APPLE
            return String{ "Macintosh" };
            #elif IPHONE
            return String{ "iPhone" };
            #else
            assert_conditional(false, "Unknown operating system", "current");
            #endif
        }

    }

    namespace JavaScript {

        inline auto evaluate_fs (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            auto source = String{};
            arguments[0].template get<String>(source);
            auto view = String{};
            Kernel::FileSystem::read_file(source, view);
            auto destination = context.evaluate(view, source);
            result.set_value(destination.release());
        }

    }

    namespace Shell {

        inline auto callback (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            const auto service = context.get_opaque<Service>();
            auto message = std::unique_ptr<Message, decltype(&free_message)>{nullptr, &free_message};
            auto destination = std::unique_ptr<Message, decltype(&free_message)>{new Message{}, &free_message};
            auto source = List<String>{arguments.size()};
            source.resize(arguments.size());
            for (auto index : Range{arguments.size()}) {
                arguments[index].template get<String>(source[index]);
            }
            construct_message(source, message);
            std::invoke(*service->callback, message.get(), destination.get());
            auto make_message = [&destination]() -> List<String> {
                auto m_result = List<String>{};
                destruct_message(destination.get(), m_result);
                return m_result;
            };
            result.template set<List<String>>(make_message());
        }

    }

}