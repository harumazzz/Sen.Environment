#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif

#include "subprojects/lzma/LzmaLib.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::lzma {

	using ::Byte;

	using ::LzmaCompress;

	using ::LzmaUncompress;

	inline constexpr auto $SZ_OK = SZ_OK;

	inline constexpr auto $LZMA_PROPS_SIZE = LZMA_PROPS_SIZE;

}
