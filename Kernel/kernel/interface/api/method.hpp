#pragma once

#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"

namespace Sen::Kernel::Interface::API {

	#pragma region proxy

	template <typename Callable> requires std::is_invocable<Callable>::value
	inline auto proxy_wrapper(
		JSContext* context,
		std::string_view func_name,
		Callable&& callable
	) -> JSValue {
		auto result = JS_UNDEFINED;
		try {
			result = JavaScript::Builder<Callable>::make_function_declaration(context, std::forward<Callable>(callable));
		}
		catch (...) {
			auto exception = parse_exception();
			if (exception.function_name.empty()) {
				exception.function_name = func_name;
			}
			result = JavaScript::throw_exception(context, exception.message(), exception.source, exception.function_name);
		}
		return result;
	}

	#pragma endregion

	#pragma region shell

	namespace Shell {

		inline auto is_gui (

		) -> bool {
			auto is_gui = std::unique_ptr<CStringView, StringFinalizer>(new CStringView{nullptr, 0}, finalizer<CStringView>);
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList{nullptr, 0}, finalizer<CStringList>);
			construct_string_list(std::to_array<std::string>({std::string{"is_gui"}}), parameters.operator*());
			Interface::Shell::callback(parameters.get(), is_gui.get());
			auto result = static_cast<bool>(StringParser::to_int32(String{reinterpret_cast<const char*>(is_gui->value), static_cast<std::size_t>(is_gui->size)}, "Cannot get is gui argument from Shell"));
			return result;
		}
		
		inline auto version (

		) -> int {
			auto shell_version = std::unique_ptr<CStringView, StringFinalizer>(new CStringView{nullptr, 0}, finalizer<CStringView>);
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList{nullptr, 0}, finalizer<CStringList>);
			construct_string_list(std::to_array<std::string>({std::string{"version"}}), parameters.operator*());
			Interface::Shell::callback(parameters.get(), shell_version.get());
			auto result = static_cast<int>(StringParser::to_int32(String{ reinterpret_cast<const char*>(shell_version->value), static_cast<std::size_t>(shell_version->size) }, "Cannot get the Shell version"));
			return result;
		}

		inline auto callback(
			JSContext* context,
			JSValue& value
		) -> JSValue {
			auto destination = std::unique_ptr<CStringView, StringFinalizer>{ new CStringView{nullptr, 0}, finalizer<CStringView> };
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>{ new CStringList{nullptr, 0}, finalizer<CStringList> };
			JavaScript::to_string_list<CStringList, CStringView>(context, value, parameters.operator*());
			Interface::Shell::callback(parameters.get(), destination.get());
			return JavaScript::to_string<CStringView>(context, destination.get());
		}
			
	}

	#pragma endregion

	#pragma region information

	inline auto version(

	) -> int {
		return Kernel::version;
	}

	inline auto arguments(
		JSContext* context
	) -> JSValue {
		return JavaScript::to_array_of_string<CStringList, CStringView>(context, Executor::arguments);
	}

	inline auto script(

	) -> String {
		return construct_string(Executor::script);
	}

	#pragma endregion

	#pragma region json

	namespace JSON {

		// inline auto deserialize(
		// 	JSContext* context,
		// 	std::string& source
		// ) -> JSValue {
		// 	auto json = nlohmann::ordered_json::parse(source);
		// 	auto object = JavaScript::to(context, json);
		// 	return object;
		// }

		// inline auto deserialize_fs(
		// 	JSContext* context,
		// 	std::string& source
		// ) -> JSValue {
		// 	auto json = Kernel::FileSystem::read_json(source);
		// 	auto object = JavaScript::to(context, json);
		// 	return object;
		// }

		// inline auto serialize(
		// 	JSContext* context,
		// 	JSValue& value,
		// 	int64_t& indent,
		// 	bool ensure_ascii
		// ) -> std::string {
		// 	auto reference_value = JavaScript::Value::as_new_reference(context, value);
		// 	auto json = JavaScript::from(reference_value);
		// 	return json.dump(indent, '\t', ensure_ascii);
		// }

		// inline auto serialize_fs(
		// 	JSContext* context,
		// 	std::string& destination,
		// 	JSValue& value,
		// 	int64_t& indent,
		// 	bool ensure_ascii
		// ) -> void {
		// 	auto reference_value = JavaScript::Value::as_new_reference(context, value);
		// 	auto json = JavaScript::from(reference_value);
		// 	auto result = json.dump(indent, '\t', ensure_ascii);
		// 	return Kernel::FileSystem::write_file(destination, result);
		// }

	}

	#pragma endregion

	#pragma region filesystem

	namespace FileSystem {

	}
	#pragma endregion

	#pragma region path

	namespace Path {

		inline auto join(
			List<std::string>& source
		) -> std::string
		{
			// TODO : Fix
			// return Kernel::Path::join(source);
			return {};
		}

		inline auto basename(
			const String& source
		) -> String
		{
			return Kernel::Path::basename(source);
		}

		inline auto dirname(
			const String& source
		) -> String
		{
			return Kernel::Path::dirname(source);
		}

		inline auto normalize(
			const String& source
		) -> String
		{
			return Kernel::Path::normalize(source);
		}

		inline auto base_without_extension(
			const String& source
		) -> String
		{
			return Kernel::Path::base_without_extension(source);
		}

		inline auto except_extension(
			const String& source
		) -> String
		{
			return Kernel::Path::except_extension(source);
		}

		inline auto resolve(
			const String& source
		) -> String
		{
			return Kernel::Path::resolve(source);
		}

		inline auto extname(
			const String& source
		) -> String
		{
			return Kernel::Path::extname(source);
		}

	}

	#pragma endregion

	#pragma region console

	namespace Console {


		inline auto print(
			List<std::string>& data
		) -> void
		{
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
			switch (data.size())
			{
				case 1:
				{
					construct_string_list(std::to_array<std::string>({ std::string{ "display" }, data[0]}), parameters.operator*());
					Interface::Shell::callback(parameters.get(), nullptr);
					break;
				}
				case 2:
				{
					construct_string_list(std::to_array<std::string>({ std::string{ "display" }, data[0], data[1] }), parameters.operator*());
					Interface::Shell::callback(parameters.get(), nullptr);
					break;
				}
				default:
				{
					construct_string_list(std::to_array<std::string>({ std::string{ "display" }, data[0], data[1], data[2]}), parameters.operator*());
					Interface::Shell::callback(parameters.get(), nullptr);
					break;
				}
			}
		}

		inline auto readline(
			JSContext* context
		) -> JSValue
		{
			auto wait_parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList{nullptr, 0}, finalizer<CStringList>);
			construct_string_list(std::to_array<std::string>({ std::string{ "wait" } }), wait_parameters.operator*());
			Interface::Shell::callback(wait_parameters.get(), nullptr);
			auto destination = std::unique_ptr<CStringView, StringFinalizer>(new CStringView{nullptr, 0}, finalizer<CStringView>);
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList{nullptr, 0}, finalizer<CStringList>);
			construct_string_list(std::array<std::string, 1>{std::string{ "input" }}, parameters.operator*());
			Interface::Shell::callback(parameters.get(), destination.get());
			auto result = JavaScript::to_string<CStringView>(context, destination.get());
			return result;
		}

	}
	#pragma endregion

	#pragma region language
	
	namespace Language {

		inline auto load_language (
			std::string& source
		) -> void {
			return Kernel::Language::read_language(source);
		}

		inline auto get (
			std::string& key
		) -> std::string {
			return Kernel::Language::get(key);
		}

	}
	#pragma endregion

	#pragma region process

	namespace Process {

		inline auto run(
			std::string& command
		) -> void
		{
			return Kernel::Process::run(command);
		}

		inline auto execute(
			std::string& command
		) -> std::string
		{
			return Kernel::Process::execute(command);
		}

		inline auto is_exists_in_path_environment(
			std::string& source
		) -> bool
		{
			return Kernel::Process::is_exists_in_path_environment(source);
		}

		inline auto get_path_environment(
			std::string& value
		) -> std::string
		{
			return Kernel::Process::get_path_environment(value);
		}
	}

	#pragma endregion

	#pragma region thread

	namespace Thread {

		inline auto sleep(
			int64_t& time
		) -> void
		{
			return std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(time)));
		}

		inline auto now(
		) -> double
		{
			return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

	}
	#pragma endregion

	#pragma region arraybuffer

	namespace ArrayBuffer {

		inline auto random(
			std::shared_ptr<JavaScript::ArrayBuffer>& data
		) -> void
		{
			auto rd = std::random_device{};
			auto gen = std::mt19937{ rd() };
			auto dist = std::uniform_int_distribution<unsigned int>(0, 255);
			for (auto i : Range<std::size_t>{ data->size }) {
				data->value[i] = static_cast<uint8_t>(dist(gen));
			}
		}

	}

	#pragma endregion

	#pragma region operating system

	namespace OperatingSystem {

		inline auto current(
		) -> std::string
		{
			#if WINDOWS
			return std::string{ "Windows" };
			#elif ANDROID
			return std::string{ "Android" };
			#elif LINUX
			return std::string{ "Linux" };
			#elif APPLE
			return std::string{ "Macintosh" };
			#elif IPHONE
			return std::string{ "iPhone" };
			#else
			assert_conditional(false, "Unknown operating system", "current");
			#endif
		}

	}

	#pragma endregion

	#pragma region image

	namespace Image {
	}

	#pragma endregion

	#pragma region javascript

	namespace JS {

		inline auto evaluate_fs(
			Pointer<JSContext> context,
			std::string& source
		) -> JSValue
		{
			auto engine = static_cast<JavaScript::Engine*>(JS_GetContextOpaque(context));
			return engine->evaluate_fs<JavaScript::Value, JavaScript::Error>(source);
		}

	}

	#pragma endregion

	#pragma region compression

	namespace Compression {

		#pragma region zip

		namespace Zip {

			#pragma region uncompress

			namespace Uncompress {

				inline auto process(
					std::string& source,
					std::string& destination
				) -> void
				{
					return
						Kernel::Compression::Zip::Uncompress::process(source, destination);
				}
			}

			#pragma endregion

		}

		#pragma endregion

		#pragma region zlib

		namespace Zlib {

			inline auto uncompress(
				std::shared_ptr<JavaScript::ArrayBuffer>& source
			) -> std::shared_ptr<JavaScript::ArrayBuffer>
			{
				auto buffer = List<unsigned char>{};
				buffer.assign(source->value, source->value + source->size);
				auto result = Kernel::Compression::Zlib::uncompress(buffer);
				return std::make_shared<JavaScript::ArrayBuffer>(reinterpret_cast<uint8_t*>(result.data()), result.size());
			}

		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region support
    /*
	namespace Support {

		#pragma region texture

		namespace Texture {

			inline auto decode_fs(
				std::string& source,
				std::string& destination,
				int64_t& width,
				int64_t& height,
				int64_t& format
			) -> void
			{
				return Kernel::Support::Texture::InvokeMethod::decode_fs(source, destination, static_cast<int>(width), static_cast<int>(height), static_cast<Kernel::Support::Texture::Format>(format));
			}

			inline auto encode_fs(
				std::string& source,
				std::string& destination,
				int64_t& format
			) -> void
			{
				return Kernel::Support::Texture::InvokeMethod::encode_fs(source, destination, static_cast<Kernel::Support::Texture::Format>(format));
			}

		}

		#pragma endregion

		#pragma region marmalade

		namespace Marmalade {

			#pragma region dzip

			namespace DZip {

				inline auto unpack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::Marmalade::DZip::Unpack::process_fs(source, destination);
				}

				inline auto pack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::Marmalade::DZip::Pack::process_fs(source, destination);
				}

			}

			#pragma endregion

		}

		#pragma endregion
	
		#pragma region popcap

		namespace PopCap {

			#pragma region package
			
			namespace Package {

				inline auto unpack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Package::Unpack::process_fs(source, destination);
				}

				inline auto pack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Package::Pack::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region new-type-object-notation

			namespace NewTypeObjectNotation {

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::NewTypeObjectNotation::Decode::process_fs(source, destination);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::NewTypeObjectNotation::Encode::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region reflection-object-notation

			namespace ReflectionObjectNotation {

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Decode::process_fs(source, destination);
				}

				inline auto decrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::decrypt_fs(source, destination, key, iv);
				}

				inline auto decrypt_and_decode_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::decrypt_and_decode_fs(source, destination, key, iv);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Encode::process_fs(source, destination);
				}

				inline auto encrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::encrypt_fs(source, destination, key, iv);
				}

				inline auto encode_and_encrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::encode_and_encrypt_fs(source, destination, key, iv);
				}

			}

			#pragma endregion

			#pragma region zlib

			namespace Zlib {

				inline auto uncompress_fs(
					std::string& source,
					std::string& destination,
					bool use_64_bit_variant
				) -> void
				{
					if (use_64_bit_variant) {
						Kernel::Support::PopCap::Zlib::Uncompress<true>::process_fs(source, destination);
					}
					else {
						Kernel::Support::PopCap::Zlib::Uncompress<false>::process_fs(source, destination);
					}
				}

				inline auto compress_fs(
					std::string& source,
					std::string& destination,
					bool use_64_bit_variant
				) -> void
				{
					if (use_64_bit_variant) {
						Kernel::Support::PopCap::Zlib::Compress<true>::process_fs(source, destination);
					}
					else {
						Kernel::Support::PopCap::Zlib::Compress<false>::process_fs(source, destination);
					}
				}

			}

			#pragma endregion

			#pragma region compiled_text

			namespace CompiledText {

				inline auto decode_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv,
					bool use_64_bit_variant
				) -> void
				{
					return Kernel::Support::PopCap::CompiledText::Decode::process_fs(source, destination, key, iv, use_64_bit_variant);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv,
					bool use_64_bit_variant
				) -> void
				{
					return Kernel::Support::PopCap::CompiledText::Encode::process_fs(source, destination, key, iv, use_64_bit_variant);
				}

			}

			#pragma endregion

			#pragma region render-effects

			namespace RenderEffects {

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::RenderEffects::Decode::process_fs(source, destination);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::RenderEffects::Encode::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region crypt-data

			namespace CryptData {

				inline auto decrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key
				) -> void
				{
					return Kernel::Support::PopCap::CryptData::Decrypt::process_fs(source, destination, key);
				}

				inline auto encrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key
				) -> void
				{
					return Kernel::Support::PopCap::CryptData::Encrypt::process_fs(source, destination, key);
				}

			}

			#pragma endregion

			#pragma region character-font-widget-2

			namespace CharacterFontWidget2
			{

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::CharacterFontWidget2::Decode::process_fs(source, destination);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::CharacterFontWidget2::Encode::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region particles

			namespace Particles
			{
				using Platform = Kernel::Support::PopCap::Particles::ParticlesPlatform;

				inline auto constexpr get_platform(
					std::string_view platform
				) -> Platform
				{
					switch (hash_string(platform)) {
						case hash_string("pc"_sv):
							return Platform::PC_Compile;
						case hash_string("game_console"_sv):
							return Platform::GameConsole_Compile;
						case hash_string("phone-32"_sv):
							return Platform::Phone32_Compile;
						case hash_string("phone-64"_sv):
							return Platform::Phone64_Compile;
						case hash_string("tv"_sv):
							return Platform::TV_Compile;
						case hash_string("wp"_sv):
							return Platform::WP_XNB;
						default:
							assert_conditional(false, fmt::format("{}", Kernel::Language::get("popcap.particles.decode.invalid_particles_platform")), "get_platform");
						}
				}

				inline auto decode_fs(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::Particles::Decode::process_fs(source, destination, get_platform(platform));
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::Particles::Encode::process_fs(source, destination, get_platform(platform));
				}

				inline auto to_xml(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Particles::ToXML::process_fs(source, destination);
				}

				inline auto from_xml(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Particles::FromXML::process_fs(source, destination);
				}

			}

			#pragma endregion 

			#pragma region player_info

			namespace PlayerInfo
			{

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::PlayerInfo::Decode::process_fs(source, destination);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::PlayerInfo::Encode::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region resource-stream-bundle

			namespace ResourceStreamBundle {

				inline auto unpack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundle::Unpack::process_fs(source, destination);
				}

				inline auto pack_fs (
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundle::Pack::process_fs(source, destination);
				}

				inline auto unpack_cipher(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous::UnpackCipher::process_fs(source, destination);
				}

				inline auto unpack_resource(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous::UnpackCipher::process_fs(source, destination);
				}

				inline auto pack_resource(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous::PackResource::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region resource-stream-bundle-patch

			namespace ResourceStreamBundlePatch
			{

				inline auto encode_fs(
					std::string& before_file,
					std::string& after_file,
					std::string& patch_file
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundlePatch::Encode::process_fs(before_file, after_file, patch_file);
				}

				inline auto decode_fs(
					std::string& before_file,
					std::string& patch_file,
					std::string& after_file
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamBundlePatch::Decode::process_fs(before_file, patch_file, after_file);
				}

			}

			#pragma endregion

			#pragma region resource-stream-group

			namespace ResourceStreamGroup
			{

				inline auto unpack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamGroup::Unpack::process_fs(source, destination);
				}

				inline auto pack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceStreamGroup::Pack::process_fs(source, destination);
				}
			}

			#pragma endregion

			#pragma region animation

			namespace Animation {

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Animation::Decode::process_fs(source, destination);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Animation::Encode::proces_fs(source, destination);
				}

				inline auto to_flash (
					std::string& source,
					std::string& destination,
					int64_t& resolution,
					bool use_label
				) -> void
				{
					if (use_label) {
						return Kernel::Support::PopCap::Animation::Convert::ConvertToFlashWithLabel::process_fs(source, destination, static_cast<int>(resolution));
					}
					else {
						return Kernel::Support::PopCap::Animation::Convert::ConvertToFlashWithMainSprite::process_fs(source, destination, static_cast<int>(resolution));
					}
				}

				inline auto from_flash (
					std::string& source,
					std::string& destination,
					bool use_label
				) -> void
				{
					if (use_label) {
						return Kernel::Support::PopCap::Animation::Convert::ConvertFromFlashWithLabel::process_fs(source, destination);
					}
					else {
						return Kernel::Support::PopCap::Animation::Convert::ConvertFromFlashWithMainSprite::process_fs(source, destination);
					}
				}

				inline auto decode_and_to_flash (
					std::string& source,
					std::string& destination,
					int64_t& resolution,
					bool use_label
				) -> void
				{
					if (use_label) {
						return Kernel::Support::PopCap::Animation::Convert::InstanceConvert::to_flash<true>(source, destination, static_cast<int>(resolution));
					}
					else {
						return Kernel::Support::PopCap::Animation::Convert::InstanceConvert::to_flash<false>(source, destination, static_cast<int>(resolution));
					}
				}

				inline auto from_flash_and_encode (
					std::string& source,
					std::string& destination,
					bool use_label
				) -> void
				{
					if (use_label) {
						return Kernel::Support::PopCap::Animation::Convert::InstanceConvert::from_flash<true>(source, destination);
					}
					else {
						return Kernel::Support::PopCap::Animation::Convert::InstanceConvert::from_flash<false>(source, destination);
					}
				}

				#pragma endregion

				#pragma region miscellaneous

				namespace Miscellaneous {

					inline auto resize_fs(
						std::string& source,
						int64_t& resolution
					) -> void
					{
						return Kernel::Support::PopCap::Animation::Convert::Resize::process_fs(source, static_cast<int>(resolution));
					}

					using VImage = Kernel::Support::PopCap::Animation::Miscellaneous::Image;

					using VSprite = Kernel::Support::PopCap::Animation::Miscellaneous::Sprite;

					using VDocument = Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument;

					inline auto dump_document(
						JSContext* context,
						std::string& source,
						JSValue& destination
					) -> void
					{
						auto doc = VDocument{};
						Kernel::Support::PopCap::Animation::Miscellaneous::Dump::process_fs(source, doc);
						auto value = JavaScript::Value::as_new_reference(context, destination);
						value.define_property("sprite", JavaScript::to_value<List<std::string>>(context, doc.sprite));
						value.define_property("image", JavaScript::to_value<List<std::string>>(context, doc.image));
						value.define_property("media", JavaScript::to_value<List<std::string>>(context, doc.media));
						value.define_property("action", JavaScript::to_value<List<std::string>>(context, doc.action));
					}

					inline auto generate_document(
						std::string& destination,
						std::shared_ptr<VDocument>& document
					) -> void
					{
						return Kernel::Support::PopCap::Animation::Miscellaneous::Generator::generate_document(destination, document.get());
					}

					inline auto generate_image(
						std::string& destination,
						std::shared_ptr<VImage>& source
					) -> void
					{
						return Kernel::Support::PopCap::Animation::Miscellaneous::Generator::generate_image(destination, source.get());
					}

					inline auto generate_sprite(
						std::string& destination,
						std::shared_ptr<VSprite>& source
					) -> void
					{
						return Kernel::Support::PopCap::Animation::Miscellaneous::Generator::generate_sprite(destination, source.get());
					}

				}

				#pragma endregion
			}

			#pragma endregion

			#pragma region re-animation

			namespace ReAnimation {

				using Platform = Kernel::Support::PopCap::ReAnimation::ReanimPlatform;

				inline auto constexpr exchange_platform(
					std::string_view platform
				) -> Platform
				{
					switch (hash_string(platform)) {
						case hash_string("pc"_sv):
							return Platform::PC_Compile;
						case hash_string("game-console"_sv):
							return Platform::GameConsole_Compile;
						case hash_string("phone-32"_sv):
							return Platform::Phone32_Compile;
						case hash_string("phone-64"_sv):
							return Platform::Phone64_Compile;
						case hash_string("tv"_sv):
							return Platform::TV_Compile;
						case hash_string("wp"_sv):
							return Platform::WP_XNB;
						default:
							assert_conditional(false, fmt::format("{}", Kernel::Language::get("popcap.reanim.decode.invalid_reanim_platform")), "exchange_platform");
					}
				}

				inline auto decode_fs(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::Decode::process_fs(source, destination, exchange_platform(platform));
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::Encode::process_fs(source, destination, exchange_platform(platform));
				}

				inline auto to_xml(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::ToXML::process_fs(source, destination);
				}

				inline auto from_xml(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::FromXML::process_fs(source, destination);
				}

				inline auto to_flash(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::Convert::ToFlash::process_fs(source, destination);
				}

				inline auto from_flash(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::Convert::FromFlash::process_fs(source, destination);
				}

				inline auto decode_and_to_flash(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::Convert::InstanceConvert::to_flash(source, destination, exchange_platform(platform));
				}


				inline auto from_flash_and_encode(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::ReAnimation::Convert::InstanceConvert::from_flash(source, destination, exchange_platform(platform));
				}

			}

			#pragma endregion

			#pragma region resource-group

			namespace ResourceGroup {

				using PathStyle = Kernel::Support::PopCap::ResourceGroup::PathStyle;

				inline auto split_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceGroup::Split::process_fs(source, destination);
				}

				inline auto merge_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResourceGroup::Merge::process_fs(source, destination);
				}

				inline auto convert_fs(
					std::string& source,
					std::string& destination,
					int64_t& path_style
				) -> void
				{
					switch (static_cast<PathStyle>(path_style)) {
						case PathStyle::ArrayStyle: {
							Kernel::Support::PopCap::ResourceGroup::Convert<false>::process_fs(source, destination);
							break;
						}
						case PathStyle::WindowStyle: {
							Kernel::Support::PopCap::ResourceGroup::Convert<true>::process_fs(source, destination);
							break;
						}
					}
				}
			}

			#pragma endregion

			#pragma region res-info

			namespace ResInfo
			{

				inline auto split_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResInfo::Split::process_fs(source, destination);
				}

				inline auto merge_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResInfo::Merge::process_fs(source, destination);
				}

				inline auto convert_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ResInfo::Convert::process_fs(source, destination);
				}

			}

			#pragma endregion
		}

		#pragma endregion

		#pragma region wwise

		namespace WWise {

			#pragma region soundbank

			namespace SoundBank {

				inline auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::WWise::SoundBank::Decode::process_fs(source, destination);
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::WWise::SoundBank::Encode::process_fs(source, destination);
				}

				inline auto hash(
					std::string& value
				) -> uint32_t
				{
					return Kernel::Support::WWise::SoundBank::Decode::fnv_hash(value);
				}
			}

			#pragma endregion

		}

		#pragma endregion

		#pragma region project

		namespace Project
		{

			#pragma region stream-compressed-group

			namespace StreamCompressedGroup
			{

				using Setting = Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Setting;

				inline auto check_scg_composite(
					std::string& source
				) -> bool
				{
					return Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Common::check_scg_composite(source);
				}

				inline auto decode_fs(
					std::string& source,
					std::string& destination,
					std::shared_ptr<Setting>& setting
				) -> void
				{
					return Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Decode::process_fs(source, destination, setting.operator*());
				}

				inline auto encode_fs(
					std::string& source,
					std::string& destination,
					std::shared_ptr<Setting>& setting
				) -> void
				{
					return Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Encode::process_fs(source, destination, setting.operator*());
				}
			}

			#pragma endregion

			#pragma region resource-stream-bundle

			namespace ResourceStreamBundle
			{

				using Setting = Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Setting;

				inline auto unpack_fs(
					std::string& source,
					std::string& destination,
					std::shared_ptr<Setting>& setting
				) -> void
				{
					Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Unpack::process_fs(source, destination, setting.operator*());
				}

				inline auto pack_fs(
					std::string& source,
					std::string& destination,
					std::shared_ptr<Setting>& setting
				) -> void
				{
					return Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Pack::process_fs(source, destination, setting.operator*());
				}
			}

			#pragma endregion

		}

		#pragma endregion
	}

	#pragma region miscellaneous

	namespace Miscellaneous {

		inline auto copyArrayBuffer(
			std::shared_ptr<JavaScript::ArrayBuffer>& data
		) -> std::shared_ptr<JavaScript::ArrayBuffer>
		{
			return std::make_shared<JavaScript::ArrayBuffer>(reinterpret_cast<uint8_t*>(data->value), data->size);
		}

		inline auto compareArrayBuffer(
			std::shared_ptr<JavaScript::ArrayBuffer>& source,
			std::shared_ptr<JavaScript::ArrayBuffer>& destination
		) -> bool
		{
			if (source->size != destination->size) {
				return false;
			}
			return std::memcmp(source->value, destination->value, source->size) == 0;
		}

		inline auto to_apng(
			List<std::string>& image_path_list,
			std::string& destination,
			std::shared_ptr<Kernel::Encoding::APNG::DefaultSetting>& setting
		) -> void
		{
			return Kernel::Encoding::APNG::Writer::process_fs(image_path_list, destination, setting.get());
		}

		inline auto make_copy(
			JSContext* context,
			JSValue& value
		) -> JSValue
		{
			return JavaScript::VariableDuplicator::deep_clone(context, value);
		}

	}
    */
	#pragma endregion
}