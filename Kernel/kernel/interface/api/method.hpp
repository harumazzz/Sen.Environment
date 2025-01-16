#pragma once
#include "kernel/interface/shell.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/utility/javascript/builder.hpp"

namespace Sen::Kernel::Interface::API {

	#pragma region proxy

	template <typename Callable> requires std::is_invocable<Callable>::value
	inline static auto proxy_wrapper(
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

		inline static auto is_gui (

		) -> bool {
			auto is_gui = std::unique_ptr<CStringView, StringFinalizer>(new CStringView(nullptr, 0), finalizer<CStringView>);
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
			construct_string_list(std::to_array<std::string>({std::string{"is_gui"}}), parameters.operator*());
			Interface::Shell::callback(parameters.get(), is_gui.get());
			return Converter::to_int32(std::string{reinterpret_cast<const char*>(is_gui->value), static_cast<std::size_t>(is_gui->size)}, "Cannot get is gui argument from Shell");
		}
		
		inline static auto version (

		) -> int {
			auto shell_version = std::unique_ptr<CStringView, StringFinalizer>(new CStringView(nullptr, 0), finalizer<CStringView>);
			auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
			construct_string_list(std::to_array<std::string>({std::string{"version"}}), parameters.operator*());
			Interface::Shell::callback(parameters.get(), shell_version.get());
			return static_cast<int>(Converter::to_int32(std::string{ reinterpret_cast<const char*>(shell_version->value), static_cast<std::size_t>(shell_version->size) }, "Cannot get the Shell version"));
		}

		inline static auto callback(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue {
			return proxy_wrapper(context, "callback"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), 1, Kernel::Language::get("kernel.argument_received"), argc), "callback");
				auto destination = std::unique_ptr<CStringView, StringFinalizer>{ new CStringView(nullptr, 0), finalizer<CStringView> };
				auto parameters = std::unique_ptr<CStringList, StringListFinalizer>{ new CStringList(nullptr, 0), finalizer<CStringList> };
				JavaScript::to_string_list<CStringList, CStringView>(context, argv[0], parameters.operator*());
				Interface::Shell::callback(parameters.get(), destination.get());
				auto result = JavaScript::to_string<CStringView>(context, destination.get());
				return result;
			});
		}
			
	}

	#pragma endregion

	#pragma region information

	inline static auto version(

	) -> int {
		return Kernel::version;
	}

	inline static auto arguments(
		JSContext* context,
		JSValue value,
		int argc,
		JSValue* argv
	) -> JSValue {
		return JavaScript::to_array_of_string<CStringList, CStringView>(context, Executor::arguments);
	}

	inline static auto script(

	) -> std::string {
		return String::to_posix_style(construct_string(Executor::script));
	}

	#pragma endregion

	#pragma region json

	namespace JSON {

		inline static auto deserialize(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue {
			return proxy_wrapper(context, "deserialize", [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize");
				auto source = JavaScript::Converter::get_string(context, argv[0]);
				auto json = nlohmann::ordered_json::parse(source);
				auto object = JavaScript::to(context, json);
				return object;
			});
		}

		inline static auto deserialize_fs(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue {
			return proxy_wrapper(context, "deserialize_fs", [&]() {
				assert_conditional(argc == 1, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), 1, Kernel::Language::get("kernel.argument_received"), argc), "deserialize_fs");
				auto source = JavaScript::Converter::get_string(context, argv[0]);
				auto json = Kernel::FileSystem::read_json(source);
				auto object = JavaScript::to(context, json.operator*());
				return object;
			});
		}

		inline static auto serialize(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue {
			return proxy_wrapper(context, "serialize", [&]() {
				assert_conditional(argc == 3, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), 3, Kernel::Language::get("kernel.argument_received"), argc), "serialize");
				auto value = JavaScript::Value::as_new_reference(context, argv[0]);
				auto json = JavaScript::from(value);
				auto indent = JavaScript::Converter::get_int32(context, argv[1]);
				auto ensure_ascii = JavaScript::Converter::get_bool(context, argv[2]);
				return json.dump(indent, '\t', ensure_ascii);
			});
		}

		inline static auto serialize_fs(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue {
			return proxy_wrapper(context, "serialize_fs", [&]() -> void {
				assert_conditional(argc == 4, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), 4, Kernel::Language::get("kernel.argument_received"), argc), "serialize_fs");
				auto destination = JavaScript::Converter::get_string(context, argv[0]);
				auto value = JavaScript::Value::as_new_reference(context, argv[1]);
				auto json = JavaScript::from(value);
				auto indent = JavaScript::Converter::get_int32(context, argv[2]);
				auto ensure_ascii = JavaScript::Converter::get_bool(context, argv[3]);
				auto result = json.dump(indent, '\t', ensure_ascii);
				Kernel::FileSystem::write_file(destination, result);
			});
		}

	}

	#pragma endregion

	#pragma region diff

	namespace Diff {

		#pragma region vcdiff

		namespace VCDiff {

			inline static auto encode_fs(
				std::string& before_file,
				std::string& after_file,
				std::string& patch_file,
				int32_t& flag
			) -> void {
				return Kernel::Diff::VCDiff::encode_fs(
					before_file,
					after_file,
					patch_file,
					static_cast<Kernel::Diff::VCDiff::Flag>(flag)
				);
			}

			inline static auto decode_fs(
				std::string& before_file,
				std::string& patch_file,
				std::string& after_file
			) -> void {
				return Kernel::Diff::VCDiff::decode_fs(before_file, patch_file, after_file);
			}

		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region filesystem

	namespace FileSystem {

		inline static auto read_file (
			std::string& source
		) -> std::string {
			return Kernel::FileSystem::read_file(source);
		}

		inline static auto read_file_encode_with_utf16le (
			std::string& source
		) -> std::string
		{
			auto result = Kernel::FileSystem::read_file_by_utf16(source);
			auto converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
			return std::string{ converter.to_bytes(result) };
		}

		inline static auto write_file (
			std::string& destination,
			std::string& data
		) -> void {
			return Kernel::FileSystem::write_file(destination, data);
		}

		inline static auto write_file_encode_with_utf16le (
			std::string& destination,
			std::string& data
		) -> void {
			auto converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{};
			auto result = std::wstring{ converter.from_bytes(data) };
			return Kernel::FileSystem::write_file_by_utf16le(destination, result);
		}

		inline static auto read_current_directory (
			std::string& source
		) -> List<std::string> {
			return Kernel::FileSystem::read_directory(source);
		}

		inline static auto read_directory_only_file(
			std::string& source
		) -> List<std::string> {
			return Kernel::FileSystem::read_directory_only_file(source);
		}

		inline static auto read_directory_only_directory(
			std::string& source
		) -> List<std::string> {
			return Kernel::FileSystem::read_directory_only_directory(source);
		}

		inline static auto read_directory (
			std::string& source
		) -> List<std::string> {
			return Kernel::FileSystem::read_whole_directory(source);
		}

		inline static auto create_directory(
			std::string& destination
		) -> void
		{
			return Kernel::FileSystem::create_directory(destination);
		}

		inline static auto is_file(
			std::string& source
		) -> bool
		{
			return Kernel::Path::Script::is_file(source);
		}

		inline static auto is_directory(
			std::string& source
		) -> bool
		{
			return Kernel::Path::Script::is_directory(source);
		}

	}
	#pragma endregion

	#pragma region path

	namespace Path {

	}

	#pragma endregion

	#pragma region console

	namespace Console {

		using Color = Kernel::Interface::Color;

		inline static auto exchange_color(
			Color color
		) -> std::string
		{
			switch (color) {
			case Color::RED:
				return "red";
			case Color::GREEN:
				return "green";
			case Color::CYAN:
				return "cyan";
			case Color::YELLOW:
				return "yellow";
			default:
				return "default";
			}
		}

		inline static auto print(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "print", [&]() {
				assert_conditional(argc >= 1, fmt::format("argument expected greater than {} but received {}", "1", argc), "print");
				auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
				switch (argc)
				{
					case 1:
					{
						construct_string_list(std::to_array<std::string>({ std::string{ "display" }, JavaScript::Converter::get_string(context, argv[0]) }), parameters.operator*());
						Interface::Shell::callback(parameters.get(), nullptr);
						break;
					}
					case 2:
					{
						construct_string_list(std::to_array<std::string>({ std::string{ "display" }, JavaScript::Converter::get_string(context, argv[0]), JavaScript::Converter::get_string(context, argv[1]) }), parameters.operator*());
						Interface::Shell::callback(parameters.get(), nullptr);
						break;
					}
					default:
					{
						construct_string_list(std::to_array<std::string>({ std::string{ "display" }, JavaScript::Converter::get_string(context, argv[0]), JavaScript::Converter::get_string(context, argv[1]), exchange_color(static_cast<Color>(JavaScript::Converter::get_int32(context, argv[2]))) }), parameters.operator*());
						Interface::Shell::callback(parameters.get(), nullptr);
						break;
					}
				}
			});
		}

		inline static auto readline(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "readline", [&]() {
				auto wait_parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
				construct_string_list(std::to_array<std::string>({ std::string{ "wait" } }), wait_parameters.operator*());
				Interface::Shell::callback(wait_parameters.get(), nullptr);
				auto destination = std::unique_ptr<CStringView, StringFinalizer>(new CStringView(nullptr, 0), finalizer<CStringView>);
				auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
				construct_string_list(std::array<std::string, 1>{std::string{ "input" }}, parameters.operator*());
				Interface::Shell::callback(parameters.get(), destination.get());
				return JavaScript::to_string<CStringView>(context, destination.get());
			});
		}

	}
	#pragma endregion

	#pragma region language
	
	namespace Language {

		inline static auto load_language (
			std::string& source
		) -> void {
			return Kernel::Language::read_language(source);
		}

		inline static auto get (
			std::string& key
		) -> std::string {
			return Kernel::Language::get(key);
		}

	}
	#pragma endregion

	#pragma region process

	namespace Process {

		inline static auto run(
			std::string& command
		) -> void
		{
			return Kernel::Process::run(command);
		}

		inline static auto execute(
			std::string& command
		) -> std::string
		{
			return Kernel::Process::execute(command);
		}

		inline static auto is_exists_in_path_environment(
			std::string& source
		) -> bool
		{
			return Kernel::Process::is_exists_in_path_environment(source);
		}

		inline static auto get_path_environment(
			std::string& value
		) -> std::string
		{
			return Kernel::Process::get_path_environment(value);
		}
	}

	#pragma endregion

	#pragma region thread

	namespace Thread {

		inline static auto sleep(
			int64_t& time
		) -> void
		{
			return std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(time)));
		}

		inline static auto now(
		) -> double
		{
			return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

	}
	#pragma endregion

	#pragma region arraybuffer

	namespace ArrayBuffer {

		inline static auto random(
			std::shared_ptr<JavaScript::ArrayBuffer>& data
		) -> void
		{
			auto rd = std::random_device{};
			auto gen = std::mt19937{ rd() };
			auto dist = std::uniform_int_distribution<unsigned int>(0, 255);
			for (auto i : Range<std::size_t>(data->size)) {
				data->value[i] = static_cast<uint8_t>(dist(gen));
			}
		}

	}

	#pragma endregion

	#pragma region operating system

	namespace OperatingSystem {

		inline static auto current(
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

		inline static auto evaluate_fs(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "evaluate", [&]() {
				auto source = JavaScript::Converter::get_string(context, argv[0]);
				auto js_source = Kernel::FileSystem::read_file(source);
				return JS_Eval(context, js_source.data(), js_source.size(), source.data(), JS_EVAL_FLAG_STRICT | JS_EVAL_TYPE_GLOBAL);
			});
		}

	}

	#pragma endregion

	#pragma region compression

	namespace Compression {

		#pragma region zip

		namespace Zip {

			#pragma region uncompress

			namespace Uncompress {

				inline static auto process(
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


		}

		#pragma endregion

	}

	#pragma endregion

	#pragma region support

	namespace Support {

		#pragma region texture

		namespace Texture {

			inline static auto decode_fs(
				std::string& source,
				std::string& destination,
				int64_t& width,
				int64_t& height,
				int64_t& format
			) -> void
			{
				return Kernel::Support::Texture::InvokeMethod::decode_fs(source, destination, static_cast<int>(width), static_cast<int>(height), static_cast<Kernel::Support::Texture::Format>(format));
			}

			inline static auto encode_fs(
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

				inline static auto unpack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::Marmalade::DZip::Unpack::process_fs(source, destination);
				}

				inline static auto pack_fs(
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

			#pragma region pak
			
			namespace PAK {

				inline static auto unpack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Package::Unpack::process_fs(source, destination);
				}

				inline static auto pack_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Package::Pack::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region newton

			namespace Newton {

				inline static auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::NewTypeObjectNotation::Decode::process_fs(source, destination);
				}

				inline static auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::NewTypeObjectNotation::Encode::process_fs(source, destination);
				}

			}

			#pragma endregion

			#pragma region rton

			namespace RTON {

				inline static auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Decode::process_fs(source, destination);
				}

				inline static auto decrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::decrypt_fs(source, destination, key, iv);
				}

				inline static auto decrypt_and_decode_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::decrypt_and_decode_fs(source, destination, key, iv);
				}

				inline static auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Encode::process_fs(source, destination);
				}

				inline static auto encrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv
				) -> void
				{
					return Kernel::Support::PopCap::ReflectionObjectNotation::Chinese::encrypt_fs(source, destination, key, iv);
				}

				inline static auto encode_and_encrypt_fs(
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

				inline static auto uncompress_fs(
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

				inline static auto compress_fs(
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

				inline static auto decode_fs(
					std::string& source,
					std::string& destination,
					std::string& key,
					std::string& iv,
					bool use_64_bit_variant
				) -> void
				{
					return Kernel::Support::PopCap::CompiledText::Decode::process_fs(source, destination, key, iv, use_64_bit_variant);
				}

				inline static auto encode_fs(
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

				inline static auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::RenderEffects::Decode::process_fs(source, destination);
				}

				inline static auto encode_fs(
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

				inline static auto decrypt_fs(
					std::string& source,
					std::string& destination,
					std::string& key
				) -> void
				{
					return Kernel::Support::PopCap::CryptData::Decrypt::process_fs(source, destination, key);
				}

				inline static auto encrypt_fs(
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

				inline static auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::CharacterFontWidget2::Decode::process_fs(source, destination);
				}

				inline static auto encode_fs(
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

				inline static auto constexpr get_platform(
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

				inline static auto decode_fs(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::Particles::Decode::process_fs(source, destination, get_platform(platform));
				}

				inline static auto encode_fs(
					std::string& source,
					std::string& destination,
					std::string& platform
				) -> void
				{
					return Kernel::Support::PopCap::Particles::Encode::process_fs(source, destination, get_platform(platform));
				}

				inline static auto to_xml(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::Particles::ToXML::process_fs(source, destination);
				}

				inline static auto from_xml(
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

				inline static auto decode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::PlayerInfo::Decode::process_fs(source, destination);
				}

				inline static auto encode_fs(
					std::string& source,
					std::string& destination
				) -> void
				{
					return Kernel::Support::PopCap::PlayerInfo::Encode::process_fs(source, destination);
				}

			}

			#pragma endregion
		}

		#pragma endregion
	}

	#pragma endregion
}