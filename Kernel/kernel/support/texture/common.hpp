#pragma once

#include "kernel/utility/encoding/image/basic_image.hpp"
#include "kernel/utility/encoding/image/read.hpp"
#include "kernel/utility/encoding/image/write.hpp"

namespace Sen::Kernel::Support::Texture {

    enum class Format : u8 {
        ARGB_8888 = 1,
        RGBA_8888,
        RGBA_4444,
        RGB_565,
        RGBA_5551,
        RGBA_4444_TILED,
        RGB_565_TILED,
        RGBA_5551_TILED,
        RGBA_PVRTC_4BPP,
        RGB_ETC1_A_8,
        RGB_ETC1_A_PALETTE,
        RGB_PVRTC_4BPP_A_8,
        // ARGB_1555,
        // ARGB_4444,
        // RGB_ETC1,
        // RGB_PVRTC_4BPP,
        //A_8,
        //L_8,
        //LA_44,
        //LA_88,
    };

    using Image = Encoding::Image::BasicImage;

    using Rectangle = Encoding::Image::BasicRectangle<u32>;

    using Color = Encoding::Image::BasicColor<u8>;

	struct Common
	{
        static constexpr auto calculate_area(u32 const &width, u32 const& height) -> u32
        {
            return width * height;
        }

	    static constexpr auto pixel_area(u32 const &area) -> u32
        {
            return area * 4_ui;
        }

	    static constexpr auto set_pixel(size_t const &x, size_t const&y, u32 const&width) -> size_t
        {
            return (y * width + x) * 4_size;
        }
	};

}


template <>
struct Sen::Kernel::Javascript::Trait<Sen::Kernel::Support::Texture::Format> {

    using Format = Sen::Kernel::Support::Texture::Format;

    static auto from_value(
        Value& source,
        Format& destination
    ) -> void {
        assert_conditional(source.is_bigint(), "Expected the value to be number, but the actual type is not", "from_value");
        auto value = i64{};
        Trait<i64>::from_value(source, value);
        assert_conditional(Math::is_between(value, static_cast<i64>(Format::ARGB_8888), static_cast<i64>(Format::RGB_PVRTC_4BPP_A_8)), "Unsupported texture format", "from_value");
        destination = static_cast<Format>(value);
    }


};