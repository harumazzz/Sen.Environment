#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "subprojects/mscharconv/include/msstl/charconv.hpp"

#pragma clang diagnostic pop

namespace Sen::Kernel::Subprojects::mscharconv {

    using ::msstl::chars_format;

    using ::msstl::from_chars_result;

    using ::msstl::to_chars;

    using ::msstl::from_chars;

}