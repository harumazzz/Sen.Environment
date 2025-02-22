#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#endif

#include "subprojects/zlib/zlib.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::zlib {

	inline static constexpr auto $Z_BEST_COMPRESSION = Z_BEST_COMPRESSION;

	inline static constexpr auto $MAX_WBITS = MAX_WBITS;

	inline static constexpr auto $MAX_MEM_LEVEL = MAX_MEM_LEVEL;

	inline static constexpr auto $Z_DEFLATED = Z_DEFLATED;

	inline static constexpr auto $Z_NO_FLUSH = Z_NO_FLUSH;

	inline static constexpr auto $Z_OK = Z_OK;

	inline static constexpr auto $ZLIB_VERSION = ZLIB_VERSION;

	inline static constexpr auto $Z_STREAM_END = Z_STREAM_END;

	inline static constexpr auto $Z_STREAM_ERROR = Z_STREAM_ERROR;

	inline static constexpr auto $Z_FINISH = Z_FINISH;

	using ::z_stream;

	using ::Bytef;

	using ::deflateInit2_;

	using ::deflate;

	using ::deflateEnd;

	using ::uInt;

	using ::inflateInit2_;

	using ::inflate;

	using ::inflateEnd;

	using ::compressBound;
	

}
