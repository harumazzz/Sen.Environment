#pragma once

#include "kernel/definition/assert.hpp"
#include "kernel/definition/library.hpp"
#include "kernel/definition/macro.hpp"
// basic
#include "kernel/definition/basic/math.hpp"
#include "kernel/definition/basic/array.hpp"
#include "kernel/definition/basic/filesystem.hpp"
#include "kernel/definition/basic/pointer.hpp"
#include "kernel/definition/basic/string.hpp"
#include "kernel/definition/basic/path.hpp"
#include "kernel/definition/basic/converter.hpp"
#include "kernel/definition/basic/timer.hpp"
#include "kernel/definition/basic/image.hpp"
#include "kernel/definition/basic/json.hpp"
#include "kernel/definition/basic/json_writer.hpp"
#include "kernel/definition/basic/buffer.hpp"
#include "kernel/definition/basic/system.hpp"
#include "kernel/definition/basic/thread.hpp"
#include "kernel/definition/basic/apng_maker.hpp"
#include "kernel/definition/basic/max_rects_lite.hpp"
#include "kernel/definition/basic/utility.hpp"

#include "kernel/definition/encryption/md5.hpp"
#include "kernel/definition/encryption/sha224.hpp"
#include "kernel/definition/encryption/sha384.hpp"
#include "kernel/definition/encryption/sha512.hpp"
#include "kernel/definition/encryption/XOR.hpp"
#include "kernel/definition/encryption/crc.hpp"
#include "kernel/definition/encryption/sha256.hpp"
#include "kernel/definition/encryption/base64.hpp"
#include "kernel/definition/encryption/fnv.hpp"
#include "kernel/definition/encryption/Rijndael.hpp"
#include "kernel/definition/compression/zlib.hpp"
#include "kernel/definition/compression/zip.hpp"
#include "kernel/definition/compression/lzma.hpp"
#include "kernel/definition/compression/bzip2.hpp"
#include "kernel/definition/javascript/runtime.hpp"
#include "kernel/definition/diff/vcdiff.hpp"

namespace Sen::Kernel {

	template <typename U> requires std::is_class<U>::value 
	&& (std::is_same<U, nlohmann::ordered_json>::value || std::is_same<U, nlohmann::json>::value)
	inline auto is_defined (
		const U &json,
		std::string_view key,
		std::string_view error
	) -> void
	{
		if (!json.contains(key)) {
            throw Exception{std::string{error.data(), error.size()}, std::source_location::current(), "is_defined"};
        }
	}

}