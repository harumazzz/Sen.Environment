#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/model.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    struct Encode : Common
    {
    private:
        /*
        template <auto platform, typename TransformClass> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television> && is_class<TransformClass>
        static auto exchange_transform_value(WriteMemoryStream &stream, TransformClass const &value) -> void
        {
            exchange_number_fixed<float>(stream, value.x);
            exchange_number_fixed<float>(stream, value.y);
            exchange_number_fixed<float>(stream, value.kx);
            exchange_number_fixed<float>(stream, value.ky);
            exchange_number_fixed<float>(stream, value.sx);
            exchange_number_fixed<float>(stream, value.sy);
            exchange_number_fixed<float>(stream, value.f);
            exchange_number_fixed<float>(stream, value.a);
            constexpr auto ignored_size = sizeof(integer_or_platform<platform>);
            exchange_null_block(stream, ignored_size * 3);
            if constexpr (platform == VersionPlatform::television)
            {
                exchange_null_block(stream, ignored_size);
            }
        }

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static auto exchange_transform_list(WriteMemoryStream &stream, List<Transform> const &value_list) -> void
        {
            exchange_null_block(stream, get_transform_data_size<platform>());
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
                }
                if constexpr (platform == VersionPlatform::mobile32 || platform == VersionPlatform::mobile64)
                {
                    auto &transform = std::get<TransformMobile>(value);
                    exchange_number_fixed<u32>(stream, transform.image);
                    exchange_string_block<u32>(stream, transform.font);
                    exchange_string_block<u32>(stream, transform.text);
                }
                if constexpr (platform == VersionPlatform::television)
                {
                    auto &transform = std::get<TransformTelevision>(value);
                    exchange_string_block<u32>(stream, transform.image_path);
                    exchange_string_block<u32>(stream, transform.image_another);
                    exchange_string_block<u32>(stream, transform.image_path_another);
                    exchange_string_block<u32>(stream, transform.font);
                    exchange_string_block<u32>(stream, transform.text);
                }
            }
        }

        template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
        static auto exchange_track_list(WriteMemoryStream &stream, List<Track> const &value_list) -> void
        {
            exchange_null_block(stream, get_transform_data_size<platform>());
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
        static auto exchange_animation(WriteMemoryStream &stream, ReanimInfo const &model) -> void
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
        static auto process_whole(WriteMemoryStream &stream, ReanimInfo const &model, VersionPlatform const &platform) -> void
        {/*
            switch (platform)
            {
            case VersionPlatform::desktop:
                {
                    stream.u32(k_desktop_magic_identifier);
                    exchange_animation<VersionPlatform::desktop>(stream, model);
                    break;
                }
            case VersionPlatform::mobile32:
                {
                    stream.u32(k_mobile32_magic_identifier);
                    exchange_animation<VersionPlatform::mobile32>(stream, model);
                    break;
                }
            case VersionPlatform::mobile64:
                {
                    stream.u32(k_mobile64_magic_identifier);
                    exchange_animation<VersionPlatform::mobile64>(stream, model);
                    break;
                }
            case VersionPlatform::television:
                {
                    stream.u32(k_television_magic_identifier);
                    exchange_animation<VersionPlatform::television>(stream, model);
                    break;
                }
                default:
                {
                    assert_conditional(false, "Invalid platform", "get_k_magic_identifier"); //TODO
                }
            }
            */
        }

        static auto process_fs(StringView const &source, StringView const &destination, VersionPlatform const &platform) -> void
        {
            auto stream = WriteMemoryStream{};
            auto model = ReanimInfo{};
            FileSystem::read_json(source, model);
            process_whole(stream, model, platform);
            FileSystem::write_file(destination, stream.view());
        }
    };

}