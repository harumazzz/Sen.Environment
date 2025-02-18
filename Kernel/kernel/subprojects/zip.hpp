#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#endif

#include "subprojects/zip/zip.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::zip {

	using ::zip_stream_open;

	using ::zip_entry_openbyindex;

	using ::zip_entry_name;

	using ::zip_entry_size;

	using ::zip_entry_close;

	using ::zip_close;

	using ::zip_entries_total;

	using ::zip_entry_extract;

	using ::zip_entry_open;

	using ::zip_entry_write;

	using ::zip_stream_copy;

	using ::zip_t;
	

}
