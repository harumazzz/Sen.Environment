#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/re_animation/model.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{

    // struct FromXML : Common
    // {
    // private:
    //     template <typename Type> requires (!std::is_void_v<Type>) && (std::is_same_v<Type, String> || std::is_arithmetic_v<Type>)
    //     static auto exchange_child_value(XMLNode const &node, Type &value) -> bool
    //     {
    //         if (const auto temporary = StringHelper::make_string_view(node.child_value()); temporary.empty())
    //         {
    //             return false;
    //         }
    //         auto new_string = StringHelper::make_string(node.child_value());
    //         if constexpr (std::is_same_v<Type, String>) {
    //             value.take_ownership(new_string);
    //         }
    //         else {
    //             if constexpr (std::is_integral_v<Type>)
    //             {
    //                 value = StringParser::to_integer<Type>(new_string, "Failed to convert to integer"_sv);
    //             }
    //             else if constexpr (std::is_floating_point_v<Type>)
    //             {
    //                 value = StringParser::to_float<Type>(new_string, "Failed to convert to float"_sv);
    //             }
    //         }
    //         return true;
    //     }

    //     template <auto can_null> requires std::is_same_v<type_of<can_null>, bool>
    //     static auto exchange_child_node(XMLNode const &node, const std::string_view name) -> XMLNode
    //     {
    //         const auto child = node.child(name.data());
    //         if constexpr (!can_null)
    //         {
    //             assert_conditional(child != nullptr, fmt::format("Failed get child node: {}", name), "exchange_child_node");
    //         }
    //         return child;
    //     }

    //     template <typename Type> requires (!std::is_void_v<Type>)
    //     static auto exchange_node_value(Optional<Type> &value, XMLNode const &node, const std::string_view name) -> void
    //     {
    //         auto node_child = exchange_child_node<true>(node, name);
    //         auto p_value = Type{};
    //         if (exchange_child_value(node_child, p_value))
    //         {
    //             value = p_value;
    //         }
    //     }

    //     template <auto platform, typename TransformClass> requires (std::is_same_v<type_of<platform>, VersionPlatform> && is_class<TransformClass>)
    //     static auto exchange_transform_value(XMLNode const &node, TransformClass &value) -> void
    //     {
    //         exchange_node_value(value.x, node, k_x);
    //         exchange_node_value(value.y, node, k_y);
    //         exchange_node_value(value.kx, node, k_kx);
    //         exchange_node_value(value.ky, node, k_ky);
    //         exchange_node_value(value.sx, node, k_sx);
    //         exchange_node_value(value.sy, node, k_sy);
    //         exchange_node_value(value.f, node, k_f);
    //         exchange_node_value(value.a, node, k_a);
    //         exchange_node_value(value.image, node, k_i);
    //         if constexpr (platform == VersionPlatform::television)
    //         {
    //             exchange_node_value(value.image_path, node, k_resource);
    //             exchange_node_value(value.image_another, node, k_i2);
    //             exchange_node_value(value.image_path_another, node, k_resource2);
    //         }
    //         exchange_node_value(value.font, node, k_font);
    //         exchange_node_value(value.text, node, k_text);
    //     }

    //     template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
    //     static auto exchange_transform(List<Transform> &value_list, XMLNode const &node) -> void
    //     {
    //         for (auto node_transform = node.child(k_t); node_transform != nullptr; node_transform = node_transform.next_sibling())
    //         {
    //             if constexpr (platform == VersionPlatform::desktop)
    //             {
    //                 auto make_value = [&] () -> TransformDesktop {
    //                     auto transform = TransformDesktop{};
    //                     exchange_transform_value<platform>(node_transform, transform);
    //                     return transform;
    //                 };
    //                 value_list.append(make_value());
    //             }
    //             if constexpr (platform == VersionPlatform::mobile32 || platform == VersionPlatform::mobile64)
    //             {
    //                 auto make_value = [&] () -> TransformMobile {
    //                     auto transform = TransformMobile{};
    //                     exchange_transform_value<platform>(node_transform, transform);
    //                     return transform;
    //                 };
    //                 value_list.append(make_value());
    //             }
    //             if constexpr (platform == VersionPlatform::television)
    //             {
    //                 auto make_value = [&] () -> TransformTelevision {
    //                     auto transform = TransformTelevision{};
    //                     exchange_transform_value<platform>(node_transform, transform);
    //                     return transform;
    //                 };
    //                 value_list.append(make_value());
    //             }
    //         }
    //     }

    //     template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
    //     static auto exchange_track(List<Track> &value_list, XMLNode const &node) -> void
    //     {
    //        // const auto track_child = exchange_child_node(node, "track"_sv);
    //         for (auto node_track = node.child(k_track); node_track != nullptr; node_track = node_track.next_sibling())
    //         {
    //             auto make_track = [&]() -> Track {
    //                 auto track = Track{};
    //                 auto name = exchange_child_node<false>(node_track, k_name);
    //                 exchange_child_value(name, track.name);
    //                 exchange_transform<platform>(track.transform, node_track);
    //                 return track;
    //             };
    //             value_list.append(make_track());
    //         }
    //     }

    //     template <auto platform> requires is_between_v<platform, VersionPlatform::desktop, VersionPlatform::television>
    //     static auto exchange_animation(ReanimInfo & model, XMLNode const &value) -> void
    //     {
    //         const auto fps_child = exchange_child_node<false>(value, k_fps);
    //         exchange_child_value(fps_child, model.frame_rate);
    //         exchange_track<platform>(model.track, value);
    //     }

    // public:
    //     static constexpr auto process_whole(ReanimInfo & model, XMLDocument const &value, VersionPlatform const &platform) -> void
    //     {
    //         switch (platform)
    //         {
    //         case VersionPlatform::desktop:
    //             {
    //                 exchange_animation<VersionPlatform::desktop>(model, value);
    //                 break;
    //             }
    //         case VersionPlatform::mobile32:
    //             {
    //                 exchange_animation<VersionPlatform::mobile32>(model, value);
    //                 break;
    //             }
    //         case VersionPlatform::mobile64:
    //             {
    //                 exchange_animation<VersionPlatform::mobile64>(model, value);
    //                 break;
    //             }
    //         case VersionPlatform::television:
    //             {
    //                 exchange_animation<VersionPlatform::television>(model, value);
    //                 break;
    //             }
    //         default:
    //             {
    //                 assert_conditional(false, "Invalid platform", "process_whole"); //TODO
    //             }
    //         }
    //     }

    //     static auto process_fs(StringView const &source, StringView const &destination, VersionPlatform const &platform) -> void
    //     {
    //         auto model = ReanimInfo{};
    //         auto doc = XMLDocument{};
    //         FileSystem::read_xml(source, doc);
    //         process_whole(model, doc, platform);
    //         FileSystem::write_json(destination, model);
    //     }
    // };

}