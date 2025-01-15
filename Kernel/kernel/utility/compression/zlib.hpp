#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/filesystem.hpp"

namespace Sen::Kernel::Compression {

	class Zlib {

		private:

			// zlib compression check

			inline static constexpr auto Z_COMPRESS_END = 0;

			// zlib uncompression check

			inline static constexpr auto Z_UNCOMPRESS_END = 0;

			// chunk

			inline static constexpr auto CHUNK = 65536;
			

			inline static constexpr auto RESERVE_MULTIPLIER = 4;

		public:

			/**
			 * Constructor
			*/

			explicit Zlib(

			) = default;

			/**
			 * Destructor
			*/

			~Zlib(

			) = default;
			

			/**
			 * Zlib compression level supported by zlib.h
			*/

			enum class Level
			{
				DEFAULT = -1,
				LEVEL_0,
				LEVEL_1,
				LEVEL_2,
				LEVEL_3,
				LEVEL_4,
				LEVEL_5,
				LEVEL_6,
				LEVEL_7,
				LEVEL_8,
				LEVEL_9,
			};

			/**
			 * data: the binary data to compress
			 * level: zlib compression level
			 * return: the compressed data
			 */
			template <auto level>
			inline static auto compress(
				const List<unsigned char> &data
			) -> List<unsigned char> 
			{
				auto destLen = compressBound(data.size());
				auto zlib_outdata = List<unsigned char>{};
				zlib_outdata.reserve(static_cast<size_t>(destLen));
				auto ret = compress2(zlib_outdata.data(), &destLen, data.data(), data.size(), static_cast<int>(level));
				assert_conditional(ret == Z_OK, fmt::format("{}", Kernel::Language::get("zlib.compress.failed")), "compress");
				zlib_outdata.resize(destLen);
				return zlib_outdata;
			}

			/**
			 * data: the binary data to uncompress
			 * return: the uncompressed data
			 */
			
			inline static auto uncompress(
				const List<unsigned char> &data
			) -> List<unsigned char>
			{
				auto zlib_init = z_stream {
					.next_in = reinterpret_cast<Bytef*>(const_cast<unsigned char*>(data.data())),
					.avail_in = static_cast<uInt>(data.size()),
					.zalloc = Z_NULL,
					.zfree = Z_NULL,
					.opaque = Z_NULL,
				};
				inflateInit(&zlib_init);
				auto result = List<unsigned char>{};
    			result.reserve(data.size() * Zlib::RESERVE_MULTIPLIER);
				auto out_chunk = std::array<unsigned char, Zlib::CHUNK>{};
				do {
					zlib_init.avail_out = static_cast<uInt>(out_chunk.size());
        			zlib_init.next_out = reinterpret_cast<Bytef*>(out_chunk.data());
					auto ret = inflate(&zlib_init, Z_NO_FLUSH);
					assert_conditional(ret != Z_STREAM_ERROR, fmt::format("{}", Kernel::Language::get("zlib.uncompress.failed")), "uncompress");
					switch(ret){
						case Z_NEED_DICT:{
							ret = Z_DATA_ERROR;
						}
						case Z_DATA_ERROR:
						case Z_MEM_ERROR:{
							inflateEnd(&zlib_init);
							return List<unsigned char>();
						}
					}
					result.insert(result.end(), out_chunk.begin(), out_chunk.begin() + (out_chunk.size() - zlib_init.avail_out));
				} while (zlib_init.avail_out == Zlib::Z_UNCOMPRESS_END);
				inflateEnd(&zlib_init);
				return result;
			}

			/**
			 * data: the binary data to uncompress
			 * return: the uncompressed data
			 */
			
			inline static auto uncompress_deflate(
				const List<unsigned char> &data
			) -> List<unsigned char>
			{
				auto zlib_init = z_stream {
					.next_in = const_cast<Bytef*>(data.data()),
					.avail_in = static_cast<uInt>(data.size()),
					.zalloc = Z_NULL,
					.zfree = Z_NULL,
					.opaque = Z_NULL,
				};
				inflateInit2(&zlib_init, -15);
				auto result = List<unsigned char>{};
				result.reserve(data.size() * Zlib::RESERVE_MULTIPLIER);
				auto out_chunk = std::array<unsigned char, Zlib::CHUNK>{};
				do {
					zlib_init.avail_out = static_cast<uInt>(out_chunk.size());
        			zlib_init.next_out = reinterpret_cast<Bytef*>(out_chunk.data());
					auto ret = inflate(&zlib_init, Z_NO_FLUSH);
					assert_conditional(ret != Z_STREAM_ERROR, fmt::format("{}", Kernel::Language::get("zlib.uncompress.failed")), "uncompress_deflate");
					switch(ret){
						case Z_NEED_DICT:{
							ret = Z_DATA_ERROR;
						}
						case Z_DATA_ERROR:
						case Z_MEM_ERROR:{
							inflateEnd(&zlib_init);
							return List<unsigned char>();
						}
					}
					result.insert(result.end(), out_chunk.begin(), out_chunk.begin() + (out_chunk.size() - zlib_init.avail_out));
				} while (zlib_init.avail_out == Zlib::Z_UNCOMPRESS_END);
				inflateEnd(&zlib_init);
				return result;
			}

			/**
			 * data: the binary data to compress
			 * level: zlib level to compress, should use ZlibLevel::LEVEL_9 for BEST COMPRESSION
			 * return: the compressed data
			 */

			template <auto level>
			inline static auto compress_deflate(
				const List<unsigned char> &data
			) ->  List<unsigned char>
			{
				auto zlib_outdata = List<unsigned char>{};
				zlib_outdata.reserve(static_cast<size_t>(compressBound(static_cast<uLong>(data.size()))));
				auto zlib_init = z_stream{
					.next_in = const_cast<unsigned char*>(data.data()),
					.avail_in = static_cast<uInt>(data.size()),
					.next_out = const_cast<Bytef*>(zlib_outdata.data()),
					.avail_out = static_cast<uInt>(zlib_outdata.size()),
					.zalloc = Z_NULL,
					.zfree = Z_NULL,
					.opaque = Z_NULL,
				};
				auto ret = deflateInit(&zlib_init, static_cast<int>(level));
				assert_conditional(ret == Z_OK, fmt::format("{}", Kernel::Language::get("zlib.compress.failed")), "compress_deflate");
				while (zlib_init.avail_in != Zlib::Z_COMPRESS_END){
					deflate(&zlib_init, Z_NO_FLUSH);
				}
				auto deflate_res = Z_OK;
				while (deflate_res == Z_OK){
					if(zlib_init.avail_out == Zlib::Z_COMPRESS_END){
						zlib_outdata.insert(zlib_outdata.end(), zlib_outdata.size());
						zlib_init.next_out = const_cast<Bytef*>(&zlib_outdata[zlib_outdata.size() / 2]);
						zlib_init.avail_out = static_cast<uInt>(zlib_outdata.size() / 2);
					}
					deflate_res = deflate(&zlib_init, Z_FINISH);
				}
				assert_conditional(deflate_res == Z_STREAM_END, fmt::format("{}", Kernel::Language::get("zlib.compress.failed")), "compress_deflate");
				zlib_outdata.resize(zlib_outdata.size() - zlib_init.avail_out);
				deflateEnd(&zlib_init);
				return zlib_outdata;
			}

			/**
			 * data: data stream
			 * level: zlib level
			 * return: compressed data
			*/
			template <auto level>
			inline static auto compress_gzip(
				const List<unsigned char> &data
			) -> List<unsigned char>
			{
				auto zlib_init = z_stream {
					.next_in = reinterpret_cast<Bytef*>(const_cast<unsigned char*>(data.data())),
					.avail_in = static_cast<uInt>(data.size()),
					.zalloc = Z_NULL,
					.zfree = Z_NULL,
					.opaque = Z_NULL
				};
				deflateInit2(&zlib_init, static_cast<int>(level), Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY);
				auto result = List<unsigned char>{};
				result.reserve(static_cast<size_t>(compressBound(static_cast<uLong>(data.size()))));
				auto out_chunk = std::array<unsigned char, Zlib::CHUNK>{};
				do {
					zlib_init.avail_out = static_cast<uInt>(out_chunk.size());
					zlib_init.next_out = reinterpret_cast<Bytef*>(out_chunk.data());
					auto ret = deflate(&zlib_init, Z_FINISH);
					assert_conditional(ret != Z_STREAM_ERROR, fmt::format("{}", Kernel::Language::get("zlib.compress.failed")), "compress_gzip");
					result.insert(result.end(), out_chunk.begin(), out_chunk.begin() + (out_chunk.size() - zlib_init.avail_out));
				} while (zlib_init.avail_out == Zlib::Z_COMPRESS_END);
				deflateEnd(&zlib_init);
				return result;
			}

			/**
			 * data: data stream
			 * result: output stream
			*/

			inline static auto uncompress_gzip(
				const List<unsigned char> &data
			) -> List<unsigned char>
			{
				auto zlib_init = z_stream {
					.next_in = Z_NULL,
					.avail_in = 0,
					.zalloc = Z_NULL,
					.zfree = Z_NULL,
					.opaque = Z_NULL,
				};
				if (inflateInit2(&zlib_init, 16 + MAX_WBITS) != Z_OK){
					throw Exception(fmt::format("{}", Language::get("gzip.init_stream.failed")), std::source_location::current(), "uncompress_gzip");
				}
				zlib_init.avail_in = static_cast<uInt>(data.size());
				zlib_init.next_in = reinterpret_cast<Bytef*>(const_cast<unsigned char*>(data.data()));
				auto result = List<unsigned char>{};
				result.reserve(data.size() * Zlib::RESERVE_MULTIPLIER);
				do {
        			auto out_chunk = std::array<unsigned char, Zlib::CHUNK>{};
					zlib_init.avail_out = static_cast<uInt>(out_chunk.size());
        			zlib_init.next_out = reinterpret_cast<Bytef*>(out_chunk.data());
					auto ret = inflate(&zlib_init, 0);
					if(result.size() < zlib_init.total_out){
						result.insert(result.end(), out_chunk.begin(), out_chunk.begin() + (out_chunk.size() - zlib_init.avail_out));
					}
					if(ret == Z_STREAM_END){
						break;
					}
					if(ret != Z_OK){
						inflateEnd(&zlib_init);
						throw Exception(fmt::format("{}", Language::get("gzip.uncompress.failed")), std::source_location::current(), "uncompress_gzip"); 
					}
				} while (zlib_init.avail_out == Zlib::Z_UNCOMPRESS_END);
				inflateEnd(&zlib_init);
				return result;
			}

			/**
			 * source: input file
			 * destination: output file
			 * level: zlib level
			 * return: compress file
			*/

			inline static auto compress_fs(
				std::string_view source,
				std::string_view destination,
				Level level
			) -> void
			{
				auto data = FileSystem::read_binary<unsigned char>(source);
				switch (level) {
					case Level::DEFAULT:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_6>(data));
						break;
					}
					case Level::LEVEL_0:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_0>(data));
						break;
					}
					case Level::LEVEL_1:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_1>(data));
						break;
					}
					case Level::LEVEL_2:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_2>(data));
						break;
					}
					case Level::LEVEL_3:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_3>(data));
						break;
					}
					case Level::LEVEL_4:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_4>(data));
						break;
					}
					case Level::LEVEL_5:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_5>(data));
						break;
					}
					case Level::LEVEL_6:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_6>(data));
						break;
					}
					case Level::LEVEL_7:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_7>(data));
						break;
					}
					case Level::LEVEL_8:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_8>(data));
						break;
					}
					case Level::LEVEL_9:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_9>(data));
						break;
					}
					default:{
						throw Exception(fmt::format("{}", Kernel::Language::get("zlib.compress.invalid_level")), std::source_location::current(), "compress_fs");
					}
				}
				return;
			}

			/**
			 * source: input file
			 * destination: output file
			 * return: uncompress file
			*/

			inline static auto uncompress_fs(
				std::string_view source,
				std::string_view destination
			) -> void
			{
				auto data = FileSystem::read_binary<unsigned char>(source);
				auto uncompressedData = Zlib::uncompress(data);
				if(uncompressedData.empty()){
					throw Exception(fmt::format("{}: {}", Language::get("gzip.uncompress.file_is_not_compressed"), source), std::source_location::current(), "uncompress_fs");
				}
				FileSystem::write_binary<unsigned char>(destination, uncompressedData);
				return;
			}

			/**
			 * file in: file path
			 * file out: output path
			 * return: compressed gzip file
			*/

			inline static auto compress_gzip_fs(
				std::string_view source,
				std::string_view destination,
				Level level
			) -> void
			{
				auto data = FileSystem::read_binary<unsigned char>(source);
				switch (level){
					case Level::DEFAULT:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_6>(data));
						break;
					}
					case Level::LEVEL_0:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_0>(data));
						break;
					}
					case Level::LEVEL_1:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_1>(data));
						break;
					}
					case Level::LEVEL_2:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_2>(data));
						break;
					}
					case Level::LEVEL_3:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_3>(data));
						break;
					}
					case Level::LEVEL_4:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_4>(data));
						break;
					}
					case Level::LEVEL_5:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_5>(data));
						break;
					}
					case Level::LEVEL_6:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_6>(data));
						break;
					}
					case Level::LEVEL_7:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_7>(data));
						break;
					}
					case Level::LEVEL_8:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_8>(data));
						break;
					}
					case Level::LEVEL_9:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_gzip<Level::LEVEL_9>(data));
						break;
					}
					default: {
						throw Exception(fmt::format("{}", Kernel::Language::get("zlib.compress.invalid_level")), std::source_location::current(), "compress_gzip_fs");
					}
				}
				return;
			}

			/**
			 * source: input file
			 * destination: output file
			 * level: zlib level
			 * return: compress file
			*/

			inline static auto compress_deflate_fs(
				std::string_view source,
				std::string_view destination,
				Level level
			) -> void
			{
				auto data = FileSystem::read_binary<unsigned char>(source);
				switch (level){
					case Level::DEFAULT:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_6>(data));
						break;
					}
					case Level::LEVEL_0:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_0>(data));
						break;
					}
					case Level::LEVEL_1:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_1>(data));
						break;
					}
					case Level::LEVEL_2:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_2>(data));
						break;
					}
					case Level::LEVEL_3:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_3>(data));
						break;
					}
					case Level::LEVEL_4:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_4>(data));
						break;
					}
					case Level::LEVEL_5:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_5>(data));
						break;
					}
					case Level::LEVEL_6:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_6>(data));
						break;
					}
					case Level::LEVEL_7:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_7>(data));
						break;
					}
					case Level::LEVEL_8:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_8>(data));
						break;
					}
					case Level::LEVEL_9:{
						FileSystem::write_binary<unsigned char>(destination, Zlib::compress_deflate<Level::LEVEL_9>(data));
						break;
					}
					default: {
						throw Exception(fmt::format("{}", Kernel::Language::get("zlib.compress.invalid_level")), std::source_location::current(), "compress_deflate_fs");
					}
				}
				return;
			}

			/**
			 * source: file path
			 * file out: output path
			 * return: uncompressed file
			*/

			inline static auto uncompress_gzip_fs(
				std::string_view source,
				std::string_view destination
			) -> void
			{
				auto data = FileSystem::read_binary<unsigned char>(source);
				auto compressed_data = Zlib::uncompress_gzip(data);
				FileSystem::write_binary<unsigned char>(destination, compressed_data);
				return;
			}

	};
}