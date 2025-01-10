#pragma once

#include "kernel/interface/script.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/interface/shell.hpp"

namespace Sen::Kernel::Interface {

	/**
	 * Use JS
	*/

	namespace JS = Sen::Kernel::JavaScript;

	// callback

	class Runtime {

		private:

			JS::Handler javascript;

			using None = void;

		public:

			explicit Runtime(
			) : javascript{}
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
					{
						javascript.add_proxy([](
							JSContext* context,
							JSValue value,
							int argc,
							JSValue* argv
						) -> JSValue {
							auto is_gui = std::unique_ptr<CStringView, StringFinalizer>(new CStringView(nullptr, 0), finalizer<CStringView>);
							auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
							construct_string_list(Array<std::string, 1>{std::string{ "is_gui" }}, parameters.operator*());
							Shell::callback(parameters.get(), is_gui.get());
							auto state = Converter::to_int32(std::string{is_gui->value, static_cast<std::size_t>(is_gui->size)}, "Cannot get is gui argument from Shell");
							auto destination = JS::Converter::to_bool(context, static_cast<bool>(state));
							return destination;
						}, std::to_array<std::string_view>({"Sen"_sv, "Shell"_sv}), "is_gui"_sv);
					}
					{
						javascript.add_proxy([](
							JSContext *context,
							JSValue value,
							int argc,
							JSValue *argv
						) -> JSValue {
							auto shell_version = std::unique_ptr<CStringView, StringFinalizer>(new CStringView(nullptr, 0), finalizer<CStringView>);
							auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
							construct_string_list(Array<std::string, 1>{std::string{"version"}}, parameters.operator*());
							Shell::callback(parameters.get(), shell_version.get());
							auto state = static_cast<int>(Converter::to_int32(std::string{ shell_version->value, static_cast<std::size_t>(shell_version->size) }, "Cannot get the Shell version"));
							auto destination = JS::Converter::to_number(context, state);
							return destination;
						}, std::to_array<std::string_view>({"Sen"_sv, "Shell"_sv}), "version"_sv);
					}
					// version
					javascript.add_proxy([](
						JSContext *context,
						JSValue value,
						int argc,
						JSValue *argv
					) -> JSValue {
						return JS::Converter::to_number(context, Kernel::version); 
					}, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "version"_sv);
					// callback
					javascript.add_proxy(Script::callback, std::to_array<std::string_view>({"Sen"_sv, "Shell"_sv}), "callback"_sv);
					// arguments
					javascript.add_proxy([](
						JSContext *context,
						JSValue value,
						int argc,
						JSValue *argv
					) -> JSValue 
					{
						return Script::to_array_of_string(context, Executor::arguments);
					}, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv}), "arguments"_sv);
				}
				// xml
				{
					// deserialize
					javascript.add_proxy(Script::XML::deserialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "deserialize"_sv);
					// deserialize_fs
					javascript.add_proxy(Script::XML::deserialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "deserialize_fs"_sv);
					// serialize
					javascript.add_proxy(Script::XML::serialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "serialize"_sv);
					// serialize_fs
					javascript.add_proxy(Script::XML::serialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "XML"_sv}), "serialize_fs"_sv);
				}
				// json
				{
					// deserialize
					javascript.add_proxy(Script::JSON::deserialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "deserialize"_sv);
					// serialize
					javascript.add_proxy(Script::JSON::serialize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "serialize"_sv);
					// deserialize fs
					javascript.add_proxy(Script::JSON::deserialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "deserialize_fs"_sv);
					// serialize fs
					javascript.add_proxy(Script::JSON::serialize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JSON"_sv}), "serialize_fs"_sv);
				}
				// home
				{
					// script path
					javascript.add_proxy([](
						JSContext *context,
						JSValue value,
						int argc,
						JSValue *argv
					) -> JSValue {
						return JS::Converter::to_string(context, String::to_posix_style(construct_string(Executor::script)));
					}, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Home"_sv}), "script"_sv);
				}
				// vcdiff
				{
					// encode fs
					javascript.add_proxy(Script::Diff::VCDiff::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Diff"_sv, "VCDiff"_sv}), "encode_fs"_sv);
					// decode fs
					javascript.add_proxy(Script::Diff::VCDiff::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Diff"_sv, "VCDiff"_sv}), "decode_fs"_sv);
				}
				// file system
				{
					// read file
					javascript.add_proxy(Script::FileSystem::read_file, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_file"_sv);
					// read_file_encode_with_utf16le
					javascript.add_proxy(Script::FileSystem::read_file_encode_with_utf16le, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_file_encode_with_utf16le"_sv);
					// write_file_encode_with_utf16le
					javascript.add_proxy(Script::FileSystem::write_file_encode_with_utf16le, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "write_file_encode_with_utf16le"_sv);
					// write file
					javascript.add_proxy(Script::FileSystem::write_file, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "write_file"_sv);
					// create directory
					javascript.add_proxy(Script::FileSystem::create_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "create_directory"_sv);
					// read_current_directory
					javascript.add_proxy(Script::FileSystem::read_current_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_current_directory"_sv);
					// read_directory_only_file
					javascript.add_proxy(Script::FileSystem::read_directory_only_file, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_directory_only_file"_sv);
					// read_directory_only_directory
					javascript.add_proxy(Script::FileSystem::read_directory_only_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_directory_only_directory"_sv);
					// read_directory
					javascript.add_proxy(Script::FileSystem::read_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "read_directory"_sv);
					// is file
					javascript.add_proxy(Script::FileSystem::is_file, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}), "is_file"_sv);
					// is directory
					javascript.add_proxy(Script::FileSystem::is_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv}),"is_directory"_sv);
					// operation : rename
					javascript.add_proxy(Script::FileSystem::Operation::rename, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "rename"_sv);
					// operation : remove
					javascript.add_proxy(Script::FileSystem::Operation::remove, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "remove"_sv);
					// operation : remove all
					javascript.add_proxy(Script::FileSystem::Operation::remove_all, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "remove_all"_sv);
					// operation : copy
					javascript.add_proxy(Script::FileSystem::Operation::copy, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "copy"_sv);
					// operation : copy_directory
					javascript.add_proxy(Script::FileSystem::Operation::copy_directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "FileSystem"_sv, "Operation"_sv}), "copy_directory"_sv);
				}
				// path
				{
					// join
					javascript.add_proxy(Script::Path::join, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "join"_sv);
					// basename
					javascript.add_proxy(Script::Path::basename, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "basename"_sv);
					// delimiter
					javascript.add_proxy(Script::Path::delimiter, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "delimiter"_sv);
					// dirname
					javascript.add_proxy(Script::Path::dirname, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "dirname"_sv);
					// format
					javascript.add_proxy(Script::Path::format, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "format"_sv);
					// normalize
					javascript.add_proxy(Script::Path::normalize, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "normalize"_sv);
					// resolve
					javascript.add_proxy(Script::Path::resolve, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "resolve"_sv);
					// relative
					javascript.add_proxy(Script::Path::relative, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "relative"_sv);
					// extname
					javascript.add_proxy(Script::Path::extname, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "extname"_sv);
					// is_absolute
					javascript.add_proxy(Script::Path::relative, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "is_absolute"_sv);
					// base_without_extension
					javascript.add_proxy(Script::Path::base_without_extension, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "base_without_extension"_sv);
					// except_extension
					javascript.add_proxy(Script::Path::except_extension, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Path"_sv}), "except_extension"_sv);
				}
				// console
				{
					// console print
					javascript.add_proxy(Script::Console::print, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Console"_sv}), "print"_sv);
					// read line
					javascript.add_proxy(Script::Console::readline, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Console"_sv}), "readline"_sv);
				}
				// language
				{
					// load language
					javascript.add_proxy(Script::Language::load_language, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Language"_sv}), "load_language"_sv);
					// get language
					javascript.add_proxy(Script::Language::get, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Language"_sv}), "get"_sv);
				}
				// process
				{
					// run
					javascript.add_proxy(Script::Process::run, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "run"_sv);
					// execute
					javascript.add_proxy(Script::Process::execute, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "execute"_sv);
					// is_exists_in_path_environment
					javascript.add_proxy(Script::Process::is_exists_in_path_environment, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "is_exists_in_path_environment"_sv);
					// get_path_environment
					javascript.add_proxy(Script::Process::get_path_environment, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Process"_sv}), "get_path_environment"_sv);
				}
				// thread
				{
					// sleep
					javascript.add_proxy(Script::Thread::sleep, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Thread"_sv}), "sleep"_sv);
					// now
					javascript.add_proxy(Script::Thread::now, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Thread"_sv}), "now"_sv);
				}
				// array buffer
				{
					// open
					javascript.add_proxy(Script::ArrayBuffer::open, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "ArrayBuffer"_sv}), "open"_sv);
					// out
					javascript.add_proxy(Script::ArrayBuffer::out, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "ArrayBuffer"_sv}), "out"_sv);
					// random
					javascript.add_proxy(Script::ArrayBuffer::random, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "ArrayBuffer"_sv}), "random"_sv);
					// fill
					javascript.add_proxy(Script::ArrayBuffer::fill, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "ArrayBuffer"_sv}), "fill"_sv);
				}
				// operating system
				{
					// current
					javascript.add_proxy(Script::OperatingSystem::current, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "OperatingSystem"_sv}), "current"_sv);
					// architecture
					javascript.add_proxy(Script::OperatingSystem::architecture, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "OperatingSystem"_sv}), "architecture"_sv);
				}
				// image
				{
					// scale_fs
					javascript.add_proxy(Script::Image::scale_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "scale_fs"_sv);
					// join_png
					javascript.add_proxy(Script::Image::join_png, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "join_png"_sv);
					// join
					javascript.add_proxy(Script::Image::join, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "join"_sv);
					// join_extend
					javascript.add_proxy(Script::Image::join_extend, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "join_extend"_sv);
					// transparent_fs
					javascript.add_proxy(Script::Image::transparent_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "transparent_fs"_sv);
					// resize_fs
					javascript.add_proxy(Script::Image::resize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "resize_fs"_sv);
					// rotate_fs
					javascript.add_proxy(Script::Image::rotate_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "rotate_fs"_sv);
					// cut_fs
					javascript.add_proxy(Script::Image::cut_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "cut_fs"_sv);
					// cut_multiple_fs
					javascript.add_proxy(Script::Image::cut_multiple_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "cut_multiple_fs"_sv);
					// cut_multiple_fs_asynchronous
					javascript.add_proxy(Script::Image::cut_multiple_fs_asynchronous, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "cut_multiple_fs_asynchronous"_sv);
				}
				// javascript
				{
					// evaluate
					javascript.add_proxy(Script::JavaScript::evaluate, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JavaScript"_sv}), "evaluate"_sv);
					// evaluate file
					javascript.add_proxy(Script::JavaScript::evaluate_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "JavaScript"_sv}), "evaluate_fs"_sv);
				}
				// md5
				{
					// hash method
					javascript.add_proxy(Script::Encryption::MD5::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "MD5"_sv}), "hash"_sv);
					// hash file method
					javascript.add_proxy(Script::Encryption::MD5::hash_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "MD5"_sv}), "hash_fs"_sv);
				}
				// rijndael
				{
					// encrypt
					javascript.add_proxy(Script::Encryption::Rijndael::encrypt, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "Rijndael"_sv}), "encrypt"_sv);
					// decrypt
					javascript.add_proxy(Script::Encryption::Rijndael::decrypt, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "Rijndael"_sv}), "decrypt"_sv);
				}
				// fnv
				{
					// hash method
					javascript.add_proxy(Script::Encryption::FNV::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "FNV"_sv}), "hash"_sv);
					// hash file method
					javascript.add_proxy(Script::Encryption::FNV::hash_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "FNV"_sv}), "hash_fs"_sv);
				}
				// sha224
				{
					// hash method
					javascript.add_proxy(Script::Encryption::SHA224::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA224"_sv}), "hash"_sv);
					// hash file method
					javascript.add_proxy(Script::Encryption::SHA224::hash_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA224"_sv}), "hash_fs"_sv);
				}
				// sha256
				{
					// hash method
					javascript.add_proxy(Script::Encryption::SHA256::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA256"_sv}), "hash"_sv);
					// hash file method
					javascript.add_proxy(Script::Encryption::SHA224::hash_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA256"_sv}), "hash_fs"_sv);
				}
				// sha384
				{
					// hash method
					javascript.add_proxy(Script::Encryption::SHA384::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA384"_sv}), "hash"_sv);
					// hash file method
					javascript.add_proxy(Script::Encryption::SHA384::hash_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA384"_sv}), "hash_fs"_sv);
				}
				// sha512
				{
					// hash method
					javascript.add_proxy(Script::Encryption::SHA512::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA512"_sv}), "hash"_sv);
					// hash file method
					javascript.add_proxy(Script::Encryption::SHA512::hash_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "SHA512"_sv}), "hash_fs"_sv);
				}
				// xor
				{
					// encrypt method
					javascript.add_proxy(Script::Encryption::XOR::encrypt, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "X0R"_sv}), "encrypt"_sv);
					// encrypt file method
					javascript.add_proxy(Script::Encryption::XOR::encrypt_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "X0R"_sv}), "encrypt_fs"_sv);
				}
				// zip
				{
					// compress directory
					javascript.add_proxy(Script::Compression::Zip::Compress::directory, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zip"_sv, "Compress"_sv}), "directory"_sv);
					// compress files
					javascript.add_proxy(Script::Compression::Zip::Compress::file, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zip"_sv, "Compress"_sv}), "file"_sv);
					// uncompress
					javascript.add_proxy(Script::Compression::Zip::Uncompress::process, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zip"_sv, "Uncompress"_sv}), "process"_sv);
				}
				// zlib
				{
					// compress file method
					javascript.add_proxy(Script::Compression::Zlib::compress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zlib"_sv}), "compress_fs"_sv);
					// uncompress file method
					javascript.add_proxy(Script::Compression::Zlib::uncompress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zlib"_sv}), "uncompress_fs"_sv);
					// uncompress method
					javascript.add_proxy(Script::Compression::Zlib::uncompress, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zlib"_sv}), "uncompress"_sv);
					// compress method
					javascript.add_proxy(Script::Compression::Zlib::compress, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Zlib"_sv}), "compress"_sv);
				}
				// gzip
				{
					// compress file method
					javascript.add_proxy(Script::Compression::Gzip::compress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Gzip"_sv}), "compress_fs"_sv);
					// uncompress file method
					javascript.add_proxy(Script::Compression::Gzip::uncompress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Gzip"_sv}), "uncompress_fs"_sv);
				}
				// bzip2
				{
					// compress file method
					javascript.add_proxy(Script::Compression::Bzip2::compress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Bzip2"_sv}), "compress_fs"_sv);
					// uncompress file method
					javascript.add_proxy(Script::Compression::Bzip2::uncompress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Bzip2"_sv}), "uncompress_fs"_sv);
				}
				// lzma
				{
					// compress file method
					javascript.add_proxy(Script::Compression::Lzma::compress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Lzma"_sv}), "compress_fs"_sv);
					// uncompress file method
					javascript.add_proxy(Script::Compression::Lzma::uncompress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Compression"_sv, "Lzma"_sv}), "uncompress_fs"_sv);
				}
				// base64
				{
					// encode base64
					javascript.add_proxy(Script::Encryption::Base64::encode, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "Base64"_sv}), "encode"_sv);
					// decode base64
					javascript.add_proxy(Script::Encryption::Base64::decode, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "Base64"_sv}), "decode"_sv);
					// encode base64 for file
					javascript.add_proxy(Script::Encryption::Base64::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "Base64"_sv}), "encode_fs"_sv);
					// decode base64 for file
					javascript.add_proxy(Script::Encryption::Base64::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Encryption"_sv, "Base64"_sv}), "decode_fs"_sv);
				}
				// texture
				{
					// decode fs
					javascript.add_proxy(Script::Support::Texture::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Texture"_sv}),"decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::Texture::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Texture"_sv}), "encode_fs"_sv);
				}
				// dzip
				{
					// unpack_fs
					javascript.add_proxy(Script::Support::Marmalade::DZip::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Marmalade"_sv, "DZip"_sv}), "unpack_fs"_sv);
					// pack_fs
					javascript.add_proxy(Script::Support::Marmalade::DZip::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Marmalade"_sv, "DZip"_sv}), "pack_fs"_sv);
				}
				// pak
				{
					// unpack_fs
					javascript.add_proxy(Script::Support::PopCap::PAK::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PAK"_sv}), "unpack_fs"_sv);
					// pack_fs
					javascript.add_proxy(Script::Support::PopCap::PAK::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PAK"_sv}), "pack_fs"_sv);
				}
				// newton
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::Newton::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Newton"_sv}), "decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::Newton::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Newton"_sv}), "encode_fs"_sv);
				}
				// rton
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::RTON::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "decode_fs"_sv);
					// decrypt fs
					javascript.add_proxy(Script::Support::PopCap::RTON::decrypt_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "decrypt_fs"_sv);
					// decrypt_and_decode_fs
					javascript.add_proxy(Script::Support::PopCap::RTON::decrypt_and_decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "decrypt_and_decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::RTON::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "encode_fs"_sv);
					// encrypt fs
					javascript.add_proxy(Script::Support::PopCap::RTON::encrypt_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "encrypt_fs"_sv);
					// encode_and_encrypt_fs
					javascript.add_proxy(Script::Support::PopCap::RTON::encode_and_encrypt_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RTON"_sv}), "encode_and_encrypt_fs"_sv);
				}
				// zlib
				{
					// uncompress fs
					javascript.add_proxy(Script::Support::PopCap::Zlib::uncompress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Zlib"_sv}), "uncompress_fs"_sv);
					// compress fs
					javascript.add_proxy(Script::Support::PopCap::Zlib::compress_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Zlib"_sv}), "compress_fs"_sv);
					// uncompress
					javascript.add_proxy(Script::Support::PopCap::Zlib::uncompress, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Zlib"_sv}), "uncompress"_sv);
					// compress
					javascript.add_proxy(Script::Support::PopCap::Zlib::compress, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Zlib"_sv}), "compress"_sv);
				}
				// compiled text
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::CompiledText::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CompiledText"_sv}), "decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::CompiledText::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CompiledText"_sv}), "encode_fs"_sv);
				}
				// render-effects
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::RenderEffects::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RenderEffects"_sv}), "decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::RenderEffects::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RenderEffects"_sv}), "encode_fs"_sv);
				}
				// crypt-data
				{
					// decrypt fs
					javascript.add_proxy(Script::Support::PopCap::CryptData::decrypt_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CryptData"_sv}), "decrypt_fs"_sv);
					// encrypt fs
					javascript.add_proxy(Script::Support::PopCap::CryptData::encrypt_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CryptData"_sv}), "encrypt_fs"_sv);
				}
				// character-font-widget-2
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::CharacterFontWidget2::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CharacterFontWidget2"_sv}), "decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::CharacterFontWidget2::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "CharacterFontWidget2"_sv}), "encode_fs"_sv);
				}
				// particles
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::Particles::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::Particles::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "encode_fs"_sv);

					javascript.add_proxy(Script::Support::PopCap::Particles::to_xml, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "to_xml"_sv);

					javascript.add_proxy(Script::Support::PopCap::Particles::from_xml, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Particles"_sv}), "from_xml"_sv);
				}
				// player_info
				{
					// decode fs
					javascript.add_proxy(Script::Support::PopCap::PlayerInfo::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PlayerInfo"_sv}), "decode_fs"_sv);
					// encode fs
					javascript.add_proxy(Script::Support::PopCap::PlayerInfo::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "PlayerInfo"_sv}), "encode_fs"_sv);
				}
				// rsb
				{
					// unpack_fs
					javascript.add_proxy(Script::Support::PopCap::RSB::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "unpack_fs"_sv);
					// pack_fs
					javascript.add_proxy(Script::Support::PopCap::RSB::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "pack_fs"_sv);
					// unpack_cipher
					javascript.add_proxy(Script::Support::PopCap::RSB::unpack_cipher, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "unpack_cipher"_sv);
					// unpack_resource
					javascript.add_proxy(Script::Support::PopCap::RSB::unpack_resource, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "unpack_resource"_sv);
					// pack_resource
					javascript.add_proxy(Script::Support::PopCap::RSB::pack_resource, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv,"RSB"_sv}), "pack_resource"_sv);
				}
				// rsb-patch
				{
					// decode_fs
					javascript.add_proxy(Script::Support::PopCap::RSBPatch::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSBPatch"_sv}), "decode_fs"_sv);
					// encode_fs
					javascript.add_proxy(Script::Support::PopCap::RSBPatch::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSBPatch"_sv}), "encode_fs"_sv);
				}
				// rsg
				{
					// unpack_fs
					javascript.add_proxy(Script::Support::PopCap::RSG::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSG"_sv}), "unpack_fs"_sv);
					// pack_fs
					javascript.add_proxy(Script::Support::PopCap::RSG::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "RSG"_sv}), "pack_fs"_sv);
				}
				// pam
				{
					// decode_fs
					javascript.add_proxy(Script::Support::PopCap::Animation::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv}), "decode_fs"_sv);
					// encode_fs
					javascript.add_proxy(Script::Support::PopCap::Animation::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv}), "encode_fs"_sv);
					// convert_fs
					javascript.add_proxy(Script::Support::PopCap::Animation::ToFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "ToFlash"_sv}), "convert_fs"_sv);
					// process
					javascript.add_proxy(Script::Support::PopCap::Animation::ToFlash::process, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "ToFlash"_sv}), "process"_sv);
					// convert_fs
					javascript.add_proxy(Script::Support::PopCap::Animation::FromFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "FromFlash"_sv}), "convert_fs"_sv);
					// to_flash
					javascript.add_proxy(Script::Support::PopCap::Animation::Instance::to_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Instance"_sv}), "to_flash"_sv);
					// from_flash
					javascript.add_proxy(Script::Support::PopCap::Animation::Instance::from_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Instance"_sv}), "from_flash"_sv);
					// resize_fs
					javascript.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::resize_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"}), "resize_fs"_sv);
					// dump_document
					javascript.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::dump_document, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "dump_document"_sv);
					// generate_document
					javascript.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::generate_document, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "generate_document"_sv);
					// generate_image
					javascript.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::generate_image, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "generate_image"_sv);
					// generate_sprite
					javascript.add_proxy(Script::Support::PopCap::Animation::Miscellaneous::generate_sprite, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "Animation"_sv, "Miscellaneous"_sv}), "generate_sprite"_sv);
				}
				// reanim
				{
					// decode_fs
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "decode_fs"_sv);
					// encode_fs
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "encode_fs"_sv);
					// to_xml
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::to_xml, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "to_xml"_sv);
					// from_xml
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::from_xml, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv}), "from_xml"_sv);
					// convert_fs
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::ToFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "ToFlash"_sv}), "convert_fs"_sv);
					// convert_fs
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::FromFlash::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "FromFlash"_sv}), "convert_fs"_sv);
					// to_flash
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::Instance::to_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "Instance"_sv}), "to_flash"_sv);
					// from_flash
					javascript.add_proxy(Script::Support::PopCap::ReAnimation::Instance::from_flash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ReAnimation"_sv, "Instance"_sv}), "from_flash"_sv);
				}
				// bnk
				{
					// decode_fs
					javascript.add_proxy(Script::Support::WWise::SoundBank::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv}), "decode_fs"_sv);
					// encode_fs
					javascript.add_proxy(Script::Support::WWise::SoundBank::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv}), "encode_fs"_sv);
					// hash
					javascript.add_proxy(Script::Support::WWise::SoundBank::hash, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv}), "hash"_sv);
					// add music
					javascript.add_proxy(Script::Support::WWise::SoundBank::Miscellaneous::add_music, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv, "Miscellaneous"_sv}), "add_music"_sv);
					// create soundbank
					javascript.add_proxy(Script::Support::WWise::SoundBank::Miscellaneous::create_soundbank, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "WWise"_sv, "SoundBank"_sv, "Miscellaneous"_sv}), "create_soundbank"_sv);
				}
				// resource group
				{
					// split_fs
					javascript.add_proxy(Script::Support::PopCap::ResourceGroup::split_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResourceGroup"_sv}), "split_fs"_sv);
					// merge_fs
					javascript.add_proxy(Script::Support::PopCap::ResourceGroup::merge_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResourceGroup"_sv}), "merge_fs"_sv);
					// convert_fs
					javascript.add_proxy(Script::Support::PopCap::ResourceGroup::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResourceGroup"_sv}), "convert_fs"_sv);
				}
				// res info
				{
					// split_fs
					javascript.add_proxy(Script::Support::PopCap::ResInfo::split_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResInfo"_sv}), "split_fs"_sv);
					// merge_fs
					javascript.add_proxy(Script::Support::PopCap::ResInfo::merge_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResInfo"_sv}), "merge_fs"_sv);
					// convert_fs
					javascript.add_proxy(Script::Support::PopCap::ResInfo::convert_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "PopCap"_sv, "ResInfo"_sv}), "convert_fs"_sv);
				}
				// custom
				{
					// to_resource_custom
					javascript.add_proxy(Script::Support::PopCap::Custom::Debug::to_resource_custom, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "Debug"_sv}), "to_resource_custom"_sv);
					// from_resource_custom
					javascript.add_proxy(Script::Support::PopCap::Custom::Debug::from_resource_custom, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "Debug"_sv}), "from_resource_custom"_sv);
					// check_scg_composite
					javascript.add_proxy(Script::Support::PopCap::Custom::StreamCompressedGroup::check_scg_composite, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "StreamCompressedGroup"_sv}), "check_scg_composite"_sv);
					// unpack_scg
					javascript.add_proxy(Script::Support::PopCap::Custom::StreamCompressedGroup::decode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "StreamCompressedGroup"_sv}), "decode_fs"_sv);
					// pack_scg
					javascript.add_proxy(Script::Support::PopCap::Custom::StreamCompressedGroup::encode_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "StreamCompressedGroup"_sv}), "encode_fs"_sv);
					// unpack_rsb
					javascript.add_proxy(Script::Support::PopCap::Custom::ResourceStreamBundle::unpack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "ResourceStreamBundle"_sv}), "unpack_fs"_sv);
					// pack_rsb
					javascript.add_proxy(Script::Support::PopCap::Custom::ResourceStreamBundle::pack_fs, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Support"_sv, "Miscellaneous"_sv, "Custom"_sv, "ResourceStreamBundle"_sv}), "pack_fs"_sv);
				}
				// dimension
				{
					// instance of dimension
					javascript.add_proxy(Script::Dimension::instance, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Dimension"_sv}), "instance"_sv);
					// open the image
					javascript.add_proxy(Script::Dimension::open, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "open"_sv);
					// write the image
					javascript.add_proxy(Script::Dimension::write, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Image"_sv}), "write"_sv);
				}
				// miscellaneous
				{
					// make_copy
					javascript.add_proxy(Script::Miscellaneous::make_copy, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "make_copy"_sv);
					// cast_ArrayBuffer_to_JS_String
					javascript.add_proxy(Script::Miscellaneous::cast_ArrayBuffer_to_JS_String, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "cast_ArrayBuffer_to_JS_String"_sv);
					// cast_ArrayBuffer_to_JS_WideString
					javascript.add_proxy(Script::Miscellaneous::cast_ArrayBuffer_to_JS_WideString, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "cast_ArrayBuffer_to_JS_WideString"_sv);
					// cast_movable_String_to_ArrayBuffer
					javascript.add_proxy(Script::Miscellaneous::cast_movable_String_to_ArrayBuffer, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "cast_movable_String_to_ArrayBuffer"_sv);
					// copyArrayBuffer
					javascript.add_proxy(Script::Miscellaneous::copyArrayBuffer, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "copyArrayBuffer"_sv);
					// compareArrayBuffer
					javascript.add_proxy(Script::Miscellaneous::compareArrayBuffer, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "compareArrayBuffer"_sv);
					// to_apng
					javascript.add_proxy(Script::Miscellaneous::to_apng, std::to_array<std::string_view>({"Sen"_sv, "Kernel"_sv, "Miscellaneous"_sv}), "to_apng"_sv);
				}
				// DataStreamView
				javascript.register_object(Script::Class::DataStreamView::register_class<false>);
				// DataStreamViewUseBigEndian
				javascript.register_object(Script::Class::DataStreamView::register_class<true>);
				// Boolean
				javascript.register_object(Script::Class::Boolean::register_class);
				// // Size
				javascript.register_object(Script::Class::Size::register_class);
				// // String
				javascript.register_object(Script::Class::String::register_class);
				// Character
				javascript.register_object(Script::Class::Character::register_class<char>);
				// UCharacter
				javascript.register_object(Script::Class::Character::register_class<unsigned char>);
				// WideCharacter
				javascript.register_object(Script::Class::Character::register_class<wchar_t>);
				// DimensionView
				javascript.register_object(Script::Class::DimensionView::register_class);
				// Rectangle
				javascript.register_object(Script::Class::Rectangle::register_class);
				// ImageView
				javascript.register_object(Script::Class::ImageView::register_class);
				// Canvas
				javascript.register_object(Script::Class::Canvas::register_class);
				// BinaryView
				javascript.register_object(Script::Class::BinaryView::register_class);
				// Integer8
				javascript.register_object(Script::Class::Number::register_class<int8_t>);
				// Integer16
				javascript.register_object(Script::Class::Number::register_class<int16_t>);
				// Integer32
				javascript.register_object(Script::Class::Number::register_class<int32_t>);
				// Integer64
				javascript.register_object(Script::Class::Number::register_class<int64_t>);
				// UInteger8
				javascript.register_object(Script::Class::Number::register_class<uint8_t>);
				// UInteger16
				javascript.register_object(Script::Class::Number::register_class<uint16_t>);
				// UInteger32
				javascript.register_object(Script::Class::Number::register_class<uint32_t>);
				// UInteger64
				javascript.register_object(Script::Class::Number::register_class<uint64_t>);
				// Float
				javascript.register_object(Script::Class::Number::register_class<float>);
				// Double
				javascript.register_object(Script::Class::Number::register_class<double>);
				// JsonWriter
				javascript.register_object(Script::Class::JsonWriter::register_class);
				// APNGMakerSetting
				javascript.register_object(Script::Class::APNGMakerSetting::register_class);
				// FileInputStream
				javascript.register_object(Script::Class::FileInputStream::register_class);
				// FileOutputStream
				javascript.register_object(Script::Class::FileOutputStream::register_class);
				// FileStream
				javascript.register_object(Script::Class::FileStream::register_class);
				// Image
				javascript.register_object(Script::Class::Image::register_class);
				// Sprite
				javascript.register_object(Script::Class::Sprite::register_class);
				// Clock
				javascript.register_object(Script::Clock::register_class);
				// execute the script
				javascript.evaluate_fs(construct_string(Executor::script));
				return;
			}

			// Execute other Promise if quickjs found any

			inline auto execute_promise_in_queue (

			) -> void
			{
				while (javascript.has_promise()) {
					javascript.execute_pending_job();
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
				javascript.evaluate("Sen.Script.main()"_sv, std::source_location::current().file_name());
				// Execute other Promise
				thiz.execute_promise_in_queue();
				#ifdef DEBUG
					javascript.dump_memory_usage();
				#endif
				return;
			}
	};
}		