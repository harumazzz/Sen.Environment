#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/texture/common.hpp"
namespace Sen::Kernel::Support::Texture
{

	struct Decode : Common
	{
	private:

	    static constexpr auto exchange_red(Color &color, u8 const & value) -> void
	    {
	        color.red(value);
	    }

	    static constexpr auto exchange_blue(Color &color, u8 const & value) -> void
	    {
	        color.blue(value);
	    }

	    static constexpr auto exchange_green(Color &color, u8 const & value) -> void
	    {
	        color.green(value);
	    }

	    static constexpr auto exchange_alpha(Color &color, u8 const & value) -> void
	    {
	        color.alpha(value);
	    }

	    template <is_integer T, auto size = sizeof(T)>
	    static constexpr auto read_of(Uint8Array &data, size_t &position) -> T
	    {
           // constexpr auto size = sizeof(T);
	        auto value = T{};
            std::memcpy(&value, data.begin() + position, size);
            position += size;
	        return value;
	    }

	    template <is_integer Type, auto size = sizeof(Type), is_exchanger Exchanger>
	    static auto exchange_image(Exchanger const &exchange_pixel, Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        auto position = k_begin_index;
	        image.allocate(width, height);
	        for (const auto y : Range{image.height})
	        {
	            for (const auto x : Range{image.width})
	            {
	                auto &rows = image[y][x];
	                const auto block = read_of<Type, size>(value, position);
	                exchange_pixel(rows, block);
	            }
	        }
	    }

	    template <is_integer Type, is_exchanger Exchanger>
	    static auto exchange_image_tiled(Exchanger const &exchange_pixel, Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        auto position = k_begin_index;
	       const auto block_height = width / k_block_width;
	       const auto chunk_height = height / k_chunk_width;
	        image.allocate(width, height);
            for (const auto block_y : Range{chunk_height})
            {
                for (const auto block_x : Range{k_chunk_width})
                {
                    auto& cols = image[set_pixel(block_y, block_x, k_chunk_width)];
                    for (const auto block_row : Range{block_height})
                    {
                        for (const auto block_col : Range{k_block_width})
                        {
                            auto& rows = cols[set_pixel(block_row, block_col, k_block_width)];
                            const auto block = read_of<Type>(value, position);
                            exchange_pixel(rows, block);
                        }
                    }
                }
            }
	    }

	    static auto exchange_image_etc_v1(Image& image, Uint8Array &value, u32 const &width, u32 const &height, usize &position) -> void
	    {
	        const auto encoded_size = etc1_get_encoded_data_size(width, height);
	        const auto stride = width * 3_ui;
	        auto dest = Uint8Array{stride * height};
	        etc1_decode_image(value.data(), dest.data(), width, height, 3_ui, stride);
	        exchange_image<u32, 3_size>([](auto &data, auto &block)
            {
	            exchange_red(data, clip_bit(block, 0_size, 8_size));
                exchange_green(data, clip_bit(block, 8_size, 8_size));
                exchange_blue(data, clip_bit(block, 16_size, 8_size));
                exchange_alpha(data, 255);
            }, image, dest, width, height);
	        position = encoded_size;
	    }

	    static auto exchange_image_rgba_pvrtc_4bpp(Image& image, Uint8Array &value, u32 const &width, u32 const &height, usize &position) -> void
	    {
	        assert_conditional(is_padded_size(width, k_block_width), "Failed width", "exchange_image_rgba_pvrtc_4bpp"); //TODO:
	        assert_conditional(is_padded_size(height, k_block_width), "Failed height", "exchange_image_rgba_pvrtc_4bpp"); //TODO:
            auto proxy_image_data = List<Subprojects::PVRTCCompressor::ColorRgba<u8>>{image.area()};
	        const auto point = Subprojects::PVRTCCompressor::Point2<int>{static_cast<int>(width), static_cast<int>(height)};
            Subprojects::PVRTCCompressor::PvrTcDecoder::DecodeRgba4Bpp(proxy_image_data.data(), point, value.data());
	        image.allocate(width, height);
	        for (const auto y : Range{image.height})
	        {
	            for (const auto x : Range{image.width})
	            {
	                auto &rows = image[y][x];
	                auto & proxy_pixel = proxy_image_data[set_pixel(x, y, width)];
	                exchange_red(rows, proxy_pixel.r);
	                exchange_green(rows, proxy_pixel.g);
	                exchange_blue(rows, proxy_pixel.b);
	                exchange_alpha(rows, proxy_pixel.a);
	            }
	        }
	        position = proxy_image_data.size();
	    }

	public:
	    static auto argb_8888(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image<u32>([](auto &data, auto &block)
	        {
	            exchange_alpha(data, clip_bit(block, 0_size, 8_size));
                exchange_red(data, clip_bit(block, 8_size, 8_size));
                exchange_green(data, clip_bit(block, 16_size, 8_size));
                exchange_blue(data, clip_bit(block, 24_size, 8_size));
	        }, image, value, width, height);
	    }

	    static auto rgba_8888(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image<u32>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 8_size));
                exchange_green(data, clip_bit(block, 8_size, 8_size));
                exchange_blue(data, clip_bit(block, 16_size, 8_size));
                exchange_alpha(data, clip_bit(block, 24_size, 8_size));
            }, image, value, width, height);
	    }

	    static auto rgba_4444(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image<u16>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 4_size));
                exchange_green(data, clip_bit(block, 4_size, 4_size));
                exchange_blue(data, clip_bit(block, 8_size, 4_size));
                exchange_alpha(data, clip_bit(block, 12_size, 4_size));
            }, image, value, width, height);
	    }

	    static auto rgba_4444_tiled(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image_tiled<u16>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 4_size));
                exchange_green(data, clip_bit(block, 4_size, 4_size));
                exchange_blue(data, clip_bit(block, 8_size, 4_size));
                exchange_alpha(data, clip_bit(block, 12_size, 4_size));
            }, image, value, width, height);
	    }

	    static auto rgba_565(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image<u16>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 5_size));
                exchange_green(data, clip_bit(block, 5_size, 6_size));
                exchange_blue(data, clip_bit(block, 11_size, 5_size));
            }, image, value, width, height);
	    }

	    static auto rgba_565_tiled(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image_tiled<u16>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 5_size));
                exchange_green(data, clip_bit(block, 5_size, 6_size));
                exchange_blue(data, clip_bit(block, 11_size, 5_size));
            }, image, value, width, height);
	    }

	    static auto rgba_5551(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image<u16>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 1_size));
                exchange_green(data, clip_bit(block, 1_size, 5_size));
                exchange_blue(data, clip_bit(block, 6_size, 5_size));
	            exchange_alpha(data, clip_bit(block, 11_size, 5_size));
            }, image, value, width, height);
	    }

	    static auto rgba_5551_tiled(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        exchange_image_tiled<u16>([](auto &data, auto &block)
            {
                exchange_red(data, clip_bit(block, 0_size, 1_size));
                exchange_green(data, clip_bit(block, 1_size, 5_size));
                exchange_blue(data, clip_bit(block, 6_size, 5_size));
                exchange_alpha(data, clip_bit(block, 11_size, 5_size));
            }, image, value, width, height);
	    }

	    static auto rgba_etc1_a8(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        auto position = k_begin_index;
	        exchange_image_etc_v1(image, value, width, height, position);
	    	for (const auto y : Range{image.height})
	    	{
	    		auto &cols = image[y];
	    		for (const auto x : Range{image.width})
	    		{
	    			auto &rows = cols[x];
	    			const auto block = read_of<u8>(value, position);
	    			exchange_alpha(rows, block);
	    		}
	    	}
	    }

	    static auto rgb_etc1_a_palette(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void
	    {
	        auto position = k_begin_index;
	        exchange_image_etc_v1(image, value, width, height, position);
	    	const auto index_count = read_of<u8>(value, position);
	    	auto palette = Uint8Array{index_count != 0_size ? index_count : 2_size};
	    	auto bit_depth = 0;
			if (index_count == 0_byte) {
				palette[0] = 0x0;
				palette[1] = 0xff;
				bit_depth = 1;
			}
	    	else {
				for (const auto i : Range{index_count}) {
					const auto color = read_of<u8>(value, position);
					palette[i] = color << 4 | color;
				}
	    		bit_depth = index_count == 1 ? 1 : std::ilogb(index_count - 1) + 1;
	    	}
	    	auto bit_position = k_begin_index;
	    	auto buffer = 0_byte;
	    	for (const auto y : Range{height}) {
	    		for (const auto x : Range{width}) {
	    			auto &rows = image[y][x];
	    			auto index_value = 0;
	    			for (auto i = bit_depth -1; i >= 0; --i) {
	    				if (bit_position == 0) {
	    					buffer = read_of<u8>(value, position);
	    				}
	    				bit_position = (bit_position + 7) & 7;
	    				index_value |= ((buffer >> bit_position) & 0b1) << i;
	    			}
	    			exchange_alpha(rows, palette[index_value]);
	    		}
	    	}
	    }

		static auto rgba_pvrtc_4bpp(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void {
	        auto position = k_begin_index;
	        exchange_image_rgba_pvrtc_4bpp(image, value, width, height, position);
	    }

		static auto rgb_pvrtc_4bpp_a8(Image& image, Uint8Array &value, u32 const &width, u32 const &height) -> void {
	        auto position = k_begin_index;
	        exchange_image_rgba_pvrtc_4bpp(image, value, width, height, position);
	        for (const auto y : Range{image.height})
	        {
	            auto &cols = image[y];
	            for (const auto x : Range{image.width})
	            {
	                auto &rows = cols[x];
	                const auto block = read_of<u8>(value, position);
	                exchange_alpha(rows, block);
	            }
	        }
	    }
	};
}