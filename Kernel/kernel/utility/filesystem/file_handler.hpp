#pragma once

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

        explicit FileHandler(
            const Pointer<File>& value
        ) : value(value) {}

        ~FileHandler(
        ) {
            if (thiz.value != nullptr) {
                std::fclose(value);
                thiz.value = nullptr;
            }
        }

        FileHandler (
        ) = delete;

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

        template <typename T> requires (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>) && requires (T t) {
            { t.size() } -> std::convertible_to<usize>;
            { t.begin() } -> std::convertible_to<u8*>;
        }
        auto read (
            T& data
        ) -> void {
            auto count = std::fread(data.begin(), sizeof(u8), data.size(), thiz.value);
            assert_conditional(count == data.size(), fmt::format("{}: {}", "Missing bytes when reading the file"), "read");
        }

        template <typename T> requires (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>) && requires (T t) {
            { t.size() } -> std::convertible_to<usize>;
            { t.begin() } -> std::convertible_to<u8*>;
        }
        auto write (
            T& data
        ) -> void {
            auto count = std::fwrite(data.begin(), sizeof(u8), data.size(), thiz.value);
            assert_conditional(count == data.size(), fmt::format("{}: {}", "Missing bytes when writing the file"), "write");
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
        assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("cannot_read_file"), to_posix_style(source)), "open_file");
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
