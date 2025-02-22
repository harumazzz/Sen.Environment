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

    template <auto type, auto windows_bits>
    concept is_valid_uncompress = std::is_same_v<type_of<type>, Type> && is_numeric_v<type_of<windows_bits>> && is_between_v<windows_bits, 8_size, Subprojects::zlib::$MAX_WBITS>;

    template <auto type, auto level, auto windows_bits, auto memory_level, auto strategy>
    concept is_valid_compress = std::is_same_v<type_of<type>, Type> && is_numeric_v<type_of<level>> && is_numeric_v<type_of<windows_bits>> && is_numeric_v<type_of<memory_level>> &&
            is_between_v<level, 0_size, Subprojects::zlib::$Z_BEST_COMPRESSION> && is_between_v<windows_bits, 8_size, Subprojects::zlib::$MAX_WBITS> && is_between_v<memory_level, 1_size, Subprojects::zlib::$MAX_MEM_LEVEL>
    && (!(windows_bits == 8_size && type == Type::zlib)) && std::is_same_v<type_of<strategy>, Strategy>;

    template <typename T>
    concept is_buffer_container = (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>);

    struct Common {

    };

}
