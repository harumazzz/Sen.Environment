#pragma once

#include "kernel/utility/filesystem/path.hpp"
#include "kernel/utility/filesystem/file_handler.hpp"
#include "kernel/utility/filesystem/windows_file_handler.hpp"

namespace Sen::Kernel::FileSystem {

    template <typename T> requires (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>) && requires (T t) {
        { t.size() } -> std::convertible_to<usize>;
        { t.begin() } -> std::convertible_to<u8*>;
    }
    auto read_file (
        const String& source,
        T& data
    ) -> void {
        auto size = Path::size_file(source);
        data.allocate(size);
        #if WINDOWS
        auto file = WindowsFileReader{source};
        file.read(data);
        #else
        auto file = open_read(source);
        file.read(data);
        #endif
    }

    template <typename T> requires (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>) && requires (T t) {
        { t.size() } -> std::convertible_to<usize>;
        { t.begin() } -> std::convertible_to<u8*>;
    }
    auto write_file (
       const String& source,
       T& data
    ) -> void {
        #if WINDOWS
        auto file = WindowsFileWriter{source};
        file.write(data);
        #else
        auto file = open_write(source);
        file.write(data);
        #endif
    }

}