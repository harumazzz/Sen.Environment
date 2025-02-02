#pragma once

#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/byte_list.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel::FileSystem {

    #if WINDOWS

        struct WindowsFileWriter {

        protected:

            HANDLE handle{INVALID_HANDLE_VALUE};

        public:

            explicit WindowsFileWriter(const String& path) {
                thiz.handle = CreateFileW(
                    path.wstring().data(),
                    GENERIC_WRITE,
                    0,
                    nullptr,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr
                );
                assert_conditional(thiz.handle != INVALID_HANDLE_VALUE, fmt::format("{}: {}", Language::get("write_file_error"), to_posix_style(path.view())), "WindowsFileWriter");
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

            template <typename T> requires (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>) && requires (T t) {
                { t.size() } -> std::convertible_to<usize>;
                { t.begin() } -> std::convertible_to<u8*>;
            }
            auto write (
                T& data
            ) -> void {
                auto byte_written = DWORD{0};
                auto result = WriteFile(
                    thiz.handle,
                    data.begin(),
                    sizeof(u8) * data.size(),
                    &byte_written,
                    nullptr
                );
                assert_conditional(SUCCEEDED(result), "Write file operation has not been completed", "write");
                assert_conditional(byte_written == sizeof(u8) * data.size(), fmt::format("Missing bytes when write file, expected: {} but got: {}", sizeof(u8) * data.size(), byte_written), "write");
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

            explicit WindowsFileReader(const String& path) {
                thiz.handle = CreateFileW(
                    path.wstring().data(),
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    nullptr,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr
                );
                assert_conditional(thiz.handle != INVALID_HANDLE_VALUE, fmt::format("{}: {}", Language::get("cannot_read_file"), to_posix_style(path.view())), "WindowsFileReader");
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

            template <typename T> requires (std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>) && requires (T t) {
                { t.size() } -> std::convertible_to<usize>;
                { t.begin() } -> std::convertible_to<u8*>;
            }
            auto read (
                T& data
            ) -> void {
                auto byte_written = DWORD{0};
                auto result = ReadFile(
                    thiz.handle,
                    data.begin(),
                    sizeof(u8) * data.size(),
                    &byte_written,
                    nullptr
                );
                assert_conditional(SUCCEEDED(result), "Read file operation has not been completed", "read");
                assert_conditional(byte_written == sizeof(u8) * data.size(), fmt::format("Missing bytes when read file, expected: {} but got: {}", sizeof(u8) * data.size(), byte_written), "read");
            }

            auto data (
            ) -> HANDLE& {
                return thiz.handle;
            }
        };

        #endif

}
