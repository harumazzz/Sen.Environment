#pragma once

#include "kernel/utility/container/list/byte_list.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/filesystem/utility.hpp"

namespace Sen::Kernel {

    class OutputStream final : public std::streambuf {

    protected:

        FILE* output;

    public:

        explicit OutputStream(FILE* stream) : output{ stream } {}

        explicit OutputStream(const std::string_view& filename) : output{ nullptr } {
            #if WINDOWS
            output = _wfopen(reinterpret_cast<const wchar_t*>(u8_to_u16(filename).data()), L"w");
            #else
            output = std::fopen(filename.data(), "w");
            #endif
            assert_not_null(output != nullptr, fmt::format("File cannot be opened: {}", filename), "OutputStream");
        }

        ~OutputStream(

        ) override {
            thiz.flush();
            if (output != nullptr && output != stdout) {
                std::fclose(output);
                output = nullptr;
            }
        }

        auto flush(
        ) const -> void {
            if (output != nullptr) {
                std::fflush(output);
            }
        }

    protected:

        auto sync(
        ) -> int override {
            return output != nullptr ? std::fflush(output) : 0;
        }

        auto overflow(
            const int ch
        ) -> int override {
            if (output != nullptr && ch != -1) {
                const auto c = static_cast<char>(ch);
                if (std::fwrite(&c, 1, 1, output) == 1) {
                    return static_cast<int>(ch);
                }
            }
            return -1;
        }

        auto xsputn(
            const char* data,
            const std::streamsize size
        ) -> std::streamsize override {
            return thiz.output != nullptr ? std::fwrite(data, 1, size, output) : 0;
        }
    };

}