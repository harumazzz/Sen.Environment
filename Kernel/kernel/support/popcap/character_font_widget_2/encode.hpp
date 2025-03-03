#pragma once

#include "kernel/support/popcap/character_font_widget_2/model.hpp"

namespace Sen::Kernel::Support::PopCap::CharacterFontWidget2
{
	struct Encode
    {
    private:
	    /*
        static auto exchange_character_item(WriteMemoryStream& stream, CharacterItem const& model) -> void
        {
            exchange_unicode_fixed<u16>(stream, model.index);
            exchange_unicode_fixed<u16>(stream, model.value);
        }

        static auto exchange_font_kerning(WriteMemoryStream& stream, FontKerning const& model) -> void
        {
            exchange_number_fixed<u16>(stream, model.offset);
            exchange_unicode_fixed<u16>(stream, model.index);
        }

        static auto exchange_font_character(WriteMemoryStream& stream, FontCharacter const& model) -> void
        {
            exchange_unicode_fixed<u16>(stream, model.index);
            exchange_number_fixed<i32>(stream, model.image_rect_x);
            exchange_number_fixed<i32>(stream, model.image_rect_y);
            exchange_number_fixed<i32>(stream, model.image_rect_width);
            exchange_number_fixed<i32>(stream, model.image_rect_height);
            exchange_number_fixed<i32>(stream, model.image_offset_x);
            exchange_number_fixed<i32>(stream, model.image_offset_y);
            exchange_number_fixed<u16>(stream, model.kerning_count);
            exchange_number_fixed<u16>(stream, model.kerning_first);
            exchange_number_fixed<i32>(stream, model.width);
            exchange_number_fixed<i32>(stream, model.order);
        }

        static auto exchange_string_32block(WriteMemoryStream& stream, String const& string) -> void
        {
            exchange_string_block<u32>(stream, string);
        }

        static auto exchange_font_layer(WriteMemoryStream& stream, FontLayer const& model) -> void
        {
            exchange_string_block<u32>(stream, model.name);
            exchange_list<u32>(&exchange_string_32block, stream, model.tag_require);
            exchange_list<u32>(&exchange_string_32block, stream, model.tag_exclude);
            exchange_list<u32>(&exchange_font_kerning, stream, model.kerning);
            exchange_list<u32>(&exchange_font_character, stream, model.character);
            exchange_number_fixed<i32>(stream, model.multiply_red);
            exchange_number_fixed<i32>(stream, model.multiply_green);
            exchange_number_fixed<i32>(stream, model.multiply_blue);
            exchange_number_fixed<i32>(stream, model.multiply_alpha);
            exchange_number_fixed<i32>(stream, model.add_red);
            exchange_number_fixed<i32>(stream, model.add_green);
            exchange_number_fixed<i32>(stream, model.add_blue);
            exchange_number_fixed<i32>(stream, model.add_alpha);
            exchange_string_block<u32>(stream, model.image_file);
            exchange_number_fixed<i32>(stream, model.draw_mode);
            exchange_number_fixed<i32>(stream, model.offset_x);
            exchange_number_fixed<i32>(stream, model.offset_y);
            exchange_number_fixed<i32>(stream, model.spacing);
            exchange_number_fixed<i32>(stream, model.minimum_point_size);
            exchange_number_fixed<i32>(stream, model.maximum_point_size);
            exchange_number_fixed<i32>(stream, model.point_size);
            exchange_number_fixed<i32>(stream, model.ascent);
            exchange_number_fixed<i32>(stream, model.ascent_padding);
            exchange_number_fixed<i32>(stream, model.height);
            exchange_number_fixed<i32>(stream, model.default_height);
            exchange_number_fixed<i32>(stream, model.line_spacing_offset);
            exchange_number_fixed<i32>(stream, model.base_order);
        }

        static auto exchange_font_widget(WriteMemoryStream& stream, FontWidget const& model) -> void
        {
            exchange_number_fixed<i32>(stream, model.ascent);
            exchange_number_fixed<i32>(stream, model.ascent_padding);
            exchange_number_fixed<i32>(stream, model.height);
            exchange_number_fixed<i32>(stream, model.line_sepacing_offset);
            exchange_boolean(stream, model.initialized);
            exchange_number_fixed<i32>(stream, model.default_point_size);
            exchange_list<u32>(&exchange_character_item, stream, model.character);
            exchange_list<u32>(&exchange_font_layer, stream, model.layer);
            exchange_string_block<u32>(stream, model.source_file);
            exchange_string_block<u32>(stream, model.error_header);
            exchange_number_fixed<i32>(stream, model.point_size);
            exchange_list<u32>(&exchange_string_32block, stream, model.tag);
            exchange_number_fixed<double>(stream, model.scale);
            exchange_boolean(stream, model.force_scaled_image_white);
            exchange_boolean(stream, model.activate_all_layer);
        }
        */
    public:
        static auto process_whole(WriteMemoryStream& stream, FontWidget const& model) -> void
        {
           // exchange_font_widget(stream, model);
        }

        static auto process_fs(StringView const& source, StringView const& destination) -> void
        {
            auto stream = WriteMemoryStream{};
            auto model = FontWidget{};
            FileSystem::read_json(source, model);
            process_whole(stream, model);
            FileSystem::write_file(destination, stream.view());
        }
    };
}