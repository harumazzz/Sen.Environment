#pragma once

#include "kernel/interface/context.hpp"
#include "kernel/interface/api/method.hpp"
#include "kernel/utility/javascript/proxy.hpp"
#include "kernel/interface/api/class.hpp"
#include "kernel/support/utility.hpp"

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
            auto s_Shell = s_Sen.add_space("Shell"_s);
            s_Shell.add_function<&API::Shell::callback>("callback"_s);
        }
        {
            auto s_Language = s_Kernel.add_space("Language"_s);
            s_Language.add_function<&Javascript::proxy_native_function_wrapper<&API::Language::read_language>>("read_language"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&API::Language::get>>("get"_s);
        }
        {
            auto s_JSON = s_Kernel.add_space("JSON"_s);
            s_JSON.add_function<&API::JSON::deserialize>("deserialize"_s)
            .add_function<&API::JSON::serialize>("serialize"_s)
            .add_function<&API::JSON::deserialize_fs>("deserialize_fs"_s)
            .add_function<&API::JSON::serialize_fs>("serialize_fs"_s);
        }
        {
            using Type = Path::PathType;
            auto s_FileSystem = s_Kernel.add_space("FileSystem"_s);
            s_FileSystem.add_function<&API::FileSystem::read_file>("read_file"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::write_file<String>>>("write_file"_s)
            .add_function<&API::FileSystem::read_current_directory<Type::None>>("read_current_directory"_s)
            .add_function<&API::FileSystem::read_current_directory<Type::File>>("read_directory_only_file"_s)
            .add_function<&API::FileSystem::read_current_directory<Type::Directory>>("read_directory_only_directory"_s)
            .add_function<&API::FileSystem::read_directory>("read_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::create_directory>>("create_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::is_file>>("is_file"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::is_directory>>("is_directory"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::rename>>("rename"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::copy>>("copy"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::remove>>("remove"_s);
        }
        {
            auto s_Path = s_Kernel.add_space("Path"_s);
            s_Path.add_function<&Javascript::proxy_native_function_wrapper<&Path::basename>>("basename"_s)
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
        {
            auto s_OperatingSystem = s_Kernel.add_space("OperatingSystem"_s);
            s_OperatingSystem.add_function<&Javascript::proxy_native_function_wrapper<&API::OperatingSystem::current>>("current"_s);
        }
        {
            auto s_Process = s_Kernel.add_space("Process"_s);
            s_Process.add_function<&Javascript::proxy_native_function_wrapper<&Process::execute>>("execute"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Process::run>>("run"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Process::is_exists_in_path_environment>>("is_exists_in_path_environment"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Process::get_path_environment>>("get_path_environment"_s);
        }
        {
            auto s_JavaScript = s_Kernel.add_space("JavaScript"_s);
            s_JavaScript.add_function<&API::JavaScript::evaluate_fs>("evaluate_fs"_s);
        }
        const auto s_Support = s_Kernel.add_space("Support"_s);
        {
            auto s_Texture = s_Support.add_space("Texture"_s);
            s_Texture.add_function<&Javascript::proxy_native_function_wrapper<&Support::Texture::InvokeMethod::Decode::process_fs>>("decode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::Texture::InvokeMethod::Encode::process_fs>>("encode_fs"_s);
        }
        const auto s_PopCap = s_Support.add_space("PopCap"_s);
        {
            auto s_ReflectionObjectNotation = s_PopCap.add_space("ReflectionObjectNotation"_s);
            s_ReflectionObjectNotation.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Encode::process_fs>>("encode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Decode::process_fs>>("decode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Encrypt::process_fs>>("encrypt_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReflectionObjectNotation::Decrypt::process_fs>>("decrypt_fs"_s);
        }
        {
            auto s_Zlib = s_PopCap.add_space("Zlib"_s);
            s_Zlib.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::Zlib::Compress::process_fs>>("compress_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::Zlib::Uncompress::process_fs>>("uncompress_fs"_s);
        }
        {
            auto s_CompiledText = s_PopCap.add_space("CompiledText"_s);
            s_CompiledText.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::CompiledText::Decode::process_fs>>("decode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::CompiledText::Encode::process_fs>>("encode_fs"_s);
        }
        {
            auto s_CharacterFontWidget2 = s_PopCap.add_space("CharacterFontWidget2"_s);
            s_CharacterFontWidget2.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::CharacterFontWidget2::Encode::process_fs>>("encode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::CharacterFontWidget2::Decode::process_fs>>("decode_fs"_s);
        }
        {
            auto s_CryptData = s_PopCap.add_space("CryptData"_s);
            s_CryptData.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::CryptData::Encrypt::process_fs>>("encrypt_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::CryptData::Decrypt::process_fs>>("decrypt_fs"_s);
        }
        {
            auto s_Package = s_PopCap.add_space("Package"_s);
            s_Package.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::Package::Unpack::process_fs>>("unpack_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::Package::Pack::process_fs>>("pack_fs"_s);
        }
        {
            auto s_NewTypeObjectNotation = s_PopCap.add_space("NewTypeObjectNotation"_s);
            s_NewTypeObjectNotation.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::NewTypeObjectNotation::Encode::process_fs>>("encode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::NewTypeObjectNotation::Decode::process_fs>>("decode_fs"_s);
        }
        {
            auto s_Animation = s_PopCap.add_space("Animation"_s);
            s_Animation.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::Animation::Decode::process_fs>>("decode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::Animation::Encode::process_fs>>("encode_fs"_s);
        }
        {
            auto s_ReAnimation = s_PopCap.add_space("ReAnimation"_s);
            s_ReAnimation.add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReAnimation::Decode::process_fs>>("decode_fs"_s)
            .add_function<&Javascript::proxy_native_function_wrapper<&Support::PopCap::ReAnimation::Encode::process_fs>>("encode_fs"_s);
        }
    }

    inline auto make_exception(
    ) -> List<String> {
        auto exception_message = List<String>{4_size};
        exception_message.append("display"_s);
        exception_message.append(StringHelper::make_string(parse_exception().what()));
        exception_message.append(""_s);
        exception_message.append("red"_s);
        return exception_message;
    };

}		