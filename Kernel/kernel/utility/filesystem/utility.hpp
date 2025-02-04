#pragma once

#include "kernel/utility/filesystem/path.hpp"
#include "kernel/utility/filesystem/file_handler.hpp"
#include "kernel/utility/filesystem/windows_file_handler.hpp"
#include "kernel/utility/filesystem/posix_file_handler.hpp"

namespace Sen::Kernel::FileSystem {

    using namespace Path;

    template <typename T>
    using List = CList<T>;

    template <typename T>
    using Array = CArray<T>;

    namespace Detail {

        template <typename T>
        concept is_buffer_container_v = std::is_base_of_v<BaseContainer<extract_container_t<T>>, T> && requires (T t) {
            { t.size() } -> std::convertible_to<usize>;
            { t.begin() } -> std::convertible_to<extract_container_t<T>*>;
        };

    }

    inline auto size_file (
        const String& path
    ) -> usize {
        #if WINDOWS
        auto size = std::filesystem::file_size(path.wstring());
        #else
        auto size = std::filesystem::file_size(path.view());
        #endif
        return size;
    }

    template <typename T> requires Detail::is_buffer_container_v<T>
    auto read_file (
        const String& source,
        T& data
    ) -> void {
        auto size = size_file(source);
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

    template <typename T> requires Detail::is_buffer_container_v<T>
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

    inline auto exist (
        const String& path
    ) -> bool {
        const auto type = get_path_type(path);
        return type == PathType::File || type == PathType::Directory;
    }

    inline auto is_directory (
        const String& path
    ) -> bool {
        return get_path_type(path) == PathType::Directory;
    }

    inline auto is_file (
        const String& path
    ) -> bool {
        return get_path_type(path) == PathType::File;
    }

    inline auto is_anything (
        const String& path
    ) -> bool {
        return get_path_type(path) == PathType::None;
    }

    inline auto create_directory (
        const String& target
    ) -> void {
        #if WINDOWS
        std::filesystem::create_directories(target.wstring());
        #else
        std::filesystem::create_directory(target.view());
        #endif
    }

    inline auto count_file (
        const String& source
    ) -> usize {
        return count<PathType::File>(source);
    }

    inline auto count_directory (
        const String& source
    ) -> usize {
        return count<PathType::File>(source);
    }

    inline auto count_anything (
        const String& source
    ) -> usize {
        return count<PathType::None>(source);
    }

    inline auto copy (
        const String& source,
        const String& destination
    ) -> void {
        assert_conditional(exist(source), fmt::format("{} does not exist, invalid copy operation call", source.view()), "copy");
        auto parent = dirname(destination);
        if (!is_directory(parent)) {
            create_directory(parent);
        }
        #if WINDOWS
        std::filesystem::copy(source.wstring(), destination.wstring(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        #else
        std::filesystem::copy(source.view(), destination.view(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        #endif
    }

    inline auto rename (
        const String& source,
        const String& destination
    ) -> void {
        #if WINDOWS
        std::filesystem::rename(source.wstring(), destination.wstring());
        #else
        std::filesystem::rename(source.view(), destination.view());
        #endif
    }

    inline auto remove (
       const String& source
    ) -> void {
        #if WINDOWS
        std::filesystem::remove_all(source.wstring());
        #else
        std::filesystem::remove_all(source.view());
        #endif
    }

    inline auto read_directory_file (
        const String& source,
        List<String>& destination
    ) -> void {
        return read_directory<PathType::File>(source, destination);
    }

    inline auto read_directory_directory (
        const String& source,
        List<String>& destination
    ) -> void {
        return read_directory<PathType::Directory>(source, destination);
    }

    inline auto read_directory_anything (
        const String& source,
        List<String>& destination
    ) -> void {
        return read_directory<PathType::None>(source, destination);
    }

    inline auto read_recursive_directory_file (
        const String& source,
        List<String>& destination
    ) -> void {
        return read_recursive_directory<PathType::File>(source, destination);
    }

    inline auto read_recursive_directory_directory (
        const String& source,
        List<String>& destination
    ) -> void {
        return read_recursive_directory<PathType::Directory>(source, destination);
    }

    inline auto read_recursive_directory_anything (
        const String& source,
        List<String>& destination
    ) -> void {
        return read_recursive_directory<PathType::None>(source, destination);
    }

    template <typename T> requires Detail::is_buffer_container_v<T>
    inline auto write_file_safe (
        const String& source,
        T& data
    ) -> void {
        auto parent = dirname(source);
        if (!is_directory(parent)) {
            create_directory(parent);
        }
        return write_file(source, data);
    }

}