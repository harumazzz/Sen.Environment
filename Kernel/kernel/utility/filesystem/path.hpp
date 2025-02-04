#pragma once

#include "kernel/utility/algorithm/string_helper.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Path {

    template <typename T>
    using List = CList<T>;

    template <typename T>
    using Array = CArray<T>;

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

    enum class PathType : u8 {
        File,
        Directory,
        None,
    };

    inline auto get_path_type (
        const std::filesystem::file_type& type
    ) -> PathType {
        if (type == std::filesystem::file_type::directory) {
            return PathType::Directory;
        }
        if (type == std::filesystem::file_type::regular) {
            return PathType::File;
        }
        return PathType::None;
    }

    inline auto get_path_type (
        const String& path
    ) -> PathType {
        #if WINDOWS
        auto type = std::filesystem::status(path.wstring()).type();
        #else
        auto type = std::filesystem::status(path.view()).type();
        #endif
        return get_path_type(type);
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

    inline auto exist (
        const String& path
    ) -> bool {
        auto type = get_path_type(path);
        return type == PathType::File || type == PathType::Directory;
    }

    template <auto filter> requires std::is_same_v<type_of<filter>, PathType>
    inline auto count_recursive(
        const String& source
    ) -> usize {
        auto result = 0_size;
        #if WINDOWS
        auto directory = std::filesystem::directory_iterator{source.wstring()};
        #else
        auto directory = std::filesystem::directory_iterator{source.view()};
        #endif
        for (auto& entry : directory) {
            const auto type = get_path_type(entry.status().type());
            if constexpr (filter == PathType::File) {
                if (type == PathType::File) {
                    ++result;
                }
            }
            if constexpr (filter == PathType::Directory) {
                if (type == PathType::Directory) {
                    ++result;
                }
            }
            if constexpr (filter == PathType::None) {
                if (type == PathType::None) {
                    ++result;
                }
            }
        }
        return result;
    }

    inline auto count_recursive_file (
        const String& source
    ) -> usize {
        return count_recursive<PathType::File>(source);
    }

    inline auto count_recursive_directory (
        const String& source
    ) -> usize {
        return count_recursive<PathType::File>(source);
    }

    inline auto count_recursive_anything (
        const String& source
    ) -> usize {
        return count_recursive<PathType::None>(source);
    }

    template <auto filter> requires std::is_same_v<type_of<filter>, PathType>
    inline auto count(
        const String& source
    ) -> usize {
        auto result = 0_size;
        #if WINDOWS
        auto directory = std::filesystem::directory_iterator{source.wstring()};
        #else
        auto directory = std::filesystem::directory_iterator{source.view()};
        #endif
        for (auto& entry : directory) {
            const auto type = get_path_type(entry.status().type());
            if constexpr (filter == PathType::File) {
                if (type == PathType::File) {
                    ++result;
                }
            }
            if constexpr (filter == PathType::Directory) {
                if (type == PathType::Directory) {
                    ++result;
                }
            }
            if constexpr (filter == PathType::None) {
                if (type == PathType::None) {
                    ++result;
                }
            }
        }
        return result;
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

    template <auto filter> requires std::is_same_v<type_of<filter>, PathType>
    inline auto read_directory (
        const String& source,
        List<String>& destination
    ) -> void {
        destination.allocate(count<filter>(source));
        #if WINDOWS
        auto directory = std::filesystem::directory_iterator{source.wstring()};
        #else
        auto directory = std::filesystem::directory_iterator{source.view()};
        #endif
        for (auto& entry : directory) {
            const auto type = get_path_type(entry.status().type());
            auto file = StringHelper::make_string(entry.path().generic_u8string());
            if constexpr (filter == PathType::File) {
                if (type == PathType::File) {
                    destination.append(file);
                }
            }
            if constexpr (filter == PathType::Directory) {
                if (type == PathType::Directory) {
                    destination.append(file);
                }
            }
            if constexpr (filter == PathType::None) {
                if (type == PathType::None) {
                    destination.append(file);
                }
            }
        }
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

    template <auto filter> requires std::is_same_v<type_of<filter>, PathType>
    inline auto read_recursive_directory (
        const String& source,
        List<String>& destination
    ) -> void {
        destination.allocate(count_recursive<filter>(source));
        #if WINDOWS
        auto directory = std::filesystem::directory_iterator{source.wstring()};
        #else
        auto directory = std::filesystem::directory_iterator{source.view()};
        #endif
        for (auto& entry : directory) {
            const auto type = get_path_type(entry.status().type());
            auto file = StringHelper::make_string(entry.path().generic_u8string());
            if constexpr (filter == PathType::File) {
                if (type == PathType::File) {
                    destination.append(file);
                }
            }
            if constexpr (filter == PathType::Directory) {
                if (type == PathType::Directory) {
                    destination.append(file);
                }
            }
            if constexpr (filter == PathType::None) {
                if (type == PathType::None) {
                    destination.append(file);
                }
            }
        }
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

    template <typename T> requires std::is_base_of_v<BaseContainer<String>, T>
    inline auto join (
        const T& source
    ) -> String {
        auto result = std::filesystem::path{};
        for (auto& arg : source) {
            #if WINDOWS
            result /= arg.wstring();
            #else
            result /= arg.view();
            #endif
        }
        return StringHelper::make_string(result.generic_u8string());
    }

    inline auto dirname (
        const String &source
    ) -> String
    {
        #if WINDOWS
        return StringHelper::make_string(std::filesystem::path{source.wstring()}.parent_path().generic_u8string());
        #else
        return StringHelper::make_string(std::filesystem::path{source.view()}.parent_path().generic_u8string());
        #endif
    }

    inline auto extname (
        const String &source
    ) -> String
    {
        #if WINDOWS
        return StringHelper::make_string(std::filesystem::path{source.wstring()}.extension().generic_u8string());
        #else
        return StringHelper::make_string(std::filesystem::path{source.view()}.extension().generic_u8string());
        #endif
    }

    inline auto basename (
        const String &source
    ) -> String
    {
        #if WINDOWS
        return StringHelper::make_string(std::filesystem::path{source.wstring()}.filename().generic_u8string());
        #else
        return StringHelper::make_string(std::filesystem::path{source.view()}.filename().generic_u8string());
        #endif
    }

    inline auto base_without_extension (
        const String &source
    ) -> String
    {
        #if WINDOWS
        return StringHelper::make_string(std::filesystem::path{source.wstring()}.stem().generic_u8string());
        #else
        return StringHelper::make_string(std::filesystem::path{source.view()}.stem().generic_u8string());
        #endif
    }

    inline auto except_extension (
        const String &source
    ) -> String
    {
        return StringHelper::make_string(fmt::format("{}/{}", dirname(source).view(), base_without_extension(source).view()));
    }

    inline auto normalize (
        const String &source
    ) -> String
    {
        #if WINDOWS
        return StringHelper::make_string(std::filesystem::path{source.wstring()}.lexically_normal().generic_u8string());
        #else
        return StringHelper::make_string(std::filesystem::path{source.view()}.lexically_normal().generic_u8string());
        #endif
    }

    inline auto resolve (
        const String &source
    ) -> String
    {
        #if WINDOWS
        return StringHelper::make_string(std::filesystem::absolute(source.wstring()).generic_u8string());
        #else
        return StringHelper::make_string(std::filesystem::absolute(source.view()).generic_u8string());
        #endif
    }

}
