#pragma once

#include "kernel/utility/encoding/image/common.hpp"
#include "kernel/utility/encoding/image/basic_image.hpp"
#include "kernel/utility/filesystem/utility.hpp"

namespace Sen::Kernel::Encoding::Image {

    struct Write : Common {

        protected:

            using Image = CImage;

            using Color = Color;

            using Rectangle = Rectangle;

        public:

            static auto process_image (
                WriteMemoryStream& data,
                Image& image
            ) -> void {
                auto png_struct = Subprojects::libpng::png_create_write_struct(Subprojects::libpng::$PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
                Subprojects::libpng::png_set_error_fn(png_struct, nullptr, &png_error, &png_warning);
                Subprojects::libpng::png_set_write_fn(png_struct, &data, &write_png_data, &flush_png_data);
                auto png_info = Subprojects::libpng::png_create_info_struct(png_struct);
                Subprojects::libpng::png_set_IHDR(png_struct, png_info, static_cast<Subprojects::libpng::png_uint_32>(image.width), static_cast<Subprojects::libpng::png_uint_32>(image.height), static_cast<int>(bit_count<u8>()), Subprojects::libpng::$PNG_COLOR_TYPE_RGB_ALPHA, Subprojects::libpng::$PNG_INTERLACE_NONE, Subprojects::libpng::$PNG_COMPRESSION_TYPE_DEFAULT, Subprojects::libpng::$PNG_FILTER_TYPE_DEFAULT);
                Subprojects::libpng::png_write_info(png_struct, png_info);
                for (auto & row : image.data()) {
                    Subprojects::libpng::png_write_row(png_struct, reinterpret_cast<Subprojects::libpng::png_const_bytep>(row.data()));
                }
                Subprojects::libpng::png_write_end(png_struct, png_info);
                Subprojects::libpng::png_destroy_write_struct(&png_struct, &png_info);
            }

            static auto process (
                Uint8Array& source,
                Image& image
            ) -> void {
                auto stream = WriteMemoryStream{ image.color_size() };
                process_image(stream, image);
                stream.release_stream(source);
            }

            static auto process_fs (
                const StringView& destination,
                Image& image
            ) -> void {
                auto source_view = Uint8Array{};
                process(source_view, image);
                return FileSystem::write_file(destination, source_view);
            }

    };
}
