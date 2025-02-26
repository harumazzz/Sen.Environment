#pragma once

#include "kernel/utility/container/array_view/byte_view.hpp"
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

    inline auto exist (
        const StringView& path
    ) -> bool {
        const auto type = get_path_type(path);
        return type == PathType::File || type == PathType::Directory;
    }

    inline auto is_directory (
        const StringView& path
    ) -> bool {
        return get_path_type(path) == PathType::Directory;
    }
    inline auto is_file (
        const StringView& path
    ) -> bool {
        return get_path_type(path) == PathType::File;
    }

    inline auto is_anything (
        const StringView& path
    ) -> bool {
        return get_path_type(path) == PathType::None;
    }

    inline auto size_file (
        const StringView& path
    ) -> usize {
        assert_not_null(is_file(path), fmt::format("File {} does not exist", path.view()), "size_file");
        #if WINDOWS
        auto size = std::filesystem::file_size(path.wstring());
        #else
        auto size = std::filesystem::file_size(path.view());
        #endif
        return size;
    }

    template <typename T> requires Detail::is_buffer_container_v<T> || std::is_same_v<T, std::string>
    auto read_file (
        const StringView& source,
        T& data
    ) -> void {
        auto size = size_file(source);
        if constexpr (!std::is_same_v<T, std::string>) {
            data.allocate(size);
        }
        if constexpr (std::is_same_v<T, CList<extract_container_t<T>>> || std::is_same_v<T, std::string>) {
            data.resize(size);
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
       const StringView& source,
       const T& data
    ) -> void {
        #if WINDOWS
        auto file = WindowsFileWriter{source};
        file.write(data);
        #else
        auto file = PosixFileWriter{source};
        file.write(data);
        #endif
    }

	inline auto read_xml(const StringView& source, pugi::xml_document &document) -> void
    {
        auto buffer = Uint8Array{};
        FileSystem::read_file(source, buffer);
        assert_conditional(document.load_buffer(buffer.data(), buffer.size()), "Failed to read xml", "read_xml"); //TODO:
    }

    inline auto write_xml(const StringView& source, pugi::xml_document const &document, u32 const &flag = pugi::format_default) -> void {
        assert_conditional(document.save_file(source.begin(), "\t"_sv.data(), flag, pugi::encoding_auto), "Failed to write xml", "write_xml"); //TODO:
    }

    template <class T>
    concept is_valid_json = std::is_class_v<T> && !std::is_void_v<T> || std::is_same_v<T, jsoncons::pmr::json>;

    namespace Detail {

        template <auto pretty, auto order_key, typename T> requires is_valid_json<T> && std::is_same_v<type_of<pretty>, bool> && std::is_same_v<type_of<order_key>, bool>
        inline auto write_json(
            const StringView& source,
            const T& value
        ) -> void {
            auto os = std::string{};
            if constexpr (jsoncons::extension_traits::is_basic_json<T>::value) {
                if constexpr (pretty) {
                    jsoncons::encode_json_pretty(value, os);
                } else {
                    jsoncons::encode_json(value, os);
                }
            } else {
                if constexpr (pretty) {
                    if constexpr (order_key) {
                        jsoncons::encode_json_pretty<jsoncons::ojson>(value, os);
                    } else {
                        jsoncons::encode_json_pretty(value, os);
                    }
                } else {
                    if constexpr (order_key) {
                        jsoncons::encode_json<jsoncons::ojson>(value, os);
                    } else {
                        jsoncons::encode_json(value, os);
                    }
                }
            }
            const auto buffer = CharacterArrayView{os.data(), os.size()};
            write_file(source, buffer);
        }

    }

    template <typename T> requires is_valid_json<T>
    inline auto write_json(
        const StringView& source,
        const T& value
    ) -> void {
        return Detail::write_json<true, true, T>(source, value);
    }

    template <typename T> requires is_valid_json<T>
    auto read_json(const StringView& source, T& value) -> void {
        auto buffer = String{};
        FileSystem::read_file(source, buffer);
        value = jsoncons::decode_json<T>(buffer.view());
    }

    inline auto create_directory (
        const StringView& target
    ) -> void {
        #if WINDOWS
        std::filesystem::create_directories(target.wstring());
        #else
        std::filesystem::create_directory(target.view());
        #endif
    }

    inline auto make_directory(const StringView& target) -> void {
        if (!is_directory(target)) {
            create_directory(target);
        }
    }


    inline auto count_file (
        const StringView& source
    ) -> usize {
        return count<PathType::File>(source);
    }

    inline auto count_directory (
        const StringView& source
    ) -> usize {
        return count<PathType::File>(source);
    }

    inline auto count_anything (
        const StringView& source
    ) -> usize {
        return count<PathType::None>(source);
    }

    inline auto copy (
        const StringView& source,
        const StringView& destination
    ) -> void {
        assert_conditional(exist(source), fmt::format("{} does not exist, invalid copy operation call", source.view()), "copy");
        if (const auto parent = dirname(destination); !is_directory(parent)) {
            create_directory(parent);
        }
        #if WINDOWS
        std::filesystem::copy(source.wstring(), destination.wstring(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        #else
        std::filesystem::copy(source.view(), destination.view(), std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        #endif
    }

    inline auto rename (
        const StringView& source,
        const StringView& destination
    ) -> void {
        #if WINDOWS
        std::filesystem::rename(source.wstring(), destination.wstring());
        #else
        std::filesystem::rename(source.view(), destination.view());
        #endif
    }

    inline auto remove (
       const StringView& source
    ) -> void {
        #if WINDOWS
        std::filesystem::remove_all(source.wstring());
        #else
        std::filesystem::remove_all(source.view());
        #endif
    }

    inline auto read_directory_file (
        const StringView& source,
        List<String>& destination
    ) -> void {
        return read_directory<PathType::File>(source, destination);
    }

    inline auto read_directory_directory (
        const StringView& source,
        List<String>& destination
    ) -> void {
        return read_directory<PathType::Directory>(source, destination);
    }

    inline auto read_directory_anything (
        const StringView& source,
        List<String>& destination
    ) -> void {
        return read_directory<PathType::None>(source, destination);
    }

    inline auto read_recursive_directory_file (
        const StringView& source,
        List<String>& destination
    ) -> void {
        return read_recursive_directory<PathType::File>(source, destination);
    }

    inline auto read_recursive_directory_directory (
        const StringView& source,
        List<String>& destination
    ) -> void {
        return read_recursive_directory<PathType::Directory>(source, destination);
    }

    inline auto read_recursive_directory_anything (
        const StringView& source,
        List<String>& destination
    ) -> void {
        return read_recursive_directory<PathType::None>(source, destination);
    }

    template <typename T> requires Detail::is_buffer_container_v<T>
    inline auto write_file_safe (
        const StringView& source,
        T& data
    ) -> void {
        if (const auto parent = dirname(source); !is_directory(parent)) {
            create_directory(parent);
        }
        return write_file(source, data);
    }

    inline auto read_utf16 (
        const StringView& source
    ) -> String {
        auto file = open_read(source);
        const auto file_size = file.size();
        auto buffer = CharacterArray{file_size};
        file.read(buffer);
        auto offset = 0_size;
        if (file_size >= 2 && buffer[0] == 0xFF && buffer[1] == 0xFE) {
            offset = 2;
        }
        auto converter = std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>>{};
        const auto content = converter.from_bytes(buffer.data() + offset, buffer.data() + file_size);
        auto value = utf16_to_utf8(content);
        return String{value.data(), value.size()};
    }

    inline auto read_utf8_bom (
        const StringView& source
    ) -> String {
        auto file = open_read(source);
        const auto file_size = file.size();
        auto buffer = CharacterArray{file_size};
        file.read(buffer);
        auto offset = 0_size;
        if (file_size >= 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF) {
            offset = 3;
        }
        return String{buffer.data() + offset, buffer.size() - offset};
    }

    template <auto is_utf16, auto write_bom> requires std::is_same_v<type_of<write_bom>, bool> && std::is_same_v<type_of<is_utf16>, bool>
    inline auto write_file_s (
        const StringView& source,
        const String& data
    ) -> void {
        auto file = open_write(source);
        if constexpr (is_utf16) {
            if constexpr (write_bom) {
                file.write(0xEF_u8, 0xFE_u8);
            }
            file.write(as_lvalue(data.warray()));
        }
        else {
            if constexpr (write_bom) {
                file.write(0xFF_u8, 0xBB_u8, 0xBF_u8);
            }
            file.write(data);
        }
    }


}