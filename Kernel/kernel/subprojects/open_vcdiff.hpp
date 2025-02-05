#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif


#include "subprojects/open_vcdiff/google/vcdecoder.h"
#include "subprojects/open_vcdiff/google/vcencoder.h"
#include "subprojects/open_vcdiff/google/output_string.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::open_vcdiff {

	using ::open_vcdiff::VCDiffStreamingDecoder;

	using ::open_vcdiff::HashedDictionary;

	using ::open_vcdiff::VCDiffStreamingEncoder;

	using ::open_vcdiff::VCDiffFormatExtensionFlagValues;
	
}