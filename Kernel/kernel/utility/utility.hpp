#pragma once

#include "kernel/utility/assert.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

#include "kernel/utility/miscellaneous/clock/clock.hpp"

#include "kernel/utility/algorithm/math.hpp"
#include "kernel/utility/algorithm/string_helper.hpp"
#include "kernel/utility/algorithm/shared.hpp"
#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/algorithm/image/max_rects/max_rects_packer.hpp"
#include "kernel/utility/algorithm/utility.hpp"
#include "kernel/utility/algorithm/image/transformation/crop.hpp"
#include "kernel/utility/algorithm/image/transformation/resize.hpp"
#include "kernel/utility/algorithm/image/transformation/merge.hpp"
#include "kernel/utility/algorithm/image/transformation/rotate.hpp"
#include "kernel/utility/algorithm/image/transformation/scale.hpp"

#include "kernel/utility/container/array/array.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/common.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/container/list_view/list_view.hpp"
#include "kernel/utility/container/list_view/byte_view.hpp"
#include "kernel/utility/container/array_view/array_view.hpp"
#include "kernel/utility/container/array_view/byte_view.hpp"
#include "kernel/utility/container/set/common.hpp"
#include "kernel/utility/container/optional/optional.hpp"
#include "kernel/utility/container/variant/common.hpp"
#include "kernel/utility/stream/read_file_stream.hpp"
#include "kernel/utility/stream/write_file_stream.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"
#include "kernel/utility/encoding/image/read.hpp"
#include "kernel/utility/encoding/image/write.hpp"

#include "kernel/utility/container/list/common.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/container/list/byte_list.hpp"

#include "kernel/utility/container/map/common.hpp"

#include "kernel/utility/filesystem/utility.hpp"

#include "kernel/utility/container/string/compile_string.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/string/string_parser.hpp"

#include "kernel/utility/miscellaneous/system/system.hpp"
#include "kernel/utility/encoding/apng/writer.hpp"
#include "kernel/utility/encoding/xml/read.hpp"
#include "kernel/utility/encoding/xml/write.hpp"

#include "kernel/utility/trait/trait.hpp"

#include "kernel/utility/encryption/md5/hash.hpp"
#include "kernel/utility/encryption/crc/crc.hpp"
#include "kernel/utility/encryption/base64/encode.hpp"
#include "kernel/utility/encryption/base64/decode.hpp"
#include "kernel/utility/encryption/fnv/hash.hpp"
#include "kernel/utility/encryption/rijndael/encrypt.hpp"
#include "kernel/utility/encryption/rijndael/decrypt.hpp"
#include "kernel/utility/compression/zlib/compress.hpp"
#include "kernel/utility/compression/zlib/uncompress.hpp"
#include "kernel/utility/compression/zip/compress.hpp"
#include "kernel/utility/compression/zip/uncompress.hpp"
#include "kernel/utility/compression/lzma/compress.hpp"
#include "kernel/utility/compression/lzma/uncompress.hpp"
#include "kernel/utility/compression/bzip2/compress.hpp"
#include "kernel/utility/compression/bzip2/uncompress.hpp"
#include "kernel/utility/diff/vcdiff/encode.hpp"
#include "kernel/utility/diff/vcdiff/decode.hpp"

#include "kernel/utility/javascript/runtime.hpp"
#include "kernel/utility/javascript/context.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/trait.hpp"

namespace Sen::Kernel {

}