#pragma once

#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/byte_list.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/container/string/common.hpp"
#include "kernel/utility/container/string_view/string_view.hpp"
#include "kernel/utility/filesystem/path.hpp"

namespace Sen::Kernel::FileSystem {

    #if WINDOWS

        struct WindowsFileWriter {

        protected:

            HANDLE handle{INVALID_HANDLE_VALUE};

        public:

            explicit WindowsFileWriter(const StringView& path) {
                thiz.handle = CreateFileW(
                    path.wstring().data(),
                    GENERIC_WRITE,
                    0,
                    nullptr,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr
                );
                assert_not_null(thiz.handle != INVALID_HANDLE_VALUE, fmt::format("{}: {}", Language::get("write_file_error"), path.view()), "WindowsFileWriter");
            }

            ~WindowsFileWriter(
            ) {
                if (thiz.handle != INVALID_HANDLE_VALUE) {
                    CloseHandle(thiz.handle);
                    thiz.handle = INVALID_HANDLE_VALUE;
                }
            }

            WindowsFileWriter(
                const WindowsFileWriter& other
            ) = delete;

            auto operator=(
                const WindowsFileWriter& other
            ) -> WindowsFileWriter& = delete;

            WindowsFileWriter(WindowsFileWriter&& other) noexcept : handle{other.handle} {
                other.handle = INVALID_HANDLE_VALUE;
            }

            auto operator=(
                WindowsFileWriter&& other
            ) noexcept -> WindowsFileWriter& {
                if (this != &other) {
                    if (thiz.handle != INVALID_HANDLE_VALUE) {
                        CloseHandle(thiz.handle);
                    }
                    thiz.handle = other.handle;
                    other.handle = INVALID_HANDLE_VALUE;
                }
                return thiz;
            }

        protected:

        public:

            template <typename T> requires std::is_base_of_v<BaseContainer<extract_container_t<T>>, T> && requires (T t) {
                { t.size() } -> std::convertible_to<usize>;
                { t.begin() } -> std::convertible_to<extract_container_t<T>*>;
            }
            auto write (
                const T& data
            ) -> void {
                auto byte_written = DWORD{0};
                auto result = WriteFile(
                    thiz.handle,
                    data.begin(),
                    sizeof(extract_container_t<T>) * data.size(),
                    &byte_written,
                    nullptr
                );
                assert_eof(SUCCEEDED(result), "Write file operation has not been completed", "write");
                assert_eof(byte_written == sizeof(extract_container_t<T>) * data.size(), fmt::format("Missing bytes when write file, expected: {} but got: {}", sizeof(u8) * data.size(), byte_written), "write");
            }


            template <typename... Args> requires (is_numeric_v<Args> && ...)
            auto write (
                Args&&... args
            ) -> void {
                auto buffer = Uint8Array{sizeof...(args)};
                {
                    auto offset = 0_size;
                    (forward_bytes(std::forward<Args>(args), buffer, offset), ...);
                }
                thiz.write(buffer);
            }

            auto data (
            ) -> HANDLE& {
                return thiz.handle;
            }
        };

        struct WindowsFileReader {

        protected:

            HANDLE handle;

        public:

            explicit WindowsFileReader(const StringView& path) {
                thiz.handle = CreateFileW(
                    path.wstring().data(),
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    nullptr,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr
                );
                assert_eof(thiz.handle != INVALID_HANDLE_VALUE, fmt::format("{}: {}", Language::get("cannot_read_file"), path.view()), "WindowsFileReader");
            }

            ~WindowsFileReader(
            ) {
                if (thiz.handle != INVALID_HANDLE_VALUE) {
                    CloseHandle(thiz.handle);
                    thiz.handle = INVALID_HANDLE_VALUE;
                }
            }

            WindowsFileReader(
                const WindowsFileReader&
            ) = delete;

            auto operator=(
                const WindowsFileReader& other
            ) -> WindowsFileReader& = delete;

            WindowsFileReader(
                WindowsFileReader&& other
            ) noexcept : handle{other.handle} {
                other.handle = INVALID_HANDLE_VALUE;
            }

            auto operator=(
                WindowsFileReader&& other
            ) noexcept -> WindowsFileReader& {
                if (this != &other) {
                    if (thiz.handle != INVALID_HANDLE_VALUE) {
                        CloseHandle(thiz.handle);
                    }
                    thiz.handle = other.handle;
                    other.handle = INVALID_HANDLE_VALUE;
                }
                return thiz;
            }

            template <typename T> requires (std::is_base_of_v<BaseContainer<extract_container_t<T>>, T> && requires (T t) {
                { t.size() } -> std::convertible_to<usize>;
            }) || std::is_same_v<T, std::string>
            auto read (
                T& data
            ) -> void {
                auto bytes_read = DWORD{0};
                auto result = ReadFile(
                    thiz.handle,
                    data.data(),
                    sizeof(extract_container_t<T>) * data.size(),
                    &bytes_read,
                    nullptr
                );
                assert_eof(SUCCEEDED(result), "Read file operation has not been completed", "read");
                assert_eof(bytes_read == sizeof(extract_container_t<T>) * data.size(), fmt::format("Missing bytes when read file, expected: {} but got: {}", sizeof(u8) * data.size(), bytes_read), "read");
            }

            auto data (
            ) -> HANDLE& {
                return thiz.handle;
            }
        };

        #endif

}
