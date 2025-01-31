#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel {

	#if defined(_WIN32) || defined(_WIN64)

        struct WindowsFileWriter {

            HANDLE handle;

            explicit WindowsFileWriter(const std::wstring& path) {
                handle = CreateFileW(
                    path.data(),
                    GENERIC_WRITE,   
                    0,               
                    nullptr,         
                    CREATE_ALWAYS,   
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr          
                );
                assert_conditional(handle != INVALID_HANDLE_VALUE, fmt::format("{}: {}", Language::get("write_file_error"), to_posix_style(utf16_to_utf8(path))), "WindowsFileWriter");
            }

            ~WindowsFileWriter() {
                if (handle != INVALID_HANDLE_VALUE) {
                    CloseHandle(handle);
                }
            }

            WindowsFileWriter(const WindowsFileWriter&) = delete;

            WindowsFileWriter& operator=(const WindowsFileWriter&) = delete;

            WindowsFileWriter(WindowsFileWriter&& other) noexcept : handle(other.handle) {
                other.handle = INVALID_HANDLE_VALUE;
            }

            WindowsFileWriter& operator=(WindowsFileWriter&& other) noexcept {
                if (this != &other) {
                    if (handle != INVALID_HANDLE_VALUE) {
                        CloseHandle(handle);
                    }
                    handle = other.handle;
                    other.handle = INVALID_HANDLE_VALUE;
                }
                return *this;
            }
        };

        struct WindowsFileReader {

            HANDLE handle;

            explicit WindowsFileReader(const std::wstring& path) {
                handle = CreateFileW(
                    path.data(),
                    GENERIC_READ,   
                    FILE_SHARE_READ,               
                    nullptr,         
                    OPEN_EXISTING,   
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr          
                );
                assert_conditional(handle != INVALID_HANDLE_VALUE, fmt::format("{}: {}", Language::get("write_file_error"), to_posix_style(utf16_to_utf8(path))), "WindowsFileReader");
            }

            ~WindowsFileReader() {
                if (handle != INVALID_HANDLE_VALUE) {
                    CloseHandle(handle);
                }
            }

            WindowsFileReader(const WindowsFileReader&) = delete;

            WindowsFileReader& operator=(const WindowsFileReader&) = delete;

            WindowsFileReader(WindowsFileReader&& other) noexcept : handle(other.handle) {
                other.handle = INVALID_HANDLE_VALUE;
            }

            WindowsFileReader& operator=(WindowsFileReader&& other) noexcept {
                if (this != &other) {
                    if (handle != INVALID_HANDLE_VALUE) {
                        CloseHandle(handle);
                    }
                    handle = other.handle;
                    other.handle = INVALID_HANDLE_VALUE;
                }
                return *this;
            }
        };

        #endif

}