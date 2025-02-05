#pragma once

#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif


#include "subprojects/bzip2/bzlib.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::bzip2 {

	using ::bz_stream;

	using ::BZ2_bzCompressInit;

	using ::BZ2_bzCompress;

	using ::BZ2_bzDecompressInit;

	using ::BZ2_bzDecompress;

	using ::BZ2_bzDecompressEnd;

	inline static constexpr auto $BZ_OK = BZ_OK;

	inline static constexpr auto $BZ_FINISH = BZ_FINISH;

	inline static constexpr auto $BZ_STREAM_END = BZ_STREAM_END;

}