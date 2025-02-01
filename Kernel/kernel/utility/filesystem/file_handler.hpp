#pragma once

#include "kernel/utility/container/string/basic_string.hpp"

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
            return thiz;
        }

        auto operator = (
            const FileHandler& other
        ) -> FileHandler & = delete;

        auto data(
        ) const -> Pointer<File> {
            return thiz.value;
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