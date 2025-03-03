#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/texture/decode.hpp"

namespace Sen::Kernel::Support::Texture
{
    struct Encode : Common
    {
    private:
        static auto exchange_red(Color const& color, usize const& bit_count) -> u8
        {
            return compress_color(color.red(), bit_count);
        }

        static auto exchange_blue(Color const& color, usize const& bit_count) -> u8
        {
            return compress_color(color.blue(), bit_count);
        }

        static auto exchange_green(Color const& color, usize const& bit_count) -> u8
        {
            return compress_color(color.green(), bit_count);
        }

        static auto exchange_alpha(Color const& color, usize const& bit_count) -> u8
        {
            return compress_color(color.alpha(), bit_count);
        }

        template <is_integer T, auto size = sizeof(T)>
        static constexpr auto write_of(Uint8Array& data, T const& value, size_t& position) -> void
        {
            std::memcpy(&data[position], &value, size);
            position += size;
        }

        template <auto format>
        static constexpr auto get_bpp() -> usize
        {
            static_assert(sizeof(format) == sizeof(Format), "Invalid Format");
            if constexpr (format == Format::ARGB_8888)
            {
                return 32_size;
            }
            if constexpr (format == Format::RGBA_8888)
            {
                return 32_size;
            }
            if constexpr (format == Format::RGBA_4444)
            {
                return 16_size;
            }
            if constexpr (format == Format::RGB_565)
            {
                return 16_size;
            }
            if constexpr (format == Format::RGBA_5551)
            {
                return 16_size;
            }
            if constexpr (format == Format::RGBA_4444_TILED)
            {
                return 16_size;
            }
            if constexpr (format == Format::RGB_565_TILED)
            {
                return 16_size;
            }
            if constexpr (format == Format::RGBA_5551_TILED)
            {
                return 16_size;
            }
            if constexpr (format == Format::RGBA_PVRTC_4BPP)
            {
                return 4_size;
            }
            if constexpr (format == Format::RGB_ETC1_A_8)
            {
                return 4_size;
            }
            if constexpr (format == Format::RGB_ETC1_A_PALETTE)
            {
                return 4_size;
            }
            if constexpr (format == Format::RGB_PVRTC_4BPP_A_8)
            {
                return 4_size;
            }
        }


        template <auto format>
        static constexpr auto compute_padded_image_size(Rectangle &dimension) -> void
        {
            static_assert(sizeof(format) == sizeof(Format), "Invalid Format");
            if constexpr (format == Format::RGB_ETC1_A_8 || format == Format::RGB_ETC1_A_PALETTE)
            {
                dimension.width = log2n(dimension.width);
                dimension.height = log2n(dimension.height);
            }
            else if constexpr (format == Format::RGBA_PVRTC_4BPP || format == Format::RGB_PVRTC_4BPP_A_8)
            {
                const auto padded_width = log2n(dimension.width);
                const auto padded_height = log2n(dimension.height);
                const auto maximum_size = padded_width > padded_height ? padded_width : padded_height;
                dimension.width = maximum_size;
                dimension.height = maximum_size;
            }

        }

        template <auto format>
        static constexpr auto compute_data_size(Rectangle &dimension) -> usize
        {
            static_assert(sizeof(format) == sizeof(Format), "Invalid Format");
            compute_padded_image_size<format>(dimension);
            return dimension.area() * get_bpp<format>() / 8_size;
        }

        static auto compress_color(u8 const& value, usize const& bit_count) -> u8
        {
            return clip_bit(value, 8_size - bit_count, bit_count);
        }

        template <is_integer Type, auto format, auto size = sizeof(Type), is_exchanger Exchanger>
        static auto exchange_image(Exchanger const& exchange_pixel, Image& image, Uint8Array& value) -> void
        {
            static_assert(sizeof(format) == sizeof(Format), "Invalid Format");
            auto position = k_begin_index;
            {
                auto rectangle = image.new_rectangle();
                value.allocate(compute_data_size<format>(rectangle));
            }
            for (const auto y : Range{image.height})
            {
                for (const auto x : Range{image.width})
                {
                    auto& rows = image[y][x];
                    auto block = Type{};
                    exchange_pixel(rows, block);
                    write_of<Type, size>(value, block, position);
                }
            }
        }

        template <is_integer Type, auto format, auto size = sizeof(Type), is_exchanger Exchanger>
        static auto exchange_image_tiled(Exchanger const& exchange_pixel, Image& image, Uint8Array& value) -> void
        {
            static_assert(sizeof(format) == sizeof(Format), "Invalid Format");
            auto position = k_begin_index;
            {
                auto rectangle = image.new_rectangle();
                value.allocate(compute_data_size<format>(rectangle));
            }
            for (const auto y : Range{image.height})
            {
                for (const auto x : Range{image.width})
                {
                    auto& rows = image[y][x];
                    auto block = Type{};
                    exchange_pixel(rows, block);
                    write_of<Type, size>(value, block, position);
                }
            }
        }

        static auto exchange_image_etc_v1(Image& image, Uint8Array &value, usize &position) -> void
	    {
            const auto encoded_size = etc1_get_encoded_data_size(image.width, image.width);
            auto source = Uint8Array{image.area() * 3_size};
            exchange_image<u32, Format::RGBA_8888, 3_size>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 8_size) << 0_size |
                    exchange_green(data, 8_size) << 8_size |
                    exchange_blue(data, 8_size) << 16_size |
                    exchange_alpha(data, 8_size) << 24_size;
            }, image, source);
	        const auto stride = image.width * 3_ui;
	        etc1_encode_image(source.data(), image.width, image.width, 3_ui, stride, value.data());
	        position = encoded_size;
	    }

	    static auto exchange_image_rgba_pvrtc_4bpp(Image& image, Uint8Array &value, usize &position) -> void
	    {
            //TODO:
	    }

    public:
        static auto argb_8888(Image& image, Uint8Array& value) -> void
        {
            exchange_image<u32, Format::ARGB_8888>([](Color const& data, auto& block)
            {
                block = exchange_alpha(data, 8_size) << 0_size |
                    exchange_red(data, 8_size) << 8_size |
                    exchange_green(data, 8_size) << 16_size |
                    exchange_blue(data, 8_size) << 24_size;
            }, image, value);
        }

        static auto rgba_8888(Image& image, Uint8Array& value) -> void
        {
            exchange_image<u32, Format::RGBA_8888>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 8_size) << 0_size |
                    exchange_green(data, 8_size) << 8_size |
                    exchange_blue(data, 8_size) << 16_size |
                    exchange_alpha(data, 8_size) << 24_size;
            }, image, value);
        }

        static auto rgba_4444(Image& image, Uint8Array& value) -> void
        {
            exchange_image<u16, Format::RGBA_4444>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 4_size) << 0_size |
                    exchange_green(data, 4_size) << 4_size |
                    exchange_blue(data, 4_size) << 8_size |
                    exchange_alpha(data, 4_size) << 12_size;
            }, image, value);
        }

        static auto rgba_4444_tiled(Image& image, Uint8Array& value) -> void
        {
            exchange_image_tiled<u16, Format::RGBA_4444_TILED>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 4_size) << 0_size |
                    exchange_green(data, 4_size) << 4_size |
                    exchange_blue(data, 4_size) << 8_size |
                    exchange_alpha(data, 4_size) << 12_size;
            }, image, value);
        }

        static auto rgba_565(Image& image, Uint8Array& value) -> void
        {
            exchange_image<u16, Format::RGB_565>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 5_size) << 0_size |
                    exchange_green(data, 6_size) << 5_size |
                    exchange_blue(data, 5_size) << 11_size;
            }, image, value);
        }

        static auto rgba_565_tiled(Image& image, Uint8Array& value) -> void
        {
            exchange_image_tiled<u16, Format::RGB_565_TILED>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 5_size) << 0_size |
                    exchange_green(data, 6_size) << 5_size |
                    exchange_blue(data, 5_size) << 11_size;
            }, image, value);
        }

        static auto rgba_5551(Image& image, Uint8Array& value) -> void
        {
            exchange_image<u16, Format::RGBA_5551>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 1_size) << 0_size |
                    exchange_green(data, 5_size) << 1_size |
                    exchange_blue(data, 5_size) << 6_size |
                    exchange_alpha(data, 5_size) << 11_size;
            }, image, value);
        }

        static auto rgba_5551_tiled(Image& image, Uint8Array& value) -> void
        {
            exchange_image_tiled<u16, Format::RGBA_5551_TILED>([](Color const& data, auto& block)
            {
                block = exchange_red(data, 1_size) << 0_size |
                    exchange_green(data, 5_size) << 1_size |
                    exchange_blue(data, 5_size) << 6_size |
                    exchange_alpha(data, 5_size) << 11_size;
            }, image, value);
        }

        static auto rgba_etc1_a8(Image& image, Uint8Array& value) -> void
        {
            auto position = k_begin_index;
            exchange_image_etc_v1(image, value, position);
            for (const auto y : Range{image.height})
            {
                for (const auto x : Range{image.width})
                {
                    auto &rows = image[y][x];
                    write_of<u8>(value, rows.alpha(), position);
                }
            }
        }

        static auto rgb_etc1_a_palette(Image& image, Uint8Array& value) -> void
        {
            auto position = k_begin_index;
            exchange_image_etc_v1(image, value, position);
            value[position++] = 0x10;
            Math::iterate_each<0x10_size>([&]<auto index>() -> void {
                value[position++] = index;
            });
            for (const auto y : Range{image.height / 2_size}) {
                for (const auto x : Range{image.width / 2_size}) {
                    auto &rows_a = image[y << 1][x << 1];
                    auto &rows_b = image[y << 1 | 1][x << 1 | 1];
                    const auto index_value = static_cast<u8>(rows_a.red() & 0b11110000 | rows_b.red() >> 4);
                    value[position++] = index_value;
                }
            }
        }

        static auto rgba_pvrtc_4bpp(Image& image, Uint8Array& value) -> void
        {
            auto position = k_begin_index;
            exchange_image_rgba_pvrtc_4bpp(image, value, position);
        }

        static auto rgb_pvrtc_4bpp_a8(Image& image, Uint8Array& value) -> void
        {
            auto position = k_begin_index;
            exchange_image_rgba_pvrtc_4bpp(image, value, position);
            for (const auto y : Range{image.height})
            {
                for (const auto x : Range{image.width})
                {
                    auto &rows = image[y][x];
                    write_of<u8>(value, rows.alpha(), position);
                }
            }
        }
    };
}
