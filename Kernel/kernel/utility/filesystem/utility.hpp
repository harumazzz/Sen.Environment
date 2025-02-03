#pragma once

#include "kernel/utility/filesystem/path.hpp"
#include "kernel/utility/filesystem/file_handler.hpp"
#include "kernel/utility/filesystem/windows_file_handler.hpp"
#include "kernel/utility/filesystem/posix_file_handler.hpp"

namespace Sen::Kernel::FileSystem {

    template <typename T> requires std::is_base_of_v<BaseContainer<extract_container_t<T>>, T> && requires (T t) {
        { t.size() } -> std::convertible_to<usize>;
        { t.begin() } -> std::convertible_to<extract_container_t<T>*>;
    }
    auto read_file (
        const String& source,
        T& data
    ) -> void {
        auto size = Path::size_file(source);
        data.allocate(size);
        if constexpr (std::is_same_v<T, CList<extract_container_t<T>>>) {
            data.size(size);
        }
        #if WINDOWS
        auto file = WindowsFileReader{source};
        file.read(data);
        #else
        auto file = PosixFileReader{source};
        file.read(data);
        #endif
    }

    template <typename T> requires std::is_base_of_v<BaseContainer<extract_container_t<T>>, T> && requires (T t) {
        { t.size() } -> std::convertible_to<usize>;
        { t.begin() } -> std::convertible_to<extract_container_t<T>*>;
    }
    auto write_file (
       const String& source,
       T& data
    ) -> void {
        #if WINDOWS
        auto file = WindowsFileWriter{source};
        file.write(data);
        #else
        auto file = PosixFileWriter{source};
        file.write(data);
        #endif
    }

    inline auto exists (
        const String& path
    ) -> bool {
        #if WINDOWS
        return std::filesystem::exists(path.wstring());
        #else
        return std::filesystem::exists(path.string());
        #endif
    }

    inline auto is_file (
        const String& path
    ) -> bool {
        #if WINDOWS
        return exists(path) && std::filesystem::is_regular_file(path.wstring());
        #else
        return exists(path) && std::filesystem::is_regular_file(path.string());
        #endif
    }

    inline auto is_directory (
        const String& path
    ) -> bool {
        #if WINDOWS
        return exists(path) && std::filesystem::is_directory(path.wstring());
        #else
        return exists(path) && std::filesystem::is_directory(path.string());
        #endif
    }

}