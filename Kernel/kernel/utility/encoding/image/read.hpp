#pragma once

#include "kernel/utility/encoding/image/common.hpp"
#include "kernel/utility/encoding/image/basic_image.hpp"

namespace Sen::Kernel::Encoding::Image {

    struct Read : Common {

    protected:

        using Image = CImage;

        using Color = Color;

        using Rectangle = Rectangle;

    public:

        static auto process_image (
            ReadStream& data,
            Image& image
        ) -> void {
            auto png_struct = Subprojects::libpng::png_create_read_struct(Subprojects::libpng::$PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            Subprojects::libpng::png_set_error_fn(png_struct, nullptr, &png_error, &png_warning);
            Subprojects::libpng::png_set_read_fn(png_struct, &data, &read_png_data);
            auto png_info = Subprojects::libpng::png_create_info_struct(png_struct);
            Subprojects::libpng::png_read_info(png_struct, png_info);
            image.allocate_new(png_info->width, png_info->height);
            switch (png_info->color_type) {
                case Subprojects::libpng::$PNG_COLOR_TYPE_PALETTE : {
                    Subprojects::libpng::png_set_add_alpha(png_struct, 0xFF, Subprojects::libpng::$PNG_FILLER_AFTER);
                    Subprojects::libpng::png_set_palette_to_rgb(png_struct);
                    break;
                }
                case Subprojects::libpng::$PNG_COLOR_TYPE_GRAY : {
                    Subprojects::libpng::png_set_add_alpha(png_struct, 0xFF, Subprojects::libpng::$PNG_FILLER_AFTER);
                    if (png_info->bit_depth == 1 || png_info->bit_depth == 2 || png_info->bit_depth == 4) {
                        Subprojects::libpng::png_set_expand_gray_1_2_4_to_8(png_struct);
                    }
                    [[fallthrough]];
                }
                case Subprojects::libpng::$PNG_COLOR_TYPE_GRAY_ALPHA : {
                    if ((*png_info).bit_depth == 16) {
                        Subprojects::libpng::png_set_scale_16(png_struct);
                    }
                    Subprojects::libpng::png_set_gray_to_rgb(png_struct);
                    break;
                }
                case Subprojects::libpng::$PNG_COLOR_TYPE_RGB : {
                    Subprojects::libpng::png_set_add_alpha(png_struct, 0xFF, Subprojects::libpng::$PNG_FILLER_AFTER);
                    [[fallthrough]];
                }
                case Subprojects::libpng::$PNG_COLOR_TYPE_RGB_ALPHA : {
                    if ((*png_info).bit_depth == 16) {
                        Subprojects::libpng::png_set_scale_16(png_struct);
                    }
                    break;
                }
                default: {
                    assert_conditional(false, fmt::format("Unknown color type: {}", png_info->color_type), "process");
                }
            }
            auto png_trans_alpha = Subprojects::libpng::png_bytep{};
            auto png_num_trans = int{};
            auto png_trans_color = Subprojects::libpng::png_color_16p{};
            if (Subprojects::libpng::png_get_tRNS(png_struct, png_info, &png_trans_alpha, &png_num_trans, &png_trans_color) != 0) {
                Subprojects::libpng::png_set_tRNS_to_alpha(png_struct);
            }
            auto png_background = Subprojects::libpng::png_color_16p{};
            if (Subprojects::libpng::png_get_bKGD(png_struct, png_info, &png_background) != 0) {
                Subprojects::libpng::png_set_background(png_struct, png_background, Subprojects::libpng::$PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
            }
            auto png_file_srgb_intent = int{};
            auto png_file_gamma = double{};
            if (Subprojects::libpng::png_get_sRGB(png_struct, png_info, &png_file_srgb_intent) != 0) {
                Subprojects::libpng::png_set_gamma(png_struct, Subprojects::libpng::$PNG_DEFAULT_sRGB, Subprojects::libpng::$PNG_DEFAULT_sRGB);
            }
            else if (Subprojects::libpng::png_get_gAMA(png_struct, png_info, &png_file_gamma) != 0) {
                Subprojects::libpng::png_set_gamma(png_struct, Subprojects::libpng::$PNG_DEFAULT_sRGB, png_file_gamma);
            }
            else {
                Subprojects::libpng::png_set_gamma(png_struct, Subprojects::libpng::$PNG_DEFAULT_sRGB, 0.45455);
            }
            for (auto & row : image.data()) {
                Subprojects::libpng::png_read_row(png_struct, reinterpret_cast<Subprojects::libpng::png_bytep>(row.begin()), nullptr);
            }
            Subprojects::libpng::png_read_end(png_struct, png_info);
            Subprojects::libpng::png_destroy_read_struct(&png_struct, &png_info, nullptr);
        }

        static auto process (
            Uint8Array& source,
            Image& image
        ) -> void {
            auto stream = ReadStream{ source };
            return process_image(stream, image);
        }


    };

}