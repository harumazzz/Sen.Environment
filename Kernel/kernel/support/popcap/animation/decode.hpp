#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/animation/model.hpp"
#include "kernel/support/popcap/animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::Animation
{
    struct Decode : Common
    {
    private:
        /*
        template<typename RawShort, typename RawLong, typename Container> requires is_integer<RawShort> && is_integer<RawLong> && is_list_v<Container>
        static auto exchange_integer_variant_and_resize(ReadMemoryStream& stream, Container &list) -> void
        {
            constexpr auto value_short_maximum = value_max<RawShort>();
            auto value_short = RawShort{};
            auto value_long = RawLong{};
            exchange_number_fixed<RawShort>(stream, value_short);
            if (value_short != value_short_maximum)
            {
                value_long = static_cast<RawLong>(value_short);
            }
            else
            {
                exchange_number_fixed<RawLong>(stream, value_long);
            }
            list.resize(value_long);
        }

        template<typename RawShort, typename RawLong, auto flag_count> requires is_integer<RawShort> && is_integer<RawLong>
        static auto exchange_integer_variant_with_flag(ReadMemoryStream& stream, u32 &value, std::bitset<flag_count> &flag) -> void
        {
            constexpr auto value_short_bit_count = type_bit_count<RawShort>() - static_cast<size_t>(flag_count);
            constexpr auto value_short_maximum = static_cast<RawShort>(value_max<RawShort>() >> flag_count);
            auto value_short_with_flag = RawShort{};
            auto value_long = RawLong{};
            exchange_number_fixed<RawShort>(stream, value_short_with_flag);
            auto value_short = clip_bit(value_short_with_flag, k_begin_index, value_short_bit_count);
            auto value_flag = clip_bit(value_short_with_flag, value_short_bit_count, static_cast<size_t>(flag_count));
            if (value_short != value_short_maximum)
            {
                value_long = static_cast<RawLong>(value_short);
            }
            else
            {
                exchange_number_fixed<RawLong>(stream, value_long);
            }
            value = static_cast<u32>(value_long);
            flag = std::bitset<flag_count>(static_cast<u64>(value_flag));
        }

        static auto exchange_image(ReadMemoryStream& stream, AnimationImage& image) -> void {
            exchange_mutil_by_string_block<u16>(stream, vertical_bar, image.path, image.id);
            if (check_version(k_version, {4})) {
                exchange_number_fixed<u16>(stream, image.dimension.width);
                exchange_number_fixed<u16>(stream, image.dimension.height);
            }
            if (check_version(k_version, {1, 2})) {
                image.transform.resize(3_size);
                exchange_floater_with_rate<ValueRate::angle, i16>(stream, image.transform[0]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[1]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[2]);
            }
            else {
                image.transform.resize(6_size);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[0]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[1]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[2]);
                exchange_floater_with_rate<ValueRate::matrix_exact, i32>(stream, image.transform[3]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[4]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, image.transform[5]);
            }
        }

        static auto exchange_work_area(ReadMemoryStream& stream, AnimationWorkArea& work_area) -> void
        {
            exchange_number_fixed<u16>(stream, work_area.start);
            exchange_number_fixed<u16>(stream, work_area.duration); //TODO: need test
        }

        static auto exchange_layer_remove(ReadMemoryStream& stream, u32& value) -> void
        {
            auto flag = std::bitset<LayerRemoveFlag::k_count>{};
            exchange_integer_variant_with_flag<u16, u32>(stream, value, flag);
        }

        static auto exchange_layer_append(ReadMemoryStream& stream, AnimationAppend& append) -> void
        {
            auto flag = std::bitset<LayerAppendFlag::k_count>{};
            exchange_integer_variant_with_flag<u16, u32>(stream, append.index, flag);
            exchange_number_fixed<u8>(stream, append.resource);
            if (check_version(k_version, {6}) && static_cast<u8>(append.resource) == value_max<u8>())
            {
                exchange_number_fixed<u16>(stream, append.resource);
            }
            if (flag.test(LayerAppendFlag::sprite))
            {
                append.sprite = true;
            }
            if (flag.test(LayerAppendFlag::additive))
            {
                append.additive = true;
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

        static auto exchange_layer_change(ReadMemoryStream& stream, AnimationChange &change) -> void
        {
            auto flag = std::bitset<LayerChangeFlag::k_count>{};
            exchange_integer_variant_with_flag<u16, u32>(stream, change.index, flag);
            if (!(flag.test(LayerChangeFlag::rotate) && flag.test(LayerChangeFlag::matrix)))
            {
                change.transform.resize(2_size);
            }
            if (flag.test(LayerChangeFlag::rotate))
            {
                assert_conditional(!flag.test(LayerChangeFlag::matrix), "Flag matrix cannot appear in flag rotate", "exchange_layer_change"); //TODO: add locale
                change.transform.resize(3_size);
                exchange_floater_with_rate<ValueRate::angle, i16>(stream, change.transform[0]);
            }
            if (flag.test(LayerChangeFlag::matrix))
            {
                assert_conditional(!flag.test(LayerChangeFlag::rotate), "Flag rotate cannot appear in flag matrix", "exchange_layer_change"); //TODO: add locale
                change.transform.resize(6_size);
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
                change.source_rectangle.emplace();
                auto& source_rectangle = change.source_rectangle.value();
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[0]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[1]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[2]);
                exchange_floater_with_rate<ValueRate::size, i16>(stream, source_rectangle[3]);
            }
            if (flag.test(LayerChangeFlag::color))
            {
                change.color.emplace();
                auto& color = change.color.value();
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

        static auto exchange_layer_command(ReadMemoryStream& stream, AnimationCommand &command) -> void
        {
            exchange_string_block<u16>(stream, command.command);
            exchange_string_block<u16>(stream, command.argument);
        }

        static auto exchange_frame(ReadMemoryStream& stream, AnimationFrame &frame) -> void
        {
            const auto flag = std::bitset<FrameFlag::k_count>{stream.u8()};
            if (flag.test(FrameFlag::remove))
            {
                exchange_integer_variant_and_resize<u8, u16>(stream, frame.remove);
                exchange_list<void>(&exchange_layer_remove, stream, frame.remove);
            }
            if (flag.test(FrameFlag::append))
            {
                exchange_integer_variant_and_resize<u8, u16>(stream, frame.append);
                exchange_list<void>(&exchange_layer_append, stream, frame.append);
            }
            if (flag.test(FrameFlag::change))
            {
                exchange_integer_variant_and_resize<u8, u16>(stream, frame.change);
                exchange_list<void>(&exchange_layer_change, stream, frame.change);
            }
            if (flag.test(FrameFlag::label))
            {
                exchange_string_block<u16>(stream, frame.label);
            }
            if (flag.test(FrameFlag::stop))
            {
                frame.stop = true;
            }
            if (flag.test(FrameFlag::command))
            {
                exchange_list<u8>(&exchange_layer_command, stream, frame.command);
            }
        }

        static auto exchange_sprite(ReadMemoryStream& stream, AnimationSprite& sprite) -> void {
            if (check_version(k_version, {4}))
            {
                exchange_string_block<u16>(stream, sprite.name);
                if (check_version(k_version, {6}))
                {
                    auto description = String{};
                    exchange_string_block<u16>(stream, description);
                }
                exchange_null_block(stream, 4_size);
            }
            sprite.frame.resize(stream.u16());
            if (check_version(k_version, {5}))
            {
                exchange_work_area(stream, sprite.work_area);
            }
            exchange_list<void>(&exchange_frame, stream, sprite.frame);
        }

        static auto exchange_main_sprite(ReadMemoryStream &stream, AnimationSprite& main) -> void {
            if (check_version(k_version, {1, 5}) || stream.boolean()) {
                exchange_sprite(stream, main);
            }
        }

        static auto exchange_animation(ReadMemoryStream& stream, SexyAnimation &model) -> void {
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
        static auto process_whole(ReadMemoryStream& stream, SexyAnimation &model) -> void
        {
            /*
            assert_conditional(stream.u32() == k_magic_identifier, fmt::format("{}", Language::get("popcap.animation.invalid_magic")), "process_whole");
            exchange_number_fixed<u32>(stream, k_version);
            assert_conditional(contain_version(k_version), fmt::format("{}", Language::get("popcap.animation.invalid_version")), "process_whole");
            model.version = k_version;
            exchange_animation(stream, model);
            */
        }

        static auto process_fs(
           StringView const &source,
           StringView const &destination
        ) -> void
        {
            auto stream = ReadMemoryStream{source};
            auto model = SexyAnimation{};
            process_whole(stream, model);
            FileSystem::write_json(destination, model);
        }
    };
}