#pragma once

#include "kernel/utility/assert.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array/array.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel::Encoding::APNG {

	struct Common {
		
        inline static constexpr auto idat_uint32 = 1229209940u;

        inline static constexpr auto file_signature = 9894494448401390090ull;

        inline static constexpr auto image_trailer_end = 5279712195050102914ull;

	};

	struct ImageHeader {

		std::uint32_t width;

		std::uint32_t height;

		std::uint32_t x;

		std::uint32_t y;

		std::uint8_t depth;

		std::uint8_t ctype;

		std::uint8_t compress;

		std::uint8_t filter;

		std::uint8_t interlace;

		constexpr explicit ImageHeader(

		) = default;

		explicit ImageHeader(
			std::uint32_t width,
			std::uint32_t height,
			std::uint32_t x,
			std::uint32_t y,
			std::uint8_t depth,
			std::uint8_t ctype,
			std::uint8_t compress,
			std::uint8_t filter,
			std::uint8_t interlace
		) : width{width}, height{height}, x{x}, y{y},
										depth{depth}, ctype{ctype}, compress{compress}, filter{filter}, interlace{interlace}
		{
		}

		constexpr ~ImageHeader(

		) = default;
	};

	struct ImageData {
		CList<CList<std::uint32_t>> pixel_list;
		ImageHeader meta_data;
	};

}