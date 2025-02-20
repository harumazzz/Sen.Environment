#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#endif

#include "subprojects/compile-time-regular-expressions/ctre.hpp"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::ctre {

	using ::ctre::search;

	using ::ctll::fixed_string;

}
