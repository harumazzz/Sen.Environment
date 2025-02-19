#pragma once

#include "kernel/subprojects/libpng.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Encoding::Image {

    struct Common {

        inline static auto png_error (
            Subprojects::libpng::png_structp png_ptr,
            Subprojects::libpng::png_const_charp error_message
        ) -> void {
            assert_conditional(false, fmt::format("png has error: {}", error_message), "png_error");
        }

        inline static auto png_warning (
            Subprojects::libpng::png_structp     png_ptr,
            Subprojects::libpng::png_const_charp error_message
        ) -> void {
        }

        inline static auto read_png_data (
            Subprojects::libpng::png_structp png_ptr,
            Subprojects::libpng::png_bytep data,
            Subprojects::libpng::size_t length
        ) -> void {
            auto stream = static_cast<ReadMemoryStream*>(Subprojects::libpng::png_get_io_ptr(png_ptr));
            assert_conditional(stream->has_space(length), "Stream is too small to read the current image", "read_png_data");
            std::memcpy(data, stream->current_iterator(), length);
            stream->operator+=(length);
        }

        inline static auto write_png_data (
            Subprojects::libpng::png_structp png_ptr,
            Subprojects::libpng::png_bytep data,
            Subprojects::libpng::size_t length
        ) -> void {
            auto stream = static_cast<WriteMemoryStream*>(Subprojects::libpng::png_get_io_ptr(png_ptr));
            assert_conditional(stream->has_space(length), "Stream is too small to write the current image", "write_png_data");
            stream->raw(reinterpret_cast<u8 *>(data), length);
        }

        inline static auto flush_png_data (
            Subprojects::libpng::png_structp png_ptr
        ) -> void {
        }

        template <typename T>
        static constexpr auto bit_count(
        ) -> usize {
            return sizeof(T) * 8_size;
        }

    };

}
