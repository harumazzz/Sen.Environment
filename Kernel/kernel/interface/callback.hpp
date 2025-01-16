#pragma once

#include "kernel/interface/script.hpp"
#include "kernel/interface/api/method.hpp"
#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/interface/shell.hpp"

namespace Sen::Kernel::Interface {

	/**
	 * Use JS
	*/

	namespace JS = Sen::Kernel::JavaScript;
	
	template <typename ReturnType, typename... Args>
	using Proxy = JS::Proxy<ReturnType, Args...>;

	// callback

	class Runtime {

		private:

			JS::Handler runtime;

			using None = void;

		public:

			explicit Runtime(
			) : runtime{}
			{

			}
			
			/**
			 * Destructor
			*/

			~Runtime(

			) = default;

			/**
			 * Prepare the script
			*/
			inline auto prepare(
			) -> void
			{
				// shell callback
				{
					// is_gui
					{
						runtime.add_proxy(JS::Proxy<bool>::as_function<Kernel::Interface::API::Shell::is_gui>, std::to_array<std::string_view>({"Sen"_sv, "Shell"_sv}), "is_gui"_sv);
					}
					// version
					{
						runtime.add_proxy(JS::Proxy<int>::as_function<Kernel::Interface::API::Shell::version>, std::to_array<std::string_view>({"Sen"_sv, "Shell"_sv}), "version"_sv);
					}
					// version
					runtime.add_proxy(Proxy<int>::as_function<Kernel::Interface::API::version>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "version"_sv);
					// callback
					runtime.add_proxy(Interface::API::Shell::callback, std::to_array<std::string_view>({"Sen"_sv, "Shell"_sv}), "callback"_sv);
					// arguments
					runtime.add_proxy(Interface::API::arguments, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "arguments"_sv);
				}
				// xml
				// TODO : Later
				{
					// deserialize
					runtime.add_proxy(Script::XML::deserialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "deserialize"_sv);
					// deserialize_fs
					runtime.add_proxy(Script::XML::deserialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "deserialize_fs"_sv);
					// serialize
					runtime.add_proxy(Script::XML::serialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "serialize"_sv);
					// serialize_fs
					runtime.add_proxy(Script::XML::serialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "serialize_fs"_sv);
				}
				// json
				{
					// deserialize
					runtime.add_proxy(Interface::API::JSON::deserialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "deserialize"_sv);
					// serialize
					runtime.add_proxy(Interface::API::JSON::serialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "serialize"_sv);
					// deserialize fs
					runtime.add_proxy(Interface::API::JSON::deserialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "deserialize_fs"_sv);
					// serialize fs
					runtime.add_proxy(Interface::API::JSON::serialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "serialize_fs"_sv);
				}
				// home
				{
					// script
					runtime.add_proxy(Proxy<std::string>::as_function<Interface::API::script>, std::to_array<std::string_view>({ "Sen"_sv, "Kernel"_sv, "Home"_sv }), "script"_sv);
				}
				// vcdiff
				{
					// encode fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, int32_t&>::as_function<Interface::API::Diff::VCDiff::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Diff"_sv, "VCDiff"_sv}), "encode_fs"_sv);
					// decode fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&>::as_function<Interface::API::Diff::VCDiff::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Diff"_sv, "VCDiff"_sv}), "decode_fs"_sv);
				}
				// filesystem
				{
					// read_file
					runtime.add_proxy(Proxy<std::string, std::string&>::as_function<Interface::API::FileSystem::read_file>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_file"_sv);
					// read_file_encode_with_utf16le
					runtime.add_proxy(Proxy<std::string,std::string&>::as_function<Interface::API::FileSystem::read_file_encode_with_utf16le>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_file_encode_with_utf16le"_sv);
					// write_file
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::FileSystem::write_file>, std::to_array<std::string_view>({ "Sen"_sv, "Kernel"_sv, "FileSystem"_sv }), "write_file"_sv);
					// write_file_encode_with_utf16le
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::FileSystem::write_file_encode_with_utf16le>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "write_file_encode_with_utf16le"_sv);
					// read_current_directory
					runtime.add_proxy(Proxy<List<std::string>, std::string&>::as_function<Interface::API::FileSystem::read_current_directory>, std::to_array<std::string_view>({ "Sen"_sv, "Kernel"_sv, "FileSystem"_sv }), "read_current_directory"_sv);
					// read_directory_only_file
					runtime.add_proxy(Proxy<List<std::string>, std::string&>::as_function<Interface::API::FileSystem::read_directory_only_file>, std::to_array<std::string_view>({ "Sen"_sv, "Kernel"_sv, "FileSystem"_sv }), "read_directory_only_file"_sv);
					// read_directory_only_directory
					runtime.add_proxy(Proxy<List<std::string>, std::string&>::as_function<Interface::API::FileSystem::read_directory_only_directory>, std::to_array<std::string_view>({ "Sen"_sv, "Kernel"_sv, "FileSystem"_sv }), "read_directory_only_directory"_sv);
					// read_directory
					runtime.add_proxy(Proxy<List<std::string>, std::string&>::as_function<Interface::API::FileSystem::read_directory>, std::to_array<std::string_view>({ "Sen"_sv, "Kernel"_sv, "FileSystem"_sv }), "read_directory"_sv);
					// create_directory
					runtime.add_proxy(Proxy<void, std::string&>::as_function<Interface::API::FileSystem::create_directory>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "create_directory"_sv);
					// is_file
					runtime.add_proxy(Proxy<bool, std::string&>::as_function<Interface::API::FileSystem::is_file>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "is_file"_sv);
					// is_directory
					runtime.add_proxy(Proxy<bool, std::string&>::as_function<Interface::API::FileSystem::is_directory>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}),"is_directory"_sv);
					// TODO : Must do later
					// operation : rename
					runtime.add_proxy(Script::FileSystem::Operation::rename, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "rename"_sv);
					// operation : remove
					runtime.add_proxy(Script::FileSystem::Operation::remove, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "remove"_sv);
					// operation : remove all
					runtime.add_proxy(Script::FileSystem::Operation::remove_all, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "remove_all"_sv);
					// operation : copy
					runtime.add_proxy(Script::FileSystem::Operation::copy, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "copy"_sv);
					// operation : copy_directory
					runtime.add_proxy(Script::FileSystem::Operation::copy_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "copy_directory"_sv);
				}
				// path
				// TODO : Remove Script : Finish this later
				{
					// join
					runtime.add_proxy(Script::Path::join, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "join"_sv);
					// basename
					runtime.add_proxy(Script::Path::basename, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "basename"_sv);
					// delimiter
					runtime.add_proxy(Script::Path::delimiter, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "delimiter"_sv);
					// dirname
					runtime.add_proxy(Script::Path::dirname, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "dirname"_sv);
					// format
					runtime.add_proxy(Script::Path::format, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "format"_sv);
					// normalize
					runtime.add_proxy(Script::Path::normalize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "normalize"_sv);
					// resolve
					runtime.add_proxy(Script::Path::resolve, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "resolve"_sv);
					// relative
					runtime.add_proxy(Script::Path::relative, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "relative"_sv);
					// extname
					runtime.add_proxy(Script::Path::extname, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "extname"_sv);
					// is_absolute
					runtime.add_proxy(Script::Path::relative, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "is_absolute"_sv);
					// base_without_extension
					runtime.add_proxy(Script::Path::base_without_extension, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "base_without_extension"_sv);
					// except_extension
					runtime.add_proxy(Script::Path::except_extension, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "except_extension"_sv);
				}
				// console
				{
					// print
					runtime.add_proxy(Interface::API::Console::print, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Console"_sv}), "print"_sv);
					// readline
					runtime.add_proxy(Interface::API::Console::readline, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Console"_sv}), "readline"_sv);
				}
				// language
				{
					// load_language
					runtime.add_proxy(Proxy<void, std::string&>::as_function<Interface::API::Language::load_language>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Language"_sv}), "load_language"_sv);
					// get
					runtime.add_proxy(Proxy<std::string, std::string&>::as_function<Interface::API::Language::get>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Language"_sv}), "get"_sv);
				}
				// process
				{
					// run
					runtime.add_proxy(Proxy<void, std::string&>::as_function<Interface::API::Process::run>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "run"_sv);
					// execute
					runtime.add_proxy(Proxy<std::string, std::string&>::as_function<Interface::API::Process::execute>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "execute"_sv);
					// is_exists_in_path_environment
					runtime.add_proxy(Proxy<bool, std::string&>::as_function<Interface::API::Process::is_exists_in_path_environment>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "is_exists_in_path_environment"_sv);
					// get_path_environment
					runtime.add_proxy(Proxy<std::string, std::string&>::as_function<Interface::API::Process::get_path_environment>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "get_path_environment"_sv);
				}
				// thread
				{
					// sleep
					runtime.add_proxy(Proxy<void, int64_t&>::as_function<Interface::API::Thread::sleep>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Thread"_sv}), "sleep"_sv);
					// now
					runtime.add_proxy(Proxy<double>::as_function<Interface::API::Thread::now>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Thread"_sv}), "now"_sv);
				}
				// array buffer
				{
					// random
					runtime.add_proxy(Proxy<void, std::shared_ptr<JS::ArrayBuffer>&>::as_function<Interface::API::ArrayBuffer::random>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "ArrayBuffer"_sv}), "random"_sv);
				}
				// operating system
				{
					// current
					runtime.add_proxy(Proxy<std::string>::as_function<Interface::API::OperatingSystem::current>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "OperatingSystem"_sv}), "current"_sv);
				}
				// image
				// TODO : Rework : Finish this later
				{
					// scale_fs
					runtime.add_proxy(Script::Image::scale_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "scale_fs"_sv);
					// join_png
					runtime.add_proxy(Script::Image::join_png, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "join_png"_sv);
					// join
					runtime.add_proxy(Script::Image::join, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "join"_sv);
					// join_extend
					runtime.add_proxy(Script::Image::join_extend, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "join_extend"_sv);
					// transparent_fs
					runtime.add_proxy(Script::Image::transparent_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "transparent_fs"_sv);
					// resize_fs
					runtime.add_proxy(Script::Image::resize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "resize_fs"_sv);
					// rotate_fs
					runtime.add_proxy(Script::Image::rotate_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "rotate_fs"_sv);
					// cut_fs
					runtime.add_proxy(Script::Image::cut_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "cut_fs"_sv);
					// cut_multiple_fs
					runtime.add_proxy(Script::Image::cut_multiple_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "cut_multiple_fs"_sv);
					// cut_multiple_fs_asynchronous
					runtime.add_proxy(Script::Image::cut_multiple_fs_asynchronous, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "cut_multiple_fs_asynchronous"_sv);
				}
				// runtime
				{
					// evaluate_fs
					runtime.add_proxy(Interface::API::JS::evaluate_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JavaScript"_sv}), "evaluate_fs"_sv);
				}
				// zip
				{
					// uncompress
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Compression::Zip::Uncompress::process>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zip"_sv, "Uncompress"_sv}), "process"_sv);
				}
				// zlib
				{
					// TODO : Rework this
					// uncompress
					runtime.add_proxy(Script::Compression::Zlib::uncompress, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zlib"_sv}), "uncompress"_sv);
				}
				// texture
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, int64_t&, int64_t&, int64_t&>::as_function<Interface::API::Support::Texture::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Texture"_sv}),"decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, int64_t&>::as_function<Interface::API::Support::Texture::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Texture"_sv}), "encode_fs"_sv);
				}
				// dzip
				{
					// unpack_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::Marmalade::DZip::unpack_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Marmalade"_sv, "DZip"_sv}), "unpack_fs"_sv);
					// pack_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::Marmalade::DZip::pack_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Marmalade"_sv, "DZip"_sv}), "pack_fs"_sv);
				}
				// pak
				{
					// unpack_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::PAK::unpack_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PAK"_sv}), "unpack_fs"_sv);
					// pack_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::PAK::pack_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PAK"_sv}), "pack_fs"_sv);
				}
				// newton
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::Newton::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Newton"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::Newton::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Newton"_sv}), "encode_fs"_sv);
				}
				// rton
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RTON::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "decode_fs"_sv);
					// decrypt_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RTON::decrypt_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "decrypt_fs"_sv);
					// decrypt_and_decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RTON::decrypt_and_decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "decrypt_and_decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RTON::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "encode_fs"_sv);
					// encrypt_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RTON::encrypt_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "encrypt_fs"_sv);
					// encode_and_encrypt_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RTON::encode_and_encrypt_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "encode_and_encrypt_fs"_sv);
				}
				// zlib
				{
					// uncompress_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, bool>::as_function<Interface::API::Support::PopCap::Zlib::uncompress_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Zlib"_sv}), "uncompress_fs"_sv);
					// compress_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, bool>::as_function<Interface::API::Support::PopCap::Zlib::compress_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Zlib"_sv}), "compress_fs"_sv);
				}
				// compiled_text
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, std::string&, bool>::as_function<Interface::API::Support::PopCap::CompiledText::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CompiledText"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&, std::string&, bool>::as_function<Interface::API::Support::PopCap::CompiledText::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CompiledText"_sv}), "encode_fs"_sv);
				}
				// render-effects
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RenderEffects::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RenderEffects"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::RenderEffects::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RenderEffects"_sv}), "encode_fs"_sv);
				}
				// crypt-data
				{
					// decrypt_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::CryptData::decrypt_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CryptData"_sv}), "decrypt_fs"_sv);
					// encrypt_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::CryptData::encrypt_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CryptData"_sv}), "encrypt_fs"_sv);
				}
				// character-font-widget-2
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::CharacterFontWidget2::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CharacterFontWidget2"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::CharacterFontWidget2::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CharacterFontWidget2"_sv}), "encode_fs"_sv);
				}
				// particles
				{
					// decode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::Particles::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::Particles::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "encode_fs"_sv);
					// to_xml
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::Particles::to_xml>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "to_xml"_sv);
					// from_xml
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::Particles::from_xml>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "from_xml"_sv);
				}
				// player_info
				{
					// decode fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::PlayerInfo::decode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PlayerInfo"_sv}), "decode_fs"_sv);
					// encode fs
					runtime.add_proxy(Proxy<void, std::string&, std::string&>::as_function<Interface::API::Support::PopCap::PlayerInfo::encode_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PlayerInfo"_sv}), "encode_fs"_sv);
				}
				// rsb
				{
					// unpack_fs
					runtime.add_proxy(Script::Support::PopCap::RSB::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "unpack_fs"_sv);
					// pack_fs
					runtime.add_proxy(Script::Support::PopCap::RSB::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "pack_fs"_sv);
					// unpack_cipher
					runtime.add_proxy(Script::Support::PopCap::RSB::unpack_cipher, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "unpack_cipher"_sv);
					// unpack_resource
					runtime.add_proxy(Script::Support::PopCap::RSB::unpack_resource, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "unpack_resource"_sv);
					// pack_resource
					runtime.add_proxy(Script::Support::PopCap::RSB::pack_resource, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "pack_resource"_sv);
				}
				// rsb-patch
				{
					// decode_fs
					runtime.add_proxy(Script::Support::PopCap::RSBPatch::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSBPatch"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Script::Support::PopCap::RSBPatch::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSBPatch"_sv}), "encode_fs"_sv);
				}
				// rsg
				{
					// unpack_fs
					runtime.add_proxy(Script::Support::PopCap::RSG::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSG"_sv}), "unpack_fs"_sv);
					// pack_fs
					runtime.add_proxy(Script::Support::PopCap::RSG::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSG"_sv}), "pack_fs"_sv);
				}
				// pam
				{
					// decode_fs
					runtime.add_proxy(Script::Support::PopCap::Animation::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Script::Support::PopCap::Animation::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv}), "encode_fs"_sv);
					// convert_fs
					runtime.add_proxy(Script::Support::PopCap::Animation::ToFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "ToFlash"_sv}), "convert_fs"_sv);
					// convert_fs
					runtime.add_proxy(Script::Support::PopCap::Animation::FromFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "FromFlash"_sv}), "convert_fs"_sv);
					// to_flash
					runtime.add_proxy(Script::Support::PopCap::Animation::Instance::to_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Instance"_sv}), "to_flash"_sv);
					// from_flash
					runtime.add_proxy(Script::Support::PopCap::Animation::Instance::from_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Instance"_sv}), "from_flash"_sv);
					// resize_fs
					runtime.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::resize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"}), "resize_fs"_sv);
					// dump_document
					runtime.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::dump_document, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "dump_document"_sv);
					// generate_document
					runtime.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::generate_document, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "generate_document"_sv);
					// generate_image
					runtime.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::generate_image, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "generate_image"_sv);
					// generate_sprite
					runtime.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::generate_sprite, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "generate_sprite"_sv);
				}
				// reanim
				{
					// decode_fs
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "encode_fs"_sv);
					// to_xml
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::to_xml, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "to_xml"_sv);
					// from_xml
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::from_xml, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "from_xml"_sv);
					// convert_fs
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::ToFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "ToFlash"_sv}), "convert_fs"_sv);
					// convert_fs
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::FromFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "FromFlash"_sv}), "convert_fs"_sv);
					// to_flash
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::Instance::to_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "Instance"_sv}), "to_flash"_sv);
					// from_flash
					runtime.add_proxy(Script::Support::PopCap::ReAnimation::Instance::from_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "Instance"_sv}), "from_flash"_sv);
				}
				// bnk
				{
					// decode_fs
					runtime.add_proxy(Script::Support::WWise::SoundBank::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv}), "decode_fs"_sv);
					// encode_fs
					runtime.add_proxy(Script::Support::WWise::SoundBank::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv}), "encode_fs"_sv);
					// hash
					runtime.add_proxy(Script::Support::WWise::SoundBank::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv}), "hash"_sv);
				}
				// resource group
				{
					// split_fs
					runtime.add_proxy(Script::Support::PopCap::ResourceGroup::split_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResourceGroup"_sv}), "split_fs"_sv);
					// merge_fs
					runtime.add_proxy(Script::Support::PopCap::ResourceGroup::merge_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResourceGroup"_sv}), "merge_fs"_sv);
					// convert_fs
					runtime.add_proxy(Script::Support::PopCap::ResourceGroup::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResourceGroup"_sv}), "convert_fs"_sv);
				}
				// res info
				{
					// split_fs
					runtime.add_proxy(Script::Support::PopCap::ResInfo::split_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResInfo"_sv}), "split_fs"_sv);
					// merge_fs
					runtime.add_proxy(Script::Support::PopCap::ResInfo::merge_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResInfo"_sv}), "merge_fs"_sv);
					// convert_fs
					runtime.add_proxy(Script::Support::PopCap::ResInfo::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResInfo"_sv}), "convert_fs"_sv);
				}
				// custom
				{
					// to_resource_custom
					runtime.add_proxy(Script::Support::PopCap::Custom::Debug::to_resource_custom, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "Debug"_sv}), "to_resource_custom"_sv);
					// from_resource_custom
					runtime.add_proxy(Script::Support::PopCap::Custom::Debug::from_resource_custom, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "Debug"_sv}), "from_resource_custom"_sv);
					// check_scg_composite
					runtime.add_proxy(Script::Support::PopCap::Custom::StreamCompressedGroup::check_scg_composite, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "StreamCompressedGroup"_sv}), "check_scg_composite"_sv);
					// unpack_scg
					runtime.add_proxy(Script::Support::PopCap::Custom::StreamCompressedGroup::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "StreamCompressedGroup"_sv}), "decode_fs"_sv);
					// pack_scg
					runtime.add_proxy(Script::Support::PopCap::Custom::StreamCompressedGroup::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "StreamCompressedGroup"_sv}), "encode_fs"_sv);
					// unpack_rsb
					runtime.add_proxy(JS::Proxy<void, std::string&, std::string&, std::shared_ptr<Kernel::Support::Miscellaneous::Custom::ResourceStreamBundle::Setting>&>::as_function<Script::Support::PopCap::Custom::ResourceStreamBundle::unpack_fs>, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "ResourceStreamBundle"_sv}), "unpack_fs"_sv);
					// pack_rsb
					runtime.add_proxy(Script::Support::PopCap::Custom::ResourceStreamBundle::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "ResourceStreamBundle"_sv}), "pack_fs"_sv);
				}
				// dimension
				{
					// instance of dimension
					runtime.add_proxy(Script::Dimension::instance, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Dimension"_sv}), "instance"_sv);
					// open the image
					runtime.add_proxy(Script::Dimension::open, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "open"_sv);
					// write the image
					runtime.add_proxy(Script::Dimension::write, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "write"_sv);
				}
				// miscellaneous
				{
					// make_copy
					runtime.add_proxy(Script::Miscellaneous::make_copy, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "make_copy"_sv);
					// cast_ArrayBuffer_to_JS_String
					runtime.add_proxy(Script::Miscellaneous::cast_ArrayBuffer_to_JS_String, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "cast_ArrayBuffer_to_JS_String"_sv);
					// cast_ArrayBuffer_to_JS_WideString
					runtime.add_proxy(Script::Miscellaneous::cast_ArrayBuffer_to_JS_WideString, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "cast_ArrayBuffer_to_JS_WideString"_sv);
					// cast_movable_String_to_ArrayBuffer
					runtime.add_proxy(Script::Miscellaneous::cast_movable_String_to_ArrayBuffer, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "cast_movable_String_to_ArrayBuffer"_sv);
					// copyArrayBuffer
					runtime.add_proxy(Script::Miscellaneous::copyArrayBuffer, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "copyArrayBuffer"_sv);
					// compareArrayBuffer
					runtime.add_proxy(Script::Miscellaneous::compareArrayBuffer, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "compareArrayBuffer"_sv);
					// to_apng
					runtime.add_proxy(Script::Miscellaneous::to_apng, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "to_apng"_sv);
				}
				// DataStreamView
				runtime.register_object(Script::Class::DataStreamView::register_class<false>);
				// DataStreamViewUseBigEndian
				runtime.register_object(Script::Class::DataStreamView::register_class<true>);
				// Boolean
				runtime.register_object(Script::Class::Boolean::register_class);
				// // Size
				runtime.register_object(Script::Class::Size::register_class);
				// // String
				runtime.register_object(Script::Class::String::register_class);
				// Character
				runtime.register_object(Script::Class::Character::register_class<char>);
				// UCharacter
				runtime.register_object(Script::Class::Character::register_class<unsigned char>);
				// WideCharacter
				runtime.register_object(Script::Class::Character::register_class<wchar_t>);
				// DimensionView
				runtime.register_object(Script::Class::DimensionView::register_class);
				// Rectangle
				runtime.register_object(Script::Class::Rectangle::register_class);
				// ImageView
				runtime.register_object(Script::Class::ImageView::register_class);
				// Canvas
				runtime.register_object(Script::Class::Canvas::register_class);
				// BinaryView
				runtime.register_object(Script::Class::BinaryView::register_class);
				// Integer8
				runtime.register_object(Script::Class::Number::register_class<int8_t>);
				// Integer16
				runtime.register_object(Script::Class::Number::register_class<int16_t>);
				// Integer32
				runtime.register_object(Script::Class::Number::register_class<int32_t>);
				// Integer64
				runtime.register_object(Script::Class::Number::register_class<int64_t>);
				// UInteger8
				runtime.register_object(Script::Class::Number::register_class<uint8_t>);
				// UInteger16
				runtime.register_object(Script::Class::Number::register_class<uint16_t>);
				// UInteger32
				runtime.register_object(Script::Class::Number::register_class<uint32_t>);
				// UInteger64
				runtime.register_object(Script::Class::Number::register_class<uint64_t>);
				// Float
				runtime.register_object(Script::Class::Number::register_class<float>);
				// Double
				runtime.register_object(Script::Class::Number::register_class<double>);
				// JsonWriter
				runtime.register_object(Script::Class::JsonWriter::register_class);
				// APNGMakerSetting
				runtime.register_object(Script::Class::APNGMakerSetting::register_class);
				// FileInputStream
				runtime.register_object(Script::Class::FileInputStream::register_class);
				// FileOutputStream
				runtime.register_object(Script::Class::FileOutputStream::register_class);
				// FileStream
				runtime.register_object(Script::Class::FileStream::register_class);
				// Image
				runtime.register_object(Script::Class::Image::register_class);
				// Sprite
				runtime.register_object(Script::Class::Sprite::register_class);
				// Clock
				runtime.register_object(Script::Clock::register_class);
				// execute the script
				runtime.evaluate_fs(construct_string(Executor::script));
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
				// call main
				runtime.evaluate("Sen.Script.main()"_sv, std::source_location::current().file_name());
				// Execute other Promise
				thiz.execute_promise_in_queue();
				#ifdef DEBUG
					runtime.dump_memory_usage();
				#endif
				return;
			}
	};
}		