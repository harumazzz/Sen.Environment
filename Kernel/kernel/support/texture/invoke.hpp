#pragma once

#include "kernel/support/texture/decode.hpp"
#include "kernel/support/texture/encode.hpp"


namespace Sen::Kernel::Support::Texture {
    namespace InvokeMethod {
        struct Decode {
            static auto process_whole(Image &image, Uint8Array &value, u32 const &width, u32 const &height, const Format& format) -> void {
                switch (format) {
                    case Format::ARGB_8888: {
                        Texture::Decode::argb_8888(image, value, width, height);
                        break;
                    }
                    case Format::RGBA_8888: {
                        Texture::Decode::rgba_8888(image, value, width, height);
                        break;
                    }
                    case Format::RGBA_4444: {
                        Texture::Decode::rgba_4444(image, value, width, height);
                        break;
                    }
                    case Format::RGB_565: {
                        Texture::Decode::rgba_565(image, value, width, height);
                        break;
                    }
                    case Format::RGBA_5551: {
                        Texture::Decode::rgba_5551(image, value, width, height);
                        break;
                    }
                    case Format::RGBA_4444_TILED: {
                        Texture::Decode::rgba_4444_tiled(image, value, width, height);
                        break;
                    }
                    case Format::RGB_565_TILED: {
                        Texture::Decode::rgba_565_tiled(image, value, width, height);
                        break;
                    }
                    case Format::RGBA_5551_TILED: {
                        Texture::Decode::rgba_5551_tiled(image, value, width, height);
                        break;
                    }
                    case Format::RGBA_PVRTC_4BPP: {
                        Texture::Decode::rgba_pvrtc_4bpp(image, value, width, height);
                        break;
                    }
                    case Format::RGB_ETC1_A_8: {
                        Texture::Decode::rgba_etc1_a8(image, value, width, height);
                        break;
                    }
                    case Format::RGB_ETC1_A_PALETTE: {
                        Texture::Decode::rgb_etc1_a_palette(image, value, width, height);
                        break;
                    }
                    case Format::RGB_PVRTC_4BPP_A_8: {
                        Texture::Decode::rgb_pvrtc_4bpp_a8(image, value, width, height);
                        break;
                    }
                    default: {
                        assert_conditional(false, "Format is not supported", "process_whole"); //TODO:
                    }
                }
            }

            static auto process_fs(StringView const &source, StringView const &destination, u32 const &width, u32 const &height, const Format& format) -> void {
                auto image = Image{};
                auto buffer = Uint8Array{};
                FileSystem::read_file(source, buffer);
                process_whole(image, buffer, width, height, format);
                Encoding::Image::Write::process_fs(destination, image);
            }
        };


        struct Encode {
            static auto process_whole(Image &image, Uint8Array &value, const Format& format) -> void {
                switch (format) {
                    case Format::ARGB_8888: {
                        break;
                    }
                    case Format::RGBA_8888: {
                        Texture::Encode::rgba_8888(image, value);
                        break;
                    }
                    case Format::RGBA_4444: {
                    }
                    case Format::RGB_565: {
                    }
                    case Format::RGBA_5551: {
                    }
                    case Format::RGBA_4444_TILED: {
                    }
                    case Format::RGB_565_TILED: {
                    }
                    case Format::RGBA_5551_TILED: {
                    }
                    case Format::RGBA_PVRTC_4BPP: {
                    }
                    case Format::RGB_ETC1_A_8: {
                    }
                    case Format::RGB_ETC1_A_PALETTE: {
                    }
                    case Format::RGB_PVRTC_4BPP_A_8: {
                    }
                    default: {
                        assert_conditional(false, "Format is not supported", "process_whole");
                    }
                }
            }

            static auto process_fs(StringView const &source, StringView const &destination, const Format& format) -> void {
                auto image = Image{};
                auto buffer = Uint8Array{};
                Encoding::Image::Read::process_fs(source, image);
                process_whole(image, buffer, format);
                FileSystem::write_file(destination, buffer);
            }
        };
    };
}
