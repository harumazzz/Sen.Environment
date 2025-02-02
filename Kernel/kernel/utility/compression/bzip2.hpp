#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Compression {

	/**
	 * Bzip2 kernel
	*/

	struct Bzip2 {

		private:

			// CHUNK

			inline static auto constexpr CHUNK = 4096;

			// BLOCK SIZE

			inline static auto constexpr BLOCK_SIZE = 9;

			// WORK FACTOR

			inline static auto constexpr WORK_FACTOR = 30;

		public:

			/**
			 * Constructor
			*/

			explicit Bzip2(

			) = default;

			/**
			 * Destructor
			*/

			~Bzip2(

			) = default;

			/**
			 * input: the unsigned char stream
			 * block_size: block size
			 * work_factor: work factor
			 * return: compressed bzip2 stream
			*/
			template <typename T> requires std::is_integral<T>::value
			inline static auto compress(
				const List<unsigned char>& input,
				T block_size,
				T work_factor
			) -> List<unsigned char> 
			{
				auto bzerror = int{};
				auto strm = bz_stream{};
				strm.bzalloc = nullptr;
				strm.bzfree = nullptr;
				strm.opaque = nullptr;
				BZ2_bzCompressInit(&strm, block_size, 0, work_factor);
				strm.next_in = reinterpret_cast<char*>(const_cast<unsigned char*>(input.data()));
    			strm.avail_in = input.size();
				auto result = List<unsigned char>{};
				auto outbuffer = std::array<unsigned char, CHUNK>{};
				do {
					strm.next_out = reinterpret_cast<char*>(outbuffer.data());
					strm.avail_out = outbuffer.size();
					bzerror = BZ2_bzCompress(&strm, BZ_FINISH);
					if (bzerror < 0) {
						BZ2_bzCompressEnd(&strm);
						throw Exception(fmt::format("{}", Language::get("bzip2.compress.failed")), std::source_location::current(), "compress");
					}
					result.insert(result.end(), outbuffer.begin(), outbuffer.begin() + (outbuffer.size() - strm.avail_out));
				} while (bzerror != BZ_STREAM_END);
				BZ2_bzCompressEnd(&strm);
				return result;
			}

			/**
			 * input: stream
			 * return: result after uncompress
			*/
			inline static auto uncompress(
				const List<unsigned char> & input
			) -> List<unsigned char> 
			{
				auto bzerror = int{};
				auto strm = bz_stream{
					.bzalloc = nullptr,
					.bzfree = nullptr,
					.opaque = nullptr,
				};
				BZ2_bzDecompressInit(&strm, 0, 0);
				strm.next_in = reinterpret_cast<char*>(const_cast<unsigned char*>(input.data()));
    			strm.avail_in = static_cast<unsigned int>(input.size());
				auto result = List<unsigned char>{};
				auto outbuffer = std::array<unsigned char, CHUNK>{};
				do {
					strm.next_out = reinterpret_cast<char*>(outbuffer.data());
					strm.avail_out = outbuffer.size();
					bzerror = BZ2_bzDecompress(&strm);
					if (bzerror < 0) {
						BZ2_bzDecompressEnd(&strm);
						throw Exception(fmt::format("{}", Language::get("bzip2.uncompress.failed")), std::source_location::current(), "uncompress");
					}
					result.insert(result.end(), outbuffer.begin(), outbuffer.begin() + (outbuffer.size() - strm.avail_out));
				} while (bzerror != BZ_STREAM_END);
				BZ2_bzDecompressEnd(&strm);
				return result;
			}
		};
}