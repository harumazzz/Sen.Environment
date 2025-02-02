#pragma once


#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Diff {

	/**
	 * Encode & Decode Struct
	*/

	struct VCDiff {

		public:

			/**
			 * VCDiff Flag
			*/

			enum Flag {
				VCD_STANDARD_FORMAT = 0x00,
				VCD_FORMAT_INTERLEAVED = 0x01,
				VCD_FORMAT_CHECKSUM = 0x02,
				VCD_FORMAT_JSON = 0x04,
			};

			/**
			 * Constructor
			*/

			VCDiff(

			) = default;

			/**
			 * Destructor
			*/

			~VCDiff(

			) = default;

			/**
			 * Encode method
			 * @param before: before buffer
			 * @param before_size: buffer size
			 * @param after: after buffer
			 * @param after_size: buffer size
			 * @param flag: encoder flag
			 * @returns: encoded buffer
			*/
			template <typename T, auto flag> requires std::is_integral<T>::value
			inline static auto encode(
				const char* before,
				T before_size,
				const char* after,
				T after_size
			) -> List<unsigned char>
			{
				static_assert(flag == open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_FORMAT_CHECKSUM or flag == open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_FORMAT_INTERLEAVED or open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_FORMAT_JSON or open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_STANDARD_FORMAT, "flag must be one of vcdiff supported format");
				auto encoding = std::string{};
				auto encoder = std::make_unique<open_vcdiff::VCDiffEncoder>(before, before_size);
				encoder->SetFormatFlags(static_cast<open_vcdiff::VCDiffFormatExtensionFlags>(flag));
				if (!encoder->Encode(after, after_size, &encoding)) {
					throw Exception(fmt::format("{}", Language::get("vcdiff.encode.failed")), std::source_location::current(), "encode");
				}
				return List<unsigned char>(encoding.begin(), encoding.end());
			}

			/**
			 * Decode method
			 * @param before: before buffer
			 * @param before_size: buffer size
			 * @param patch: after buffer
			 * @param patch_size: buffer size
			 * @param flag: encoder flag
			 * @returns: decoded buffer
			*/

			template <typename T> requires std::is_integral<T>::value
			inline static auto decode(
				const char* before,
				T before_size,
				const char* patch,
				T patch_size
			) -> List<unsigned char>
			{
				auto decoded_data = std::string{};
				auto decoder = std::make_unique<open_vcdiff::VCDiffStreamingDecoder>();
				decoder->SetMaximumTargetFileSize(332873785_size); // 325MB
				decoder->SetMaximumTargetWindowSize(332873785_size); //  325MB
				decoder->StartDecoding(before, before_size);
				if (!decoder->DecodeChunk(patch, patch_size, &decoded_data)) {
					throw Exception(fmt::format("{}", Language::get("vcdiff.decode.failed")), std::source_location::current(), "decode");
				}
				return List<unsigned char>(decoded_data.data(), decoded_data.data() + decoded_data.size());
			}
	};
}