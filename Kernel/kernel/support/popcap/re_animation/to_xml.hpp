#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/model.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{

    struct ToXML : Common
    {
    private:
        template <typename Type> requires (!std::is_void_v<Type>)
        static auto exchange_child_value(XMLNode &node, Type const &value) -> void
        {
            if constexpr (std::is_same_v<Type, String> || std::is_same_v<Type, std::string_view>) {
                node.append_child(pugi::node_pcdata).set_value(value.begin());
            }
            else {
                node.append_child(pugi::node_pcdata).set_value(fmt::format("{}", value).c_str());
            }
        }

        static auto exchange_child_node(XMLNode &node, const std::string_view name) -> XMLNode
        {
            return node.append_child(name.data());
        }

        template <typename Type> requires (!std::is_void_v<Type>)
        static auto exchange_node_value(Optional<Type> const &value, XMLNode &node, const std::string_view name) -> void
        {
            if (value.has_value())
            {
                auto node_child = exchange_child_node(node, name);
                exchange_child_value(node_child, *value);
            }
        }

        template <auto platform, is_class TransformClass>
        static auto exchange_transform_value(XMLNode &node, TransformClass const &value) -> void
        {
            exchange_node_value(value.x, node, k_x);
            exchange_node_value(value.y, node, k_y);
            exchange_node_value(value.kx, node, k_kx);
            exchange_node_value(value.ky, node, k_ky);
            exchange_node_value(value.sx, node, k_sx);
            exchange_node_value(value.sy, node, k_sy);
            exchange_node_value(value.f, node, k_f);
            exchange_node_value(value.a, node, k_a);
            exchange_node_value(value.image, node, k_i);
            if constexpr (platform == television)
            {
                exchange_node_value(value.image_path, node, k_resource);
                exchange_node_value(value.image_another, node, k_i2);
                exchange_node_value(value.image_path_another, node, k_resource2);
            }
            exchange_node_value(value.font, node, k_font);
            exchange_node_value(value.text, node, k_text);
        }

        template <auto platform>
        static auto exchange_transform(List<Transform> const &value_list, XMLNode &node) -> void
        {
            for (const auto &value : value_list)
            {
                auto transform_child = exchange_child_node(node, k_t);
                if constexpr (platform == desktop)
                {
                    auto &transform = std::get<TransformDesktop>(value);
                    exchange_transform_value<platform>(transform_child, transform);
                }
                if constexpr (platform == mobile32 || platform == mobile64)
                {
                    auto &transform = std::get<TransformMobile>(value);
                    exchange_transform_value<platform>(transform_child, transform);
                }
                if constexpr (platform == television)
                {
                    auto &transform = std::get<TransformTelevision>(value);
                    exchange_transform_value<platform>(transform_child, transform);
                }
            }
        }

        template <auto platform>
        static auto exchange_track(List<Track> const &value_list, XMLNode &value) -> void
        {
            for (const auto & track : value_list)
            {
                auto track_child = exchange_child_node(value, k_track);
                auto name = exchange_child_node(track_child, k_name);
                exchange_child_value(name, track.name);
                exchange_transform<platform>(track.transform, track_child);
            }
        }

        template <auto platform> requires is_between_v<platform, desktop, television>
        static auto exchange_animation(ReanimInfo const& model, XMLNode &value) -> void
        {
            auto fps_child = exchange_child_node(value, k_fps);
            exchange_child_value(fps_child, model.frame_rate);
            exchange_track<platform>(model.track, value);
        }

    public:
        static constexpr auto process_whole(ReanimInfo const& model, XMLDocument &value, VersionPlatform const &platform) -> void
        {
            switch (platform)
            {
            case desktop:
                {
                    exchange_animation<desktop>(model, value);
                    break;
                }
            case mobile32:
                {
                    exchange_animation<mobile32>(model, value);
                    break;
                }
            case mobile64:
                {
                    exchange_animation<mobile64>(model, value);
                    break;
                }
            case television:
                {
                    exchange_animation<television>(model, value);
                    break;
                }
            default:
                {
                    assert_conditional(false, "Invalid platform", "process_whole"); //TODO
                }
            }
        }

        static auto process_fs(StringView const &source, StringView const &destination, VersionPlatform const &platform) -> void
        {
            auto model = ReanimInfo{};
            FileSystem::read_json(source, model);
            auto doc = XMLDocument{};
            process_whole(model, doc, platform);
            FileSystem::write_xml(destination, doc, pugi::format_indent | pugi::format_no_declaration);
        }
    };

}