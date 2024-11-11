#pragma once

#include "kernel/definition/utility.hpp"
#include "kernel/support/popcap/re_animation/definition.hpp"
#include "kernel/support/popcap/re_animation/common.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation
{
    using namespace Definition;

    class FromXML : Common
    {
    private:
        using XMLDocument = tinyxml2::XMLDocument;

        using XMLElement = tinyxml2::XMLElement;

    protected:
        inline auto convert(
            ReanimInfo &reanim,
            XMLDocument *document) -> void
        {
            auto fps = document->FirstChildElement("fps");
            assert_conditional(fps != nullptr, fmt::format("{}", Language::get("popcap.reanim.decode.invalid_fps_chuck")), "convert");
            reanim.fps = Converter::to_float32(fps->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"fps"}));
            auto do_scale = document->FirstChildElement("doScale");
            if (do_scale != nullptr)
            {
                reanim.do_scale = static_cast<int8_t>(Converter::to_int32(do_scale->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"do_scale"})));
            }
            for (auto track = document->FirstChildElement("track"); track != nullptr; track = track->NextSiblingElement("track"))
            {
                auto track_info = ReanimTrack{};
                auto name = track->FirstChildElement("name");
                if (name != nullptr)
                {
                    track_info.name = std::string{name->FirstChild()->Value()};
                }
                for (auto transform = track->FirstChildElement("t"); transform != nullptr; transform = transform->NextSiblingElement("t"))
                {
                    auto transform_info = ReanimTransform{};
                    auto x = transform->FirstChildElement("x");
                    if (x != nullptr)
                    {
                        transform_info.x = Converter::to_float32(x->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"x"}));
                    }
                    auto y = transform->FirstChildElement("y");
                    if (y != nullptr)
                    {
                        transform_info.y = Converter::to_float32(y->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"y"}));
                    }
                    auto kx = transform->FirstChildElement("kx");
                    if (kx != nullptr)
                    {
                        transform_info.kx = Converter::to_float32(kx->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"kx"}));
                    }
                    auto ky = transform->FirstChildElement("ky");
                    if (ky != nullptr)
                    {
                        transform_info.ky = Converter::to_float32(ky->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"ky"}));
                    }
                    auto sx = transform->FirstChildElement("sx");
                    if (sx != nullptr)
                    {
                        transform_info.sx = Converter::to_float32(sx->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"sx"}));
                    }
                    auto sy = transform->FirstChildElement("sy");
                    if (sy != nullptr)
                    {
                        transform_info.sy = Converter::to_float32(sy->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"sy"}));
                    }
                    auto a = transform->FirstChildElement("a");
                    if (a != nullptr)
                    {
                        transform_info.x = Converter::to_float32(a->FirstChild()->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"a"}));
                    }
                    auto i = transform->FirstChildElement("i");
                    if (i != nullptr)
                    {
                        transform_info.i = std::string{i->FirstChild()->Value()};
                    }
                    auto resource = transform->FirstChildElement("resource");
                    if (resource != nullptr)
                    {
                        transform_info.resource = std::string{resource->FirstChild()->Value()};
                    }
                    auto i2 = transform->FirstChildElement("i2");
                    if (i2 != nullptr)
                    {
                        transform_info.i2 = std::string{i2->FirstChild()->Value()};
                    }
                    auto resource2 = transform->FirstChildElement("resource2");
                    if (resource2 != nullptr)
                    {
                        transform_info.resource2 = std::string{resource2->FirstChild()->Value()};
                    }
                    auto font = transform->FirstChildElement("font");
                    if (font != nullptr)
                    {
                        transform_info.font = std::string{font->FirstChild()->Value()};
                    }
                    auto text = transform->FirstChildElement("text");
                    if (text != nullptr)
                    {
                        transform_info.text = std::string{text->FirstChild()->Value()};
                    }
                    track_info.transforms.emplace_back(transform_info);
                }
                reanim.tracks.emplace_back(track_info);
            }
            return;
        }

    public:
        explicit FromXML(

        ) = default;

        ~FromXML(

            ) = default;

        inline auto process(
            ReanimInfo &reanim,
            XMLDocument &document) -> void
        {
            convert(reanim, &document);
            return;
        }

        inline static auto process_fs(
            std::string_view source,
            std::string_view destination
        ) -> void
        {
            auto from_xml = FromXML{};
            auto definition = ReanimInfo{};
            auto document = XMLDocument{};
            FileSystem::read_xml(source, &document);
            from_xml.process(definition, document);
            FileSystem::write_json(destination, definition);
            return;
        }
    };

}