#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif

#include "subprojects/PVRTCCompressor/PvrTcDecoder.h"
#include "subprojects/PVRTCCompressor/PvrTcEncoder.h"
#include "subprojects/PVRTCCompressor/RgbBitmap.h"
#include "subprojects/PVRTCCompressor/RgbaBitmap.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::PVRTCCompressor {

    using namespace ::Javelin;

}