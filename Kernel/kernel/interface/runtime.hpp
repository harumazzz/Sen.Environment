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
            s_Kernel.add_function_by_proxy<&API::version>("version"_s);
        }
        {
            auto s_Shell = s_Sen.add_space("Shell"_s);
            s_Shell.add_function<&API::Shell::callback>("callback"_s);
        }
        {
            auto s_Module = s_Kernel.add_space("Module"_s);
            s_Module.add_function<&API::Module::add_support>("add_support"_s);
        }
        {
            auto s_Language = s_Kernel.add_space("Language"_s);
            s_Language.add_function_by_proxy<&API::Language::read_language>("read_language"_s)
            .add_function_by_proxy<&API::Language::get>("get"_s);
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
            .add_function_by_proxy<&FileSystem::write_file<StringView>>("write_file"_s)
            .add_function<&API::FileSystem::read_current_directory<Type::None>>("read_current_directory"_s)
            .add_function<&API::FileSystem::read_current_directory<Type::File>>("read_directory_only_file"_s)
            .add_function<&API::FileSystem::read_current_directory<Type::Directory>>("read_directory_only_directory"_s)
            .add_function<&API::FileSystem::read_directory>("read_directory"_s)
            .add_function_by_proxy<&FileSystem::create_directory>("create_directory"_s)
            .add_function_by_proxy<&FileSystem::is_file>("is_file"_s)
            .add_function_by_proxy<&FileSystem::is_directory>("is_directory"_s)
            .add_function_by_proxy<&FileSystem::rename>("rename"_s)
            .add_function_by_proxy<&FileSystem::copy>("copy"_s)
            .add_function_by_proxy<&FileSystem::remove>("remove"_s);
        }
        {
            auto s_Path = s_Kernel.add_space("Path"_s);
            s_Path.add_function_by_proxy<&Path::basename>("basename"_s)
            .add_function_by_proxy<&Path::dirname>("dirname"_s)
            .add_function_by_proxy<&Path::normalize>("normalize"_s)
            .add_function_by_proxy<&Path::base_without_extension>("base_without_extension"_s)
            .add_function_by_proxy<&Path::except_extension>("except_extension"_s)
            .add_function_by_proxy<&Path::resolve>("resolve"_s)
            .add_function_by_proxy<&Path::extname>("extname"_s);
        }
        {
            auto s_Clock = s_Kernel.add_class<Clock>("Clock"_s);
            s_Clock.add_constructor_by_proxy<&Detail::make_constructor<Clock>>()
            .add_member_function_by_proxy<&Clock::start_safe>("start_safe"_s)
            .add_member_function_by_proxy<&Clock::stop_safe>("stop_safe"_s)
            .add_member_function_by_proxy<&Clock::reset>("reset"_s)
            .add_member_function_by_proxy<&Clock::duration>("duration"_s)
            .add_member_function_by_proxy<&Clock::duration_as_seconds>("duration_as_seconds"_s)
            .add_member_function_by_proxy<&Clock::is_started>("is_started"_s)
            .add_member_function_by_proxy<&Clock::is_stopped>("is_stopped"_s);
        }
        {
            auto s_ReadMemoryStream = s_Kernel.add_class<ReadMemoryStream>("ReadMemoryStream"_s);
            s_ReadMemoryStream.add_constructor_by_proxy<&Detail::make_constructor<ReadMemoryStream, Uint8Array&>>()
            .add_member_function_by_proxy<&ReadMemoryStream::size>("size"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::has_space>("has_space"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::set_position>("set_position"_s)
            .add_member_function_by_proxy<static_cast<String (ReadMemoryStream::*)(const usize&)>(&ReadMemoryStream::string)>("string"_s)
            .add_member_function_by_proxy<static_cast<String (ReadMemoryStream::*)(const usize&, const usize&)>(&ReadMemoryStream::string)>("string_of"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::boolean>("boolean"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::bytes>("bytes"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::current_position>("current_position"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::f32>("f32"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::f64>("f64"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::i8>("i8"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::u8>("u8"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::i16>("i16"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::u16>("u16"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::i32>("i32"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::u32>("u32"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::i64>("i64"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::u64>("u64"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::string_null_terminator>("string_null_terminator"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::string_v32>("string_v32"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::v32>("v32"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::v64>("v64"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::z32>("z32"_s)
            .add_member_function_by_proxy<&ReadMemoryStream::z64>("z64"_s);
        }
        {
            auto s_OperatingSystem = s_Kernel.add_space("OperatingSystem"_s);
            s_OperatingSystem.add_function_by_proxy<&API::OperatingSystem::current>("current"_s);
        }
        {
            auto s_Process = s_Kernel.add_space("Process"_s);
            s_Process.add_function_by_proxy<&Process::execute>("execute"_s)
            .add_function_by_proxy<&Process::run>("run"_s)
            .add_function_by_proxy<&Process::is_exists_in_path_environment>("is_exists_in_path_environment"_s)
            .add_function_by_proxy<&Process::get_path_environment>("get_path_environment"_s);
        }
        {
            auto s_JavaScript = s_Kernel.add_space("JavaScript"_s);
            s_JavaScript.add_function<&API::JavaScript::evaluate_fs>("evaluate_fs"_s);
        }
        const auto s_Support = s_Kernel.add_space("Support"_s);
        {
            auto s_Texture = s_Support.add_space("Texture"_s);
            s_Texture.add_function_by_proxy<&Support::Texture::InvokeMethod::Decode::process_fs>("decode_fs"_s)
            .add_function_by_proxy<&Support::Texture::InvokeMethod::Encode::process_fs>("encode_fs"_s);
        }
        const auto s_PopCap = s_Support.add_space("PopCap"_s);
        {
            auto s_ReflectionObjectNotation = s_PopCap.add_space("ReflectionObjectNotation"_s);
            s_ReflectionObjectNotation.add_function_by_proxy<&Support::PopCap::ReflectionObjectNotation::Encode::process_fs>("encode_fs"_s)
            .add_function_by_proxy<&Support::PopCap::ReflectionObjectNotation::Decode::process_fs>("decode_fs"_s)
            .add_function_by_proxy<&Support::PopCap::ReflectionObjectNotation::Encrypt::process_fs>("encrypt_fs"_s)
            .add_function_by_proxy<&Support::PopCap::ReflectionObjectNotation::Decrypt::process_fs>("decrypt_fs"_s);
        }
        {
            auto s_Zlib = s_PopCap.add_space("Zlib"_s);
            s_Zlib.add_function_by_proxy<&Support::PopCap::Zlib::Compress::process_fs>("compress_fs"_s)
            .add_function_by_proxy<&Support::PopCap::Zlib::Uncompress::process_fs>("uncompress_fs"_s);
        }
        {
            auto s_CompiledText = s_PopCap.add_space("CompiledText"_s);
            s_CompiledText.add_function_by_proxy<&Support::PopCap::CompiledText::Decode::process_fs>("decode_fs"_s)
            .add_function_by_proxy<&Support::PopCap::CompiledText::Encode::process_fs>("encode_fs"_s);
        }
        {
            auto s_CharacterFontWidget2 = s_PopCap.add_space("CharacterFontWidget2"_s);
            s_CharacterFontWidget2.add_function_by_proxy<&Support::PopCap::CharacterFontWidget2::Encode::process_fs>("encode_fs"_s)
            .add_function_by_proxy<&Support::PopCap::CharacterFontWidget2::Decode::process_fs>("decode_fs"_s);
        }
        {
            auto s_CryptData = s_PopCap.add_space("CryptData"_s);
            s_CryptData.add_function_by_proxy<&Support::PopCap::CryptData::Encrypt::process_fs>("encrypt_fs"_s)
            .add_function_by_proxy<&Support::PopCap::CryptData::Decrypt::process_fs>("decrypt_fs"_s);
        }
        {
            auto s_Package = s_PopCap.add_space("Package"_s);
            s_Package.add_function_by_proxy<&Support::PopCap::Package::Unpack::process_fs>("unpack_fs"_s)
            .add_function_by_proxy<&Support::PopCap::Package::Pack::process_fs>("pack_fs"_s);
        }
        {
            auto s_NewTypeObjectNotation = s_PopCap.add_space("NewTypeObjectNotation"_s);
            s_NewTypeObjectNotation.add_function_by_proxy<&Support::PopCap::NewTypeObjectNotation::Encode::process_fs>("encode_fs"_s)
            .add_function_by_proxy<&Support::PopCap::NewTypeObjectNotation::Decode::process_fs>("decode_fs"_s);
        }
        {
            auto s_Animation = s_PopCap.add_space("Animation"_s);
            s_Animation.add_function_by_proxy<&Support::PopCap::Animation::Decode::process_fs>("decode_fs"_s)
            .add_function_by_proxy<&Support::PopCap::Animation::Encode::process_fs>("encode_fs"_s);
        }
        {
            auto s_ResourceStreamGroup = s_PopCap.add_space("ResourceStreamGroup"_s);
            s_ResourceStreamGroup.add_function_by_proxy<&Support::PopCap::ResourceStreamGroup::Unpack::process_fs>("unpack_fs"_s)
            .add_function_by_proxy<&Support::PopCap::ResourceStreamGroup::Pack::process_fs>("pack_fs"_s);
        }
        {
            auto s_ResourceStreamBundle = s_PopCap.add_space("ResourceStreamBundle"_s);
            s_ResourceStreamBundle.add_function_by_proxy<&Support::PopCap::ResourceStreamBundle::Unpack::process_fs>("unpack_fs"_s)
            .add_function_by_proxy<&Support::PopCap::ResourceStreamBundle::Pack::process_fs>("pack_fs"_s);
        }
        {
            // auto s_ReAnimation = s_PopCap.add_space("ReAnimation"_s);
            // s_ReAnimation.add_function_by_proxy<&Support::PopCap::ReAnimation::Decode::process_fs>("decode_fs"_s)
            // .add_function_by_proxy<&Support::PopCap::ReAnimation::Encode::process_fs>("encode_fs"_s)
            // .add_function_by_proxy<&Support::PopCap::ReAnimation::ToXML::process_fs>("to_xml"_s);
        }
        {
            auto s_TextTable = s_PopCap.add_space("TextTable"_s);
            s_TextTable.add_function_by_proxy<&Support::PopCap::TextTable::Convert::process_fs>("convert_fs"_s);
        }
        {
            auto s_Miscellaneous = s_Kernel.add_space("Miscellaneous"_s);
            s_Miscellaneous.add_function<&API::Miscellaneous::deep_copy>("deep_copy"_s);
        }
    }

    template <typename... Args>
    inline auto make_exception(
        Args&&... message
    ) -> CArray<String> {
        return make_array<String>(
            "display"_s,
            (message)...,
            "red"_s
        );
    }

    inline auto make_runtime_exception(
    ) -> CArray<String> {
        return make_exception(
            std::forward<String>(StringHelper::make_string(parse_exception()->what()))
        );
    }

    inline auto make_unhandled_exception(
    ) -> CArray<String> {
        return make_exception(
            std::forward<String>(StringHelper::make_string(fmt::format("Unhandled exception: {}", "Kernel is about to crash, but was prevented.")))
        );
    }

    inline auto bind_service (
        Service* service
    ) -> void {
        service->allocate = [](Message* message, size_t* size) -> int {
            message->value = new uint8_t[*size];
            message->size = *size;
            return 0;
        };
    }

    inline auto unbind_service (
        Service* service
    ) -> void {
        service->allocate = nullptr;
    }

}		