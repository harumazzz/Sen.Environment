#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/model.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"
#include "kernel/support/popcap/zlib/uncompress.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    struct Decode : Common
    {
    private:
        /*
        template <auto platform, typename TransformClass> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television> && is_class<TransformClass>
        static auto exchange_transform_value(ReadMemoryStream &stream, TransformClass &value) -> void
        {
            exchange_number_fixed<float>(stream, value.x);
            exchange_number_fixed<float>(stream, value.y);
            exchange_number_fixed<float>(stream, value.kx);
            exchange_number_fixed<float>(stream, value.ky);
            exchange_number_fixed<float>(stream, value.sx);
            exchange_number_fixed<float>(stream, value.sy);
            exchange_number_fixed<float>(stream, value.f);
            exchange_number_fixed<float>(stream, value.a);
            compare_value_to_reset(value.x, k_transform_float_end);
            compare_value_to_reset(value.y, k_transform_float_end);
            compare_value_to_reset(value.kx, k_transform_float_end);
            compare_value_to_reset(value.ky, k_transform_float_end);
            compare_value_to_reset(value.sx, k_transform_float_end);
            compare_value_to_reset(value.sy, k_transform_float_end);
            compare_value_to_reset(value.f, k_transform_float_end);
            compare_value_to_reset(value.a, k_transform_float_end);
            constexpr auto ignored_size = sizeof(integer_or_platform<platform>);
            exchange_null_block(stream, ignored_size * 3);
            if constexpr (platform == VersionPlatform::television)
            {
                exchange_null_block(stream, ignored_size);
            }
        }

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static auto exchange_transform_list(ReadMemoryStream &stream, List<Transform> &value_list) -> void
        {
            exchange_raw_constant(stream, static_cast<u32>(get_transform_data_size<platform>()));
            for (auto & value : value_list)
            {
                if constexpr (platform == VersionPlatform::desktop)
                {
                    auto &transform = std::get<TransformDesktop>(value);
                    exchange_transform_value<platform>(stream, transform);
                }
                if constexpr (platform == VersionPlatform::mobile32 || platform == VersionPlatform::mobile64)
                {
                    auto &transform = std::get<TransformMobile>(value);
                    exchange_transform_value<platform>(stream, transform);
                }
                if constexpr (platform == VersionPlatform::television)
                {
                    auto &transform = std::get<TransformTelevision>(value);
                    exchange_transform_value<platform>(stream, transform);
                }
            }
            for (auto & value : value_list)
            {
                if constexpr (platform == VersionPlatform::desktop)
                {
                    auto &transform = std::get<TransformDesktop>(value);
                    exchange_string_block<u32>(stream, transform.image);
                    exchange_string_block<u32>(stream, transform.font);
                    exchange_string_block<u32>(stream, transform.text);
                    compare_value_to_reset(transform.image, k_string_empty);
                    compare_value_to_reset(transform.font, k_string_empty);
                    compare_value_to_reset(transform.text, k_string_empty);
                }
                if constexpr (platform == VersionPlatform::mobile32 || platform == VersionPlatform::mobile64)
                {
                    auto &transform = std::get<TransformMobile>(value);
                    exchange_number_fixed<u32>(stream, transform.image);
                    exchange_string_block<u32>(stream, transform.font);
                    exchange_string_block<u32>(stream, transform.text);
                    compare_value_to_reset(transform.image, k_image_null);
                    compare_value_to_reset(transform.font, k_string_empty);
                    compare_value_to_reset(transform.text, k_string_empty);
                }
                if constexpr (platform == VersionPlatform::television)
                {
                    auto &transform = std::get<TransformTelevision>(value);
                    exchange_string_block<u32>(stream, transform.image_path);
                    exchange_string_block<u32>(stream, transform.image_another);
                    exchange_string_block<u32>(stream, transform.image_path_another);
                    exchange_string_block<u32>(stream, transform.font);
                    exchange_string_block<u32>(stream, transform.text);
                    compare_value_to_reset(transform.image, k_string_empty);
                    compare_value_to_reset(transform.image_another, k_string_empty);
                    compare_value_to_reset(transform.image_path_another, k_string_empty);
                    compare_value_to_reset(transform.font, k_string_empty);
                    compare_value_to_reset(transform.text, k_string_empty);
                }
            }
        }

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static auto exchange_track_list(ReadMemoryStream &stream, List<Track> &value_list) -> void
        {
            exchange_raw_constant(stream, static_cast<u32>(get_track_data_size<platform>()));
            for (auto & value : value_list)
            {
                constexpr auto ignored_size = sizeof(integer_or_platform<platform>);
                exchange_null_block(stream, ignored_size * 2);
                if constexpr (platform == VersionPlatform::mobile32 || platform == VersionPlatform::mobile64)
                {
                    exchange_null_block(stream, ignored_size);
                }
                exchange_list_size<u32>(stream, value.transform);
                if constexpr (platform == VersionPlatform::television)
                {
                    exchange_null_block(stream, ignored_size);
                }
            }

            for (auto & value : value_list) {
                exchange_string_block<u32>(stream, value.name);
                exchange_transform_list<platform>(stream, value.transform);
            }
        }

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static auto exchange_animation(ReadMemoryStream &stream, ReanimInfo &model) -> void
        {
            constexpr auto ignored_size = sizeof(integer_or_platform<platform>);
            exchange_null_block(stream, ignored_size);
            exchange_list_size<u32>(stream, model.track);
            exchange_number_fixed<float>(stream, model.frame_rate);
            exchange_null_block(stream, ignored_size);
            exchange_track_list<platform>(stream, model.track);
        }
        */
    public:
        /*
        static auto process_whole(ReadMemoryStream &stream, ReanimInfo &model, VersionPlatform const &platform) -> void
        {
            switch (platform)
            {
            case VersionPlatform::desktop:
                {
                    assert_conditional(stream.u32() == k_desktop_magic_identifier, fmt::format("{}", Language::get("popcap.reanim.decode.invalid_pc_reanim_magic")), "process_whole");
                    exchange_animation<VersionPlatform::desktop>(stream, model);
                    break;
                }
            case VersionPlatform::mobile32:
                {
                    assert_conditional(stream.u32() == k_mobile32_magic_identifier, fmt::format("{}", Language::get("popcap.reanim.decode.invalid_phone_32_reanim_magic")), "process_whole");
                    exchange_animation<VersionPlatform::mobile32>(stream, model);
                    break;
                }
            case VersionPlatform::mobile64:
                {
                    assert_conditional(stream.u32() == k_mobile64_magic_identifier, fmt::format("{}", Language::get("popcap.reanim.decode.invalid_phone_64_reanim_magic")), "process_whole");
                    exchange_animation<VersionPlatform::mobile64>(stream, model);
                    break;
                }
            case VersionPlatform::television:
                {
                    assert_conditional(stream.u32() == k_television_magic_identifier, fmt::format("{}", Language::get("popcap.reanim.decode.invalid_tv_reanim_magic")), "process_whole");
                    exchange_animation<VersionPlatform::television>(stream, model);
                    break;
                }
                default:
                {
                    assert_conditional(false, "Invalid platform", "get_k_magic_identifier"); //TODO
                }
            }
        }
        */
        static auto process_fs(StringView const &source, StringView const &destination, VersionPlatform const &platform) -> void
        {
            auto stream = ReadMemoryStream{source};
            auto model = ReanimInfo{};
           // process_whole(stream, model, platform);
            FileSystem::write_json(destination, model);
        }
    };
}

