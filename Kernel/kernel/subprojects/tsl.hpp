#pragma once

#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-macro-identifier"
#endif

#include "subprojects/tsl/include/tsl/ordered_hash.h"
#include "subprojects/tsl/include/tsl/ordered_map.h"
#include "subprojects/tsl/include/tsl/ordered_set.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::tsl {
	
}