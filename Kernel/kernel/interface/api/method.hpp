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
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 1, "Expected one argument", "serialize_fs");
            auto destination = String{};
            arguments[0].template get<String>(destination);
            auto data = String{};
            Kernel::FileSystem::read_file(destination, data);
            result.template set<String>(data);
        }

        inline auto write_file (
            const String& source,
            const String& destination
        ) -> void {
            Kernel::FileSystem::write_file(source, destination);
        }

        template <auto mode> requires std::is_same_v<type_of<mode>, Path::PathType>
        inline auto read_current_directory (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 1, "Expected one argument", "serialize_fs");
            auto source = String{};
            arguments[0].template get<String>(source);
            auto destination = List<String>{};
            Kernel::FileSystem::read_directory<mode>(source , destination);
            result.template set<List<String>>(destination);
        }

        inline auto read_directory (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 1, "Expected one argument", "serialize_fs");
            auto source = String{};
            arguments[0].template get<String>(source);
            auto destination = List<String>{};
            Kernel::FileSystem::read_recursive_directory_file(source , destination);
            result.template set<List<String>>(destination);
        }

    }

    namespace OperatingSystem {

        namespace Detail {

            inline static auto constexpr k_windows = "Windows"_sv;

            inline static auto constexpr k_linux = "Linux"_sv;

            inline static auto constexpr k_mac = "Macintosh"_sv;

            inline static auto constexpr k_android = "Android"_sv;

            inline static auto constexpr k_iphone = "iPhone"_sv;

        }

        inline auto constexpr current(
        ) -> std::string_view {
            #if WINDOWS
            return Detail::k_windows;
            #elif ANDROID
            return Detail::k_android;
            #elif LINUX
            return Detail::k_linux;
            #elif APPLE
            return Detail::k_mac;
            #elif IPHONE
            return Detail::k_iphone;
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
            auto source = Javascript::NativeString{};
            arguments[0].template get<Javascript::NativeString>(source);
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
            // TODO : Add loc
            assert_conditional(arguments.size() >= 1, "Expected at least one argument", "callback");
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

    namespace JSON {

        namespace Detail {

            inline auto deserialize (
                const std::string& source,
                Javascript::Value& result
            ) -> void {
                auto stream = std::istringstream{source};
                auto destination = Subprojects::jsoncons::json_stream_cursor{stream};
                result.template set<Subprojects::jsoncons::json_stream_cursor>(destination);
            }

            inline auto serialize (
                Kernel::Javascript::Value& object,
                std::ostringstream& stream
            ) -> void {
                auto os = Subprojects::jsoncons::json_stream_encoder{stream};
                object.template get<Subprojects::jsoncons::json_stream_encoder>(os);
                os.flush();
            }

        }

        inline auto deserialize (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 1, "Expected one argument", "deserialize");
            auto source = std::string{};
            arguments[0].template get<std::string>(source);
            Detail::deserialize(source, result);
        }

        inline auto deserialize_fs (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 1, "Expected one argument", "deserialize_fs");
            auto source = Javascript::NativeString{};
            arguments[0].template get<Javascript::NativeString>(source);
            auto view = std::string{};
            Kernel::FileSystem::read_file(source, view);
            Detail::deserialize(view, result);
        }

        inline auto serialize (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 1, "Expected one argument", "serialize");
            auto stream = std::ostringstream{};
            Detail::serialize(arguments[0], stream);
            result.template set<std::string_view>(stream.view());
        }

        inline auto serialize_fs (
            Javascript::Context& context,
            Javascript::Value& value,
            Array<Javascript::Value>& arguments,
            Javascript::Value& result
        ) -> void {
            // TODO : Add loc
            assert_conditional(arguments.size() == 2, "Expected two argument", "serialize_fs");
            auto destination = Javascript::NativeString{};
            arguments[0].template get<Javascript::NativeString>(destination);
            auto stream = std::ostringstream{};
            Detail::serialize(arguments[1], stream);
            const auto data = CharacterArrayView{const_cast<char*>(stream.view().data()), stream.view().size()};
            Kernel::FileSystem::write_file(destination, data);
            result.set_undefined();
        }

    }

    namespace Language {

        inline auto read_language (
            const StringView& source
        ) -> void {
            return Kernel::Language::read_language([&](std::optional<Subprojects::jsoncons::json>& value) -> void {
                auto data = String{};
                Kernel::FileSystem::read_file(source, data);
                value.emplace(Subprojects::jsoncons::json::parse(data.view()));
            });
        }

        inline auto get (
            const StringView& key
        ) -> std::string_view {
            return Kernel::Language::get(key.view());
        }

    }

}