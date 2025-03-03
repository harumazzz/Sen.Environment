#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/animation/model.hpp"

namespace Sen::Kernel::Support::PopCap::Animation
{

    struct Encode : Common
    {
    private:
        /*
        template<is_integer RawShort, is_integer RawLong>
        static auto exchange_integer_variant(WriteMemoryStream& stream, u32 const &size) -> void
        {
            constexpr auto value_short_maximum = value_max<RawShort>();
            auto value_long = static_cast<RawLong>(size);
            if (value_long < static_cast<RawLong>(value_short_maximum))
            {
                exchange_number_fixed<RawShort>(stream, value_long);
            }
            else
            {
                exchange_number_fixed<RawShort>(stream, value_short_maximum);
                exchange_number_fixed<RawLong>(stream, value_long);
            }
        }

        template<is_integer RawShort, is_integer RawLong, auto flag_count>
        static auto exchange_integer_variant_with_flag(WriteMemoryStream& stream, u32 const &value, std::bitset<flag_count> const &flag) -> void
        {
            constexpr auto value_short_bit_count = type_bit_count<RawShort>() - static_cast<size_t>(flag_count);
            constexpr auto value_short_maximum = static_cast<RawShort>(value_max<RawShort>() >> flag_count);

            auto value_long = static_cast<RawLong>(value);
            auto value_flag = static_cast<RawShort>(flag.to_ullong());
            auto value_short_with_flag = value_flag << value_short_bit_count;
            if (value_long < static_cast<RawLong>(value_short_maximum)) {
                value_short_with_flag |= static_cast<RawShort>(value_long);
                exchange_number_fixed<RawShort>(stream, value_short_with_flag);
            }
            else {
                value_short_with_flag |= value_short_maximum;
                exchange_number_fixed<RawShort>(stream, value_short_with_flag);
                exchange_number_fixed<RawLong>(stream, value_long);
            }
        }

        static auto exchange_image(WriteMemoryStream& stream, AnimationImage const& image) -> void {
            exchange_mutil_by_string_block<u16>(stream, vertical_bar, image.path, image.id);
            if (check_version(k_version, {4})) {
                exchange_number_fixed<u16>(stream, image.dimension.width);
                exchange_number_fixed<u16>(stream, image.dimension.height);
            }
            if (check_version(k_version, {1, 2})) {
                exchange_floater_with_rate<ValueRate::angle, i16>(stream, image.transform[0]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[1]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[2]);
            }
            else {
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[0]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[1]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[2]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[3]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[4]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[5]);
            }
        }

        static auto exchange_work_area(WriteMemoryStream& stream, AnimationWorkArea const& work_area) -> void
        {
            exchange_number_fixed<u16>(stream, work_area.start);
            exchange_number_fixed<u16>(stream, work_area.duration - 1);
        }

        static auto exchange_layer_remove(WriteMemoryStream& stream, u32 const& value) -> void
        {
            constexpr auto flag = std::bitset<LayerRemoveFlag::k_count>{};
            exchange_integer_variant_with_flag<u16, u32>(stream, value, flag);
        }

        static auto exchange_layer_append(WriteMemoryStream& stream, AnimationAppend const&append) -> void
        {
             auto flag = std::bitset<LayerAppendFlag::k_count>{};
            if (append.time_scale != 1.0) {
                flag.set(LayerAppendFlag::time_scale);
            }
            if (!append.name.empty()) {
                flag.set(LayerAppendFlag::name);
            }
            if (append.preload_frame != static_cast<u16>(0)) {
                flag.set(LayerAppendFlag::preload_frame);
            }
            if (append.additive) {
                flag.set(LayerAppendFlag::additive);
            }
            if (append.sprite) {
                flag.set(LayerAppendFlag::sprite);
            }
            exchange_integer_variant_with_flag<u16, u32>(stream, append.index, flag);
            if (check_version(k_version, {6}))
            {
                exchange_integer_variant<u8, u16>(stream, append.resource);
            }
            else {
                exchange_number_fixed<u8>(stream, append.resource);
            }
            if (flag.test(LayerAppendFlag::preload_frame))
            {
                exchange_number_fixed<u16>(stream, append.preload_frame);
            }
            if (flag.test(LayerAppendFlag::name))
            {
                exchange_string_block<u16>(stream, append.name);
            }
            if (flag.test(LayerAppendFlag::time_scale))
            {
                exchange_floater_with_rate<ValueRate::time, int>(stream, append.time_scale);
            }
        }

        static auto exchange_layer_change(WriteMemoryStream& stream, AnimationChange const &change) -> void
        {
            auto flag = std::bitset<LayerChangeFlag::k_count>{};
            if (change.sprite_frame_number != static_cast<u16>(0)) {
                flag.set(LayerChangeFlag::sprite_frame_number);
            }
            if (change.color.has_value()) {
                flag.set(LayerChangeFlag::color);
            }
            if (change.source_rectangle.has_value()) {
                flag.set(LayerChangeFlag::source_rectangle);
            }
            if (change.transform.size() == 3_size) {
                flag.set(LayerChangeFlag::rotate);
            }
            if (change.transform.size() == 6_size) {
                flag.set(LayerChangeFlag::matrix);
            }
            flag.set(LayerChangeFlag::long_coord);
            exchange_integer_variant_with_flag<u16, u32>(stream, change.index, flag);
            if (flag.test(LayerChangeFlag::rotate))
            {
                assert_conditional(!flag.test(LayerChangeFlag::matrix), "Flag matrix cannot appear in flag rotate", "exchange_layer_change"); //TODO: add locale
                exchange_floater_with_rate<ValueRate::angle, i16>(stream, change.transform[0]);
            }
            if (flag.test(LayerChangeFlag::matrix))
            {
                assert_conditional(!flag.test(LayerChangeFlag::rotate), "Flag rotate cannot appear in flag matrix", "exchange_layer_change"); //TODO: add locale
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, change.transform[0]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, change.transform[1]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, change.transform[2]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, change.transform[3]);
            }
            const auto tx_index = change.transform.size() - 2_size;
            const auto ty_index = change.transform.size() - 1_size;
            if (flag.test(LayerChangeFlag::long_coord))
            {
                exchange_floater_with_rate<ValueRate::size, i32>(stream, change.transform[tx_index]);
                exchange_floater_with_rate<ValueRate::size, i32>(stream, change.transform[ty_index]);
            }
            else
            {
                exchange_floater_with_rate<ValueRate::size, i16>(stream, change.transform[tx_index]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, change.transform[ty_index]);
            }
            if (flag.test(LayerChangeFlag::source_rectangle))
            {
                auto const &source_rectangle = *change.source_rectangle;
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[0]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[1]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[2]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[3]);
            }
            if (flag.test(LayerChangeFlag::color))
            {
                auto const &color = *change.color;
                exchange_floater_with_rate<ValueRate::color, u8>(stream, color[0]);
                exchange_floater_with_rate<ValueRate::color, u8>(stream, color[1]);
                exchange_floater_with_rate<ValueRate::color, u8>(stream, color[2]);
                exchange_floater_with_rate<ValueRate::color, u8>(stream, color[3]);
            }
            if (flag.test(LayerChangeFlag::sprite_frame_number))
            {
                exchange_number_fixed<u16>(stream, change.sprite_frame_number);
            }
        }

        static auto exchange_layer_command(WriteMemoryStream& stream, AnimationCommand const &command) -> void
        {
            exchange_string_block<u16>(stream, command.command);
            exchange_string_block<u16>(stream, command.argument);
        }

        static auto exchange_frame(WriteMemoryStream& stream, AnimationFrame const &frame) -> void
        {
            auto flag = std::bitset<FrameFlag::k_count>{};
            if (!frame.remove.empty()) {
                flag.set(FrameFlag::remove);
            }
            if (!frame.append.empty()) {
                flag.set(FrameFlag::append);
            }
            if (!frame.change.empty()) {
                flag.set(FrameFlag::change);
            }
            if (!frame.label.empty()) {
                flag.set(FrameFlag::label);
            }
            if (frame.stop) {
                flag.set(FrameFlag::stop);
            }
            if (!frame.command.empty()) {
                flag.set(FrameFlag::command);
            }
            exchange_number_fixed<u8>(stream, flag.to_ullong());
            if (flag.test(FrameFlag::remove))
            {
                exchange_integer_variant<u8, u16>(stream, frame.remove.size());
                exchange_list<void>(&exchange_layer_remove, stream, frame.remove);
            }
            if (flag.test(FrameFlag::append))
            {
                exchange_integer_variant<u8, u16>(stream, frame.append.size());
                exchange_list<void>(&exchange_layer_append, stream, frame.append);
            }
            if (flag.test(FrameFlag::change))
            {
                exchange_integer_variant<u8, u16>(stream, frame.change.size());
                exchange_list<void>(&exchange_layer_change, stream, frame.change);
            }
            if (flag.test(FrameFlag::label))
            {
                exchange_string_block<u16>(stream, frame.label);
            }
            if (flag.test(FrameFlag::command))
            {
                exchange_list<u8>(&exchange_layer_command, stream, frame.command);
            }
        }

        static auto exchange_sprite(WriteMemoryStream& stream, AnimationSprite const& sprite) -> void {
            if (check_version(k_version, {4}))
            {
                exchange_string_block<u16>(stream, sprite.name);
                if (check_version(k_version, {6}))
                {
                    exchange_null_block(stream, 2_size);
                }
                exchange_null_block(stream, 4_size); //TODO: need fix
            }
            exchange_number_fixed<u16>(stream, sprite.frame.size());
            if (check_version(k_version, {5}))
            {
                exchange_work_area(stream, sprite.work_area);
            }
            exchange_list<void>(&exchange_frame, stream, sprite.frame);
        }

        static auto exchange_main_sprite(WriteMemoryStream &stream, AnimationSprite const& main) -> void {
            if (check_version(k_version, {4})) {
                exchange_boolean(stream, true);
            }
            exchange_sprite(stream, main);
        }

        static auto exchange_animation(WriteMemoryStream& stream, SexyAnimation const &model) -> void {
            exchange_number_fixed<u8>(stream, model.frame_rate);
            exchange_floater_with_rate<ValueRate::size, int16_t>(stream, model.position.x);
            exchange_floater_with_rate<ValueRate::size, int16_t>(stream, model.position.y);
            exchange_floater_with_rate<ValueRate::size, int16_t>(stream, model.size.width);
            exchange_floater_with_rate<ValueRate::size, int16_t>(stream, model.size.height);
            exchange_list<u16>(&exchange_image, stream, model.image);
            exchange_list<u16>(&exchange_sprite, stream, model.sprite);
            exchange_main_sprite(stream, model.main_sprite);
        }
        */
    public:
        static auto process_whole(WriteMemoryStream& stream, SexyAnimation const &model) -> void
        {
            /*
            stream.u32(k_magic_identifier);
            k_version = model.version;
            assert_conditional(contain_version(k_version), fmt::format("{}", Language::get("popcap.animation.invalid_version")), "process_whole");
            exchange_number_fixed<u32>(stream, k_version);
            exchange_animation(stream, model);
            */
        }

        static auto process_fs(
           StringView const &source,
           StringView const &destination
        ) -> void
        {
            auto stream = WriteMemoryStream{};
            auto model = SexyAnimation{};
            FileSystem::read_json<SexyAnimation>(source, model);
            process_whole(stream, model);
            FileSystem::write_file(destination, stream.view());
        }
    };

}