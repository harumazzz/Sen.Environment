#pragma once

#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Compression::Zlib {

    enum class Strategy : u8 {
        default_mode,
        filtered,
        huffman_only,
        rle,
        fixed,
    };

    enum class Type : u8 {
        deflate,
        zlib,
        gzip,
    };


    struct Common {

    };

}
