#pragma once

#include "kernel/interface/context.hpp"
#include "kernel/interface/api/method.hpp"
#include "kernel/utility/javascript/proxy.hpp"

namespace Sen::Kernel::Interface::Runtime {


    inline auto make_environment (
        Context& context
    ) -> void {
        const auto s_Sen = Javascript::NamespaceBuilder{"Sen"_s, context.context().global_object()};
        auto s_Kernel = s_Sen.add_space("Kernel"_s);
        {
            s_Kernel.add_function<&Javascript::proxy_native_function_wrapper<&API::version>>("version"_s);
        }
        {
            using Type = Path::PathType;
            auto s_FileSystem = s_Kernel.add_space("FileSystem"_s);
            s_FileSystem.add_function<&Javascript::proxy_native_function_wrapper<&API::FileSystem::read_file>>("read_file"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&API::FileSystem::write_file>>("write_file"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&API::FileSystem::read_current_directory<Type::None>>>("read_current_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&API::FileSystem::read_current_directory<Type::File>>>("read_directory_only_file"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&API::FileSystem::read_current_directory<Type::Directory>>>("read_directory_only_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&API::FileSystem::read_directory>>("read_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::create_directory>>("create_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::is_file>>("is_file"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::is_directory>>("is_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::rename>>("rename"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::copy>>("copy"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::remove>>("remove"_s);
        }
        {
            auto s_Path = s_Kernel.add_space("Path"_s);
            s_Path.add_function<&Javascript::proxy_native_function_wrapper<&Path::join<String>>>("join"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::basename>>("basename"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::dirname>>("dirname"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::normalize>>("normalize"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::base_without_extension>>("base_without_extension"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::except_extension>>("except_extension"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::resolve>>("resolve"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Path::extname>>("extname"_s);
        }
        {
            auto clock = s_Kernel.add_class<Clock>("Clock"_s);
            clock.add_constructor<&Javascript::proxy_native_function_wrapper<&Detail::make_constructor<Clock>>>()
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::start_safe>>("start_safe"_s)
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::stop_safe>>("stop_safe"_s)
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::reset>>("reset"_s)
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::duration>>("duration"_s)
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::duration_as_seconds>>("duration_as_seconds"_s)
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::is_started>>("is_started"_s)
            .add_member_function<&Javascript::proxy_native_function_wrapper<&Clock::is_stopped>>("is_stopped"_s);
        }
        const auto s_Support = s_Kernel.add_space("Support"_s);
        const auto s_PopCap = s_Support.add_space("PopCap"_s);
        {
            auto s_ReflectionObjectNotation = s_PopCap.add_space("ReflectionObjectNotation"_s);
            s_ReflectionObjectNotation.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Encode::process_fs>>("encode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Decode::process_fs>>("decode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Encrypt::process_fs>>("encrypt_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Decrypt::process_fs>>("decrypt_fs"_s);
        }
    }

}		