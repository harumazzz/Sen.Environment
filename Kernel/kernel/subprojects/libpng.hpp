#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif

#include "subprojects/libpng/png.h"
#include "subprojects/libpng/pngstruct.h"
#include "subprojects/libpng/pnginfo.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::libpng {

	using ::png_structp;

	using ::png_const_charp;

	using ::png_infop;

	using ::size_t;

	using ::png_bytep;

	using ::png_color_16p;

	using ::png_uint_32;

	using ::png_const_bytep;

	using ::png_create_read_struct;

	using ::png_create_write_struct;

	using ::png_destroy_read_struct;

	using ::png_destroy_write_struct;

	using ::png_get_io_ptr;

	using ::png_get_sRGB;

	using ::png_set_gamma;

	using ::png_set_error_fn;

	using ::png_set_read_fn;

	using ::png_create_info_struct;

	using ::png_read_info;

	using ::png_set_expand_gray_1_2_4_to_8;

	using ::png_set_add_alpha;

	using ::png_set_palette_to_rgb;

	using ::png_set_scale_16;

	using ::png_set_gray_to_rgb;

	using ::png_get_tRNS;

	using ::png_set_tRNS_to_alpha;

	using ::png_get_bKGD;

	using ::png_set_background;

	using ::png_get_gAMA;

	using ::png_read_row;

	using ::png_read_end;

	using ::png_set_write_fn;

	using ::png_set_IHDR;

	using ::png_write_info;

	using ::png_write_row;

	using ::png_write_end;

	inline static auto constexpr $PNG_LIBPNG_VER_STRING = PNG_LIBPNG_VER_STRING;

	inline static auto constexpr $PNG_COLOR_TYPE_PALETTE = PNG_COLOR_TYPE_PALETTE;

	inline static auto constexpr $PNG_FILLER_AFTER = PNG_FILLER_AFTER;
	
	inline static auto constexpr $PNG_COLOR_TYPE_GRAY = PNG_COLOR_TYPE_GRAY;
	
	inline static auto constexpr $PNG_COLOR_TYPE_GRAY_ALPHA = PNG_COLOR_TYPE_GRAY_ALPHA;
	
	inline static auto constexpr $PNG_COLOR_TYPE_RGB = PNG_COLOR_TYPE_RGB;
	
	inline static auto constexpr $PNG_COLOR_TYPE_RGB_ALPHA = PNG_COLOR_TYPE_RGB_ALPHA;
	
	inline static auto constexpr $PNG_BACKGROUND_GAMMA_FILE = PNG_BACKGROUND_GAMMA_FILE;

	inline static auto constexpr $PNG_DEFAULT_sRGB = PNG_DEFAULT_sRGB;

	inline static auto constexpr $PNG_INTERLACE_NONE = PNG_INTERLACE_NONE;

	inline static auto constexpr $PNG_COMPRESSION_TYPE_DEFAULT = PNG_COMPRESSION_TYPE_DEFAULT;

	inline static auto constexpr $PNG_FILTER_TYPE_DEFAULT = PNG_FILTER_TYPE_DEFAULT;

}