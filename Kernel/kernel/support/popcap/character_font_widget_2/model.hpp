#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::CharacterFontWidget2
{
    struct CharacterItem
    {
        String index;
        String value;

        constexpr explicit CharacterItem() = default;

        ~CharacterItem() = default;
    };

    struct FontKerning
    {
        String index;
        i32 offset;

        constexpr explicit FontKerning() = default;

        ~FontKerning() = default;
    };

    struct FontCharacter
    {
        String index;
        i32 image_rect_x;
        i32 image_rect_y;
        i32 image_rect_width;
        i32 image_rect_height;
        i32 image_offset_x;
        i32 image_offset_y;
        i32 kerning_first;
        i32 kerning_count;
        i32 width;
        i32 order;

        constexpr explicit FontCharacter() = default;

        ~FontCharacter() = default;
    };

    struct FontLayer
    {
        String name;
        List<String> tag_require;
        List<String> tag_exclude;
        List<FontKerning> kerning;
        List<FontCharacter> character;
        i32 multiply_red;
        i32 multiply_green;
        i32 multiply_blue;
        i32 multiply_alpha;
        i32 add_red;
        i32 add_green;
        i32 add_blue;
        i32 add_alpha;
        String image_file;
        i32 draw_mode;
        i32 offset_x;
        i32 offset_y;
        i32 spacing;
        i32 minimum_point_size;
        i32 maximum_point_size;
        i32 point_size;
        i32 ascent;
        i32 ascent_padding;
        i32 height;
        i32 default_height;
        i32 line_spacing_offset;
        i32 base_order;

        constexpr explicit FontLayer() = default;

        ~FontLayer() = default;
    };


    struct FontWidget
    {
        i32 ascent;
        i32 ascent_padding;
        i32 height;
        i32 line_sepacing_offset;
        bool initialized;
        i32 default_point_size;
        List<CharacterItem> character;
        List<FontLayer> layer;
        String source_file;
        String error_header;
        i32 point_size;
        List<String> tag;
        float scale;
        bool force_scaled_image_white;
        bool activate_all_layer;

        constexpr explicit FontWidget() = default;

        ~FontWidget() = default;
    };
}

using namespace Sen::Kernel::Support::PopCap::CharacterFontWidget2;

JSONCONS_ALL_MEMBER_TRAITS(CharacterItem, index, value);

JSONCONS_ALL_MEMBER_TRAITS(FontKerning, index, offset);

JSONCONS_ALL_MEMBER_TRAITS(FontCharacter, index, image_rect_x, image_rect_y, image_rect_width, image_rect_height, image_offset_x, image_offset_y, kerning_first, kerning_count, width, order);

JSONCONS_ALL_MEMBER_TRAITS(FontLayer, name, tag_require, tag_exclude, kerning, character, multiply_red, multiply_green, multiply_blue, multiply_alpha, add_red, add_green, add_blue, add_alpha, image_file, draw_mode, offset_x, offset_y, spacing, minimum_point_size, maximum_point_size, point_size, ascent, ascent_padding, height, default_height, line_spacing_offset, base_order);

JSONCONS_ALL_MEMBER_TRAITS(FontWidget, ascent, ascent_padding, height, line_sepacing_offset, initialized, default_point_size, character, layer, source_file, error_header, point_size, tag, scale, force_scaled_image_white, activate_all_layer);