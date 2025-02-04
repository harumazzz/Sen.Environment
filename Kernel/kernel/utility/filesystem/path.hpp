#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Path {

    inline auto size_file (
        const String& path
    ) -> usize {
        #if WINDOWS
        auto size = std::filesystem::file_size(path.wstring());
        #else
        auto size = std::filesystem::file_size(path.string());
        #endif
        return size;
    }

    enum class PathType : u8 {
        File,
        Directory,
        None,
    };

    

}
