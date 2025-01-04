#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/container/filesystem.hpp"
#include "kernel/utility/container/path.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Encryption {

	/**
	 * Base64 class to decode & encode
	*/

	class Base64 {

		public:

			/**
			 * Pass message to decode
			*/

			inline static auto decode(
				const std::string &message
			) -> std::string const
			{
				return Sen::Kernel::Dependencies::Base64::base64_decode(message);
			}

			/**
			 * Pass message to encode
			*/

			inline static auto encode(
				const std::string &message
			) -> std::string const
			{
				return Sen::Kernel::Dependencies::Base64::base64_encode(message);
			}

			/**
			 * Pass message to encode
			*/

			inline static auto encode(
				const unsigned char* message,
				std::size_t size
			) -> std::string const
			{
				return Sen::Kernel::Dependencies::Base64::base64_encode(message, size);
			}

			
			/**
			 * filePath: the file path to decode
			 * outPath: output file
			*/

			inline static auto decode_fs(
				std::string_view source,
				std::string_view destination
			) -> void
			{
				FileSystem::write_file(destination, Encryption::Base64::decode(FileSystem::read_file(source)));
				return;
			}

			/**
			 * filePath: the file path to encode
			 * outPath: output file
			*/

			inline static auto encode_fs(
				std::string_view source,
				std::string_view destination
			) -> void
			{
				FileSystem::write_file(destination, Encryption::Base64::encode(FileSystem::read_file(source)));
				return;
			}

			/**
			 * Process
			*/

			inline static auto encode_fs_as_multiple_thread(
				const List<List<std::string>> & paths
			) -> void 
			{
				auto threads = List<std::thread>{};
				auto file_mutexes = std::unordered_map<std::string, std::mutex>{};
				for (const auto & data : paths) {
					threads.emplace_back([=, &file_mutexes]() { 
					auto lock_source = std::lock_guard<std::mutex>(file_mutexes[data[0]]);
					auto lock_destination = std::lock_guard<std::mutex>(file_mutexes[data[1]]);
						Base64::encode_fs(data[0], data[1]); 
					});
				}
				for (auto & thread : threads) {
					thread.join();
				}
				return;
			}

			/**
			 * Process
			*/

			inline static auto decode_fs_as_multiple_thread(
				const List<List<std::string>> & paths
			) -> void 
			{
				auto threads = List<std::thread>{};
				auto file_mutexes = std::unordered_map<std::string, std::mutex>{};
				for (const auto & data : paths) {
					threads.emplace_back([=, &file_mutexes]() { 
					auto lock_source = std::lock_guard<std::mutex>(file_mutexes[data[0]]);
					auto lock_destination = std::lock_guard<std::mutex>(file_mutexes[data[1]]);
						Base64::decode_fs(data[0], data[1]); 
					});
				}
				for (auto & thread : threads) {
					thread.join();
				}
				return;
			}
	};
}