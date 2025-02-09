#pragma once

#include "path.hpp"
#include "kernel/utility/container/list/byte_list.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel::FileSystem {

    struct FileHandler;

    inline auto open (
        const String& source,
        const String& mode
    ) -> FileHandler;

    inline auto open_read (
        const String& source
    ) -> FileHandler;

    inline auto open_write (
        const String& source
    ) -> FileHandler;

    struct FileHandler {

    protected:

        using File = std::FILE;

        template <typename T>
        using Pointer = T*;

        Pointer<File> value{nullptr};

    public:

        constexpr explicit FileHandler(
            const Pointer<File>& value
        ) : value(value) {

        }

        ~FileHandler(
        ) {
            if (thiz.value != nullptr) {
                std::fclose(value);
                thiz.value = nullptr;
            }
        }

        constexpr explicit FileHandler (
        ) : value{nullptr} {

        }

        FileHandler (
            FileHandler const & other
        ) = delete;

        FileHandler(FileHandler&& other) noexcept : value{other.value} {
            other.value = nullptr;
        }

        auto operator=(
            FileHandler&& other
        ) noexcept -> FileHandler& {
            if (this != &other) {
                if (thiz.value != nullptr) {
                    std::fclose(thiz.value);
                }
                thiz.value = other.value;
                other.value = nullptr;
            }
            return *this;
        }

        auto operator = (
            const FileHandler& other
        ) -> FileHandler & = delete;

        auto data(
        ) const -> Pointer<File> {
            return thiz.value;
        }

        template <typename T> requires (std::is_base_of_v<BaseContainer<extract_container_t<T>>, T>) && requires (T t) {
            { t.size() } -> std::convertible_to<usize>;
            { t.begin() } -> std::convertible_to<extract_container_t<T>*>;
        }
        auto read (
            T& data
        ) -> void {
            auto count = std::fread(data.begin(), sizeof(extract_container_t<T>) * data.size(), 1, thiz.value);
            assert_conditional(count == 1, fmt::format("{}", "Missing bytes when reading the file"), "read");
        }

        auto read (
            u8* data,
            const usize& size
        ) const -> void {
            auto count = std::fread(data, size, 1, thiz.value);
            assert_conditional(count == 1, fmt::format("{}", "Missing bytes when reading the file"), "read");
        }

        template <typename T> requires (std::is_base_of_v<BaseContainer<extract_container_t<T>>, T>) && requires (T t) {
            { t.size() } -> std::convertible_to<usize>;
            { t.begin() } -> std::convertible_to<extract_container_t<T>*>;
        }
        auto write (
            T& data
        ) -> void {
            auto count = std::fwrite(data.begin(), sizeof(extract_container_t<T>) * data.size(), 1, thiz.value);
            assert_conditional(count == 1, fmt::format("{}", "Missing bytes when writing the file"), "write");
        }

        auto write (
            const uint8_t* data,
            const usize& size
        ) const -> void {
            auto count = std::fwrite(data, size, 1, thiz.value);
            assert_conditional(count == 1, fmt::format("{}", "Missing bytes when writing the file"), "write");
        }

        template <typename... Args> requires (is_numeric_v<Args> && ...)
        auto write (
            Args&&... args
        ) -> void {
            auto buffer = Uint8Array{total_sizeof<Args>()...};
            {
                auto offset = 0_size;
                (forward_bytes (std::forward<Args>(args), buffer, offset), ...);
            }
            thiz.write(buffer);
        }

        auto move_to_end (

        ) const -> void {
            #ifdef WINDOWS
            _fseeki64(thiz.value, 0, SEEK_END);
            #else
            std::fseeko(value, 0, SEEK_END);
            #endif
        }

        auto move_to_begin (

        ) const -> void {
            #ifdef WINDOWS
            _fseeki64(thiz.value, 0, SEEK_SET);
            #else
            std::fseeko(value, 0, SEEK_SET);
            #endif
        }

        auto tell (

        ) const -> usize {
            #ifdef WINDOWS
            return static_cast<usize>(_ftelli64(thiz.value));
            #else
            return static_cast<usize>(std::ftello(value));
            #endif
        }

        auto size (

        ) const -> usize {
            thiz.move_to_end();
            auto value = thiz.tell();
            thiz.move_to_begin();
            return value;
        }

    };

    inline auto open (
        const String& source,
        const String& mode
    ) -> FileHandler {
        #if WINDOWS
        auto file = _wfopen(source.wstring().data(), mode.wstring().data());
        #else
        auto file = std::fopen(source.cbegin(), mode.cbegin());
        #endif
        #if WINDOWS
        assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("cannot_read_file"), Path::to_posix(source).view()), "open_file");
        #else
        assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("cannot_read_file"), source.view()), "open_file");
        #endif
        return FileHandler{file};
    }

    inline auto open_read (
        const String& source
    ) -> FileHandler {
        auto file = open(source, String{"rb"});
        return file;
    }

    inline auto open_write (
        const String& source
    ) -> FileHandler {
        auto file = open(source, String{"wb"});
        return file;
    }


}
