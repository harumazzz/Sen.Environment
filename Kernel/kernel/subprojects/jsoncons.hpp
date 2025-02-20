#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#endif

#include "subprojects/jsoncons/include/jsoncons/json.hpp"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::jsoncons {

    using ::jsoncons::json_stream_encoder;

    using ::jsoncons::json_stream_cursor;

    using ::jsoncons::staj_event_type;

    using ::jsoncons::json;

    using ::jsoncons::ojson;

    using ::jsoncons::json_type;

}

