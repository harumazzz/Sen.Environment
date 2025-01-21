#pragma once

#include "kernel/interface/script.hpp"
#include "kernel/interface/api/method.hpp"
#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/interface/shell.hpp"

namespace Sen::Kernel::Interface {

	namespace JS = Sen::Kernel::JavaScript;
	
	template <typename ReturnType, typename... Args>
	using FunctionProxy = JS::FunctionProxy<ReturnType, Args...>;

	template <typename ReturnType, typename... Args>
	using SpecialFunctionProxy = JS::SpecialFunctionProxy<ReturnType, Args...>;

	using Integer = int;

	class Runtime {

		private:

			JS::Engine runtime;

			JS::NamespaceBuilder builder;

		public:

			explicit Runtime(
			) : runtime{}, builder{runtime.context().value}
			{
				runtime.set_context_opaque(&runtime);
			}

			~Runtime(

			) = default;

			inline auto prepare(
			) -> void
			{
				auto sen = builder.add_space("Sen");
				auto shell = sen.add_space("Shell");
				auto kernel = sen.add_space("Kernel");
				{
					shell.add_function("is_gui"_sv, FunctionProxy<bool>::template as_function<Kernel::Interface::API::Shell::is_gui>)
						.add_function("version"_sv, FunctionProxy<int>::template as_function<Kernel::Interface::API::Shell::version>)
						.add_function("callback"_sv, SpecialFunctionProxy<JSValue, JSValue&>::template as_function<Interface::API::Shell::callback>);
				}
				{
					kernel.add_function("version"_sv, FunctionProxy<int>::template as_function<Kernel::Interface::API::version>)
						.add_function("arguments"_sv, SpecialFunctionProxy<JSValue>::template as_function<Interface::API::arguments>);
				}
				auto xml = kernel.add_space("XML");
				// TODO : Later
				{
					xml.add_function("deserialize"_sv, Script::XML::deserialize)
						.add_function("deserialize_fs"_sv, Script::XML::deserialize_fs)
						.add_function("serialize"_sv, Script::XML::serialize)
						.add_function("serialize_fs"_sv, Script::XML::serialize_fs);
				}
				auto json = kernel.add_space("JSON");
				{
					json.add_function("deserialize"_sv, SpecialFunctionProxy<JSValue, std::string&>::template as_function<Interface::API::JSON::deserialize>)
					.add_function("serialize"_sv, SpecialFunctionProxy<std::string, JSValue&, int64_t&, bool>::template as_function<Interface::API::JSON::serialize>)
					.add_function("deserialize_fs"_sv, SpecialFunctionProxy<JSValue, std::string&>::template as_function<Interface::API::JSON::deserialize_fs>)
					.add_function("serialize_fs"_sv, SpecialFunctionProxy<void, std::string&, JSValue&, int64_t&, bool>::template as_function<Interface::API::JSON::serialize_fs>);
				}
				auto home = kernel.add_space("Home");
				{
					home.add_function("script"_sv, FunctionProxy<std::string>::template as_function<Interface::API::script>);
				}
				auto filesystem = kernel.add_space("FileSystem");
				{
					filesystem.add_function("read_file"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::FileSystem::read_file>)
					.add_function("read_file_encode_with_utf16le"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::FileSystem::read_file_encode_with_utf16le>)
					.add_function("write_file"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::FileSystem::write_file>)
					.add_function("write_file_encode_with_utf16le"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::FileSystem::write_file_encode_with_utf16le>)
					.add_function("read_current_directory"_sv, FunctionProxy<List<std::string>, std::string&>::template as_function<Interface::API::FileSystem::read_current_directory>)
					.add_function("read_directory_only_file"_sv, FunctionProxy<List<std::string>, std::string&>::template as_function<Interface::API::FileSystem::read_directory_only_file>)
					.add_function("read_directory_only_directory"_sv, FunctionProxy<List<std::string>, std::string&>::template as_function<Interface::API::FileSystem::read_directory_only_directory>)
					.add_function("read_directory"_sv, FunctionProxy<List<std::string>, std::string&>::template as_function<Interface::API::FileSystem::read_directory>)
					.add_function("create_directory"_sv, FunctionProxy<void, std::string&>::template as_function<Interface::API::FileSystem::create_directory>)
					.add_function("is_file"_sv, FunctionProxy<bool, std::string&>::template as_function<Interface::API::FileSystem::is_file>)
					.add_function("is_directory"_sv, FunctionProxy<bool, std::string&>::template as_function<Interface::API::FileSystem::is_directory>)
					.add_function("rename"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::FileSystem::rename>)
					.add_function("remove"_sv, FunctionProxy<void, std::string&>::template as_function<Interface::API::FileSystem::remove>)
					.add_function("remove_all"_sv, FunctionProxy<void, std::string&>::template as_function<Interface::API::FileSystem::remove_all>)
					.add_function("copy"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::FileSystem::copy>)
					.add_function("copy_directory"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::FileSystem::copy_directory>);
				}
				auto path = kernel.add_space("Path");
				{
					path.add_function("join"_sv, FunctionProxy<std::string, List<std::string>&>::template as_function<Interface::API::Path::join>)
					.add_function("basename"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::basename>)
					.add_function("delimiter"_sv, FunctionProxy<std::string>::as_function<Interface::API::Path::delimiter>)
					.add_function("dirname"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::dirname>)
					.add_function("normalize"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::normalize>)
					.add_function("resolve"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::resolve>)
					.add_function("relative"_sv, FunctionProxy<std::string, std::string&, std::string&>::template as_function<Interface::API::Path::relative>)
					.add_function("extname"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::extname>)
					.add_function("is_absolute"_sv, FunctionProxy<bool, std::string&>::template as_function<Interface::API::Path::is_absolute>)
					.add_function("base_without_extension"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::base_without_extension>)
					.add_function("except_extension"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Path::except_extension>);
				}
				auto console = kernel.add_space("Console");
				{
					console.add_function("print"_sv, FunctionProxy<void, List<std::string>&>::template as_function<Interface::API::Console::print>)
					.add_function("readline"_sv, SpecialFunctionProxy<JSValue>::as_function<Interface::API::Console::readline>);
				}
				auto language = kernel.add_space("Language");
				{
					language.add_function("load_language"_sv, FunctionProxy<void, std::string&>::template as_function<Interface::API::Language::load_language>)
					.add_function("get"_sv, FunctionProxy<std::string, std::string&>::as_function<Interface::API::Language::get>);
				}
				auto process = kernel.add_space("Process");
				{
					process.add_function("run"_sv, FunctionProxy<void, std::string&>::template as_function<Interface::API::Process::run>)
					.add_function("execute"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Process::execute>)
					.add_function("is_exists_in_path_environment"_sv, FunctionProxy<bool, std::string&>::template as_function<Interface::API::Process::is_exists_in_path_environment>)
					.add_function("get_path_environment"_sv, FunctionProxy<std::string, std::string&>::template as_function<Interface::API::Process::get_path_environment>);
				}
				auto arrayBuffer = kernel.add_space("ArrayBuffer");
				{
					arrayBuffer.add_function("random"_sv, FunctionProxy<void, std::shared_ptr<JS::ArrayBuffer>&>::template as_function<Interface::API::ArrayBuffer::random>)
					.add_function("cast_ArrayBuffer_to_JS_String"_sv, FunctionProxy<std::string, std::shared_ptr<JS::ArrayBuffer>&>::template as_function<Interface::API::Miscellaneous::cast_ArrayBuffer_to_JS_String>)
					.add_function("cast_ArrayBuffer_to_JS_WideString"_sv, FunctionProxy<std::string, std::shared_ptr<JS::ArrayBuffer>&>::template as_function<Interface::API::Miscellaneous::cast_ArrayBuffer_to_JS_WideString>)
					.add_function("cast_movable_String_to_ArrayBuffer"_sv, FunctionProxy<std::shared_ptr<JS::ArrayBuffer>, std::string&>::template as_function<Interface::API::Miscellaneous::cast_movable_String_to_ArrayBuffer>)
					.add_function("copyArrayBuffer"_sv, FunctionProxy<std::shared_ptr<JS::ArrayBuffer>, std::shared_ptr<JS::ArrayBuffer>&>::template as_function<Interface::API::Miscellaneous::copyArrayBuffer>)
					.add_function("compareArrayBuffer"_sv, FunctionProxy<bool, std::shared_ptr<JS::ArrayBuffer>&, std::shared_ptr<JS::ArrayBuffer>&>::template as_function<Interface::API::Miscellaneous::compareArrayBuffer>);
				}
				auto operatingSystem = kernel.add_space("OperatingSystem");
				{
					operatingSystem.add_function("current"_sv, FunctionProxy<std::string>::template as_function<Interface::API::OperatingSystem::current>);
				}
				auto image = kernel.add_space("Image");
				{
					image.add_function("join"_sv, FunctionProxy<std::shared_ptr<JavaScript::ImageView>, std::shared_ptr<JavaScript::Dimension>&, List<std::shared_ptr<JavaScript::VImageView>>&>::template as_function<Interface::API::Image::join>)
					.add_function("join_extend"_sv, FunctionProxy<std::shared_ptr<JavaScript::ImageView>, std::shared_ptr<JavaScript::Dimension>&, List<std::shared_ptr<JavaScript::VImageView>>&>::template as_function<Interface::API::Image::join_extend>)
					.add_function("resize_fs"_sv, FunctionProxy<void, std::string&, std::string&, float&>::template as_function<Interface::API::Image::resize_fs>)
					.add_function("cut_multiple_fs"_sv, FunctionProxy<void, std::string&, List<std::shared_ptr<JavaScript::ExtendedRectangle>>&>::template as_function<Interface::API::Image::cut_multiple_fs>)
					.add_function("cut_multiple_fs_asynchronous"_sv, FunctionProxy<void, std::string&, List<std::shared_ptr<JavaScript::ExtendedRectangle>>&>::template as_function<Interface::API::Image::cut_multiple_fs_asynchronous>)
					.add_function("open"_sv, FunctionProxy<std::shared_ptr<JavaScript::ImageView>, std::string&>::template as_function<Interface::API::Image::open>)
					.add_function("write"_sv, FunctionProxy<void, std::string&, std::shared_ptr<JavaScript::ImageView>&>::template as_function<Interface::API::Image::write>);
				}
				auto javascript = kernel.add_space("JavaScript");
				{
					javascript.add_function("evaluate_fs"_sv, SpecialFunctionProxy<JSValue, std::string&>::template as_function<Interface::API::JS::evaluate_fs>);
				}
				auto compression = kernel.add_space("Compression");
				auto zip = compression.add_space("Zip");
				{
					zip.add_function("process"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Compression::Zip::Uncompress::process>);
				}
				auto zlib = compression.add_space("Zlib");
				{
					zlib.add_function("uncompress"_sv, FunctionProxy<std::shared_ptr<JS::ArrayBuffer>, std::shared_ptr<JS::ArrayBuffer>&>::template as_function<Interface::API::Compression::Zlib::uncompress>);
				}
				auto support = kernel.add_space("Support");
				auto texture = support.add_space("Texture");
				{
					texture.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, int64_t&, int64_t&, int64_t&>::template as_function<Interface::API::Support::Texture::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&, int64_t&>::template as_function<Interface::API::Support::Texture::encode_fs>);
				}
				auto marmalade = support.add_space("Marmalade");
				auto dzip = marmalade.add_space("DZip");
				{
					dzip.add_function("unpack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::Marmalade::DZip::unpack_fs>)
					.add_function("pack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::Marmalade::DZip::pack_fs>);
				}
				auto popcap = support.add_space("PopCap");
				auto package = popcap.add_space("Package");
				{
					package.add_function("unpack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Package::unpack_fs>)
					.add_function("pack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Package::pack_fs>);
				}
				auto newTypeObjectNotation = popcap.add_space("NewTypeObjectNotation");
				{
					newTypeObjectNotation.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::NewTypeObjectNotation::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::NewTypeObjectNotation::encode_fs>);
				}
				auto reflectionObjectNotation = popcap.add_space("ReflectionObjectNotation");
				{
					reflectionObjectNotation.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReflectionObjectNotation::decode_fs>)
					.add_function("decrypt_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReflectionObjectNotation::decrypt_fs>)
					.add_function("decrypt_and_decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReflectionObjectNotation::decrypt_and_decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReflectionObjectNotation::encode_fs>)
					.add_function("encrypt_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReflectionObjectNotation::encrypt_fs>)
					.add_function("encode_and_encrypt_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReflectionObjectNotation::encode_and_encrypt_fs>);
				}
				auto pZlib = popcap.add_space("Zlib");
				{
					pZlib.add_function("uncompress_fs"_sv, FunctionProxy<void, std::string&, std::string&, bool>::template as_function<Interface::API::Support::PopCap::Zlib::uncompress_fs>)
					.add_function("compress_fs"_sv, FunctionProxy<void, std::string&, std::string&, bool>::template as_function<Interface::API::Support::PopCap::Zlib::compress_fs>);
				}
				auto compiledText = popcap.add_space("CompiledText");
				{
					compiledText.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&, std::string&, bool>::template as_function<Interface::API::Support::PopCap::CompiledText::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&, std::string&, bool>::template as_function<Interface::API::Support::PopCap::CompiledText::encode_fs>);
				}
				auto renderEffects = popcap.add_space("RenderEffects");
				{
					renderEffects.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::RenderEffects::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::RenderEffects::encode_fs>);
				}
				auto cryptData = popcap.add_space("CryptData");
				{
					cryptData.add_function("decrypt_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::CryptData::decrypt_fs>)
					.add_function("encrypt_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::CryptData::encrypt_fs>);
				}
				auto characterFontWidget2 = popcap.add_space("CharacterFontWidget2");
				{
					characterFontWidget2.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::CharacterFontWidget2::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::CharacterFontWidget2::encode_fs>);
				}
				auto particles = popcap.add_space("Particles");
				{
					particles.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Particles::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Particles::encode_fs>)
					.add_function("to_xml"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Particles::to_xml>)
					.add_function("from_xml"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Particles::from_xml>);
				}
				auto playerInfo = popcap.add_space("PlayerInfo");
				{
					playerInfo.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::PlayerInfo::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::PlayerInfo::encode_fs>);
				}
				auto resourceStreamBundle = popcap.add_space("ResourceStreamBundle");
				{
					resourceStreamBundle.add_function("unpack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundle::unpack_fs>)
					.add_function("pack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundle::pack_fs>)
					.add_function("unpack_cipher"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundle::unpack_cipher>)
					.add_function("unpack_resource"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundle::unpack_resource>)
					.add_function("pack_resource"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundle::pack_resource>);
				}
				auto resourceStreamBundlePatch = popcap.add_space("ResourceStreamBundlePatch");
				{
					resourceStreamBundlePatch.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundlePatch::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamBundlePatch::encode_fs>);
				}
				auto resourceStreamGroup = popcap.add_space("ResourceStreamGroup");
				{
					resourceStreamGroup.add_function("unpack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamGroup::unpack_fs>)
					.add_function("pack_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceStreamGroup::pack_fs>);
				}
				auto animation = popcap.add_space("Animation");
				auto aMiscellaneous = animation.add_space("Miscellaneous");
				{
					animation.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Animation::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::Animation::encode_fs>)
					.add_function("to_flash"_sv, FunctionProxy<void, std::string&, std::string&, int64_t&, bool>::template as_function<Interface::API::Support::PopCap::Animation::to_flash>)
					.add_function("from_flash"_sv, FunctionProxy<void, std::string&, std::string&, bool>::template as_function<Interface::API::Support::PopCap::Animation::from_flash>)
					.add_function("decode_and_to_flash"_sv, FunctionProxy<void, std::string&, std::string&, int64_t&, bool>::template as_function<Interface::API::Support::PopCap::Animation::decode_and_to_flash>)
					.add_function("from_flash_and_encode"_sv, FunctionProxy<void, std::string&, std::string&, bool>::template as_function<Interface::API::Support::PopCap::Animation::from_flash_and_encode>);
					aMiscellaneous.add_function("resize_fs"_sv, FunctionProxy<void, std::string&, int64_t&>::template as_function<Interface::API::Support::PopCap::Animation::Miscellaneous::resize_fs>)
					.add_function("dump_document"_sv, SpecialFunctionProxy<void, std::string&, JSValue&>::template as_function<Interface::API::Support::PopCap::Animation::Miscellaneous::dump_document>)
					.add_function("generate_document"_sv, FunctionProxy<void, std::string&, std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument>&>::template as_function<Interface::API::Support::PopCap::Animation::Miscellaneous::generate_document>)
					.add_function("generate_image"_sv, FunctionProxy<void, std::string&, std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::Image>&>::template as_function<Interface::API::Support::PopCap::Animation::Miscellaneous::generate_image>)
					.add_function("generate_sprite"_sv, FunctionProxy<void, std::string&, std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::Sprite>&>::template as_function<Interface::API::Support::PopCap::Animation::Miscellaneous::generate_sprite>);
				}
				auto reAnimation = popcap.add_space("ReAnimation");
				{
					reAnimation.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::encode_fs>)
					.add_function("to_xml"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::to_xml>)
					.add_function("from_xml"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::from_xml>)
					.add_function("to_flash"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::to_flash>)
					.add_function("from_flash"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::from_flash>)
					.add_function("decode_and_to_flash"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::decode_and_to_flash>)
					.add_function("from_flash_and_encode"_sv, FunctionProxy<void, std::string&, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ReAnimation::from_flash_and_encode>);
				}
				auto wwise = support.add_space("WWise");
				auto soundBank = wwise.add_space("SoundBank");
				{
					soundBank.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::WWise::SoundBank::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::WWise::SoundBank::encode_fs>)
					.add_function("hash"_sv, FunctionProxy<uint32_t, std::string&>::template as_function<Interface::API::Support::WWise::SoundBank::hash>);
				}
				auto resourceGroup = popcap.add_space("ResourceGroup");
				{
					resourceGroup.add_function("split_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceGroup::split_fs>)
					.add_function("merge_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResourceGroup::merge_fs>)
					.add_function("convert_fs"_sv, FunctionProxy<void, std::string&, std::string&, int64_t&>::template as_function<Interface::API::Support::PopCap::ResourceGroup::convert_fs>);
				}
				auto resInfo = popcap.add_space("ResInfo");
				{
					resInfo.add_function("split_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResInfo::split_fs>)
					.add_function("merge_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResInfo::merge_fs>)
					.add_function("convert_fs"_sv, FunctionProxy<void, std::string&, std::string&>::template as_function<Interface::API::Support::PopCap::ResInfo::convert_fs>);
				}
				auto project = support.add_space("Project");
				auto pStreamCompressedGroup = project.add_space("StreamCompressedGroup");
				auto pResourceStreamBundle = project.add_space("ResourceStreamBundle");
				{
					pStreamCompressedGroup.add_function("test_scg"_sv, FunctionProxy<bool, std::string&>::template as_function<Interface::API::Support::Project::StreamCompressedGroup::check_scg_composite>)
					.add_function("decode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::shared_ptr<Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Setting>&>::template as_function<Interface::API::Support::Project::StreamCompressedGroup::decode_fs>)
					.add_function("encode_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::shared_ptr<Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Setting>&>::template as_function<Interface::API::Support::Project::StreamCompressedGroup::encode_fs>);
					pResourceStreamBundle.add_function("unpack_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::shared_ptr<Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Setting>&>::template as_function<Interface::API::Support::Project::ResourceStreamBundle::unpack_fs>)
					.add_function("pack_fs"_sv, FunctionProxy<void, std::string&, std::string&, std::shared_ptr<Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Setting>&>::template as_function<Interface::API::Support::Project::ResourceStreamBundle::pack_fs>);
				}
				auto miscellaneous = kernel.add_space("Miscellaneous");
				{
					miscellaneous.add_function("make_copy"_sv, SpecialFunctionProxy<JSValue, JSValue&>::as_function<Interface::API::Miscellaneous::make_copy>)
					.add_function("to_apng"_sv, FunctionProxy<void, List<std::string>&, std::string&, std::shared_ptr<Kernel::APNGMakerSetting>&>::template as_function<Interface::API::Miscellaneous::to_apng>);
				}
				auto DataStreamView = JavaScript::ClassBuilder<Kernel::DataStreamView>{ runtime.context().value, "DataStreamView" };
				Interface::API::DataStreamView::register_class(DataStreamView, kernel);
				auto Clock = JavaScript::ClassBuilder<Kernel::Clock>{ runtime.context().value, "Clock" };
				auto ImageView = JavaScript::ClassBuilder<Kernel::Image<Integer>>{ runtime.context().value, "ImageView" };
				Interface::API::ImageView::register_class(ImageView, kernel);
				auto Canvas = JavaScript::ClassBuilder<canvas_ity::canvas>{ runtime.context().value, "Canvas" };
				Interface::API::Canvas::register_class(Canvas, kernel);
				auto JsonWriter = JavaScript::ClassBuilder<Kernel::JsonWriter>{ runtime.context().value, "JsonWriter" };
				Interface::API::JsonWriter::register_class(JsonWriter, kernel);
				Interface::API::Clock::register_class(Clock, kernel);
				// execute the script
				runtime.evaluate_fs(construct_string(Executor::script));
				// call main
				runtime.evaluate("Sen.Script.main()"_sv, std::source_location::current().file_name());
				// Execute other Promise
				thiz.execute_promise_in_queue();
				#ifdef DEBUG
					runtime.dump_memory_usage();
				#endif
				return;
			}

			// Execute other Promise if quickjs found any

			inline auto execute_promise_in_queue (

			) -> void
			{
				while (runtime.has_promise()) {
					runtime.execute_pending_job();
				}
				return;
			}

			/**
			 * Execute method
			*/

			inline auto execute(

			) -> void
			{
				return;
			}
	};
}		