#pragma once

#include "kernel/support/popcap/character_font_widget_2/definition.hpp"

namespace Sen::Kernel::Support::PopCap::CharacterFontWidget2
{

    struct Decode {

    public:

        constexpr Decode(

        ) = default;

        constexpr ~Decode(

        ) = default;

        inline static auto process(
            DataStreamView& stream,
            CharacterFontWidget2& data
        ) -> void
        {
            process_unknown(stream, data);
            process_ascent(stream, data);
            process_character(stream, data);
            process_layer(stream, data);
            process_source(stream, data);
            process_tag(stream, data);
            process_scale(stream, data);
        }

        inline static auto process_fs(
            std::string_view source,
            std::string_view destination
        ) -> void
        {
            auto stream = DataStreamView{ source };
            auto result = CharacterFontWidget2{};
            Decode::process(stream, result);
            FileSystem::write_json(destination, result);
        }

    protected:

        inline static auto process_unknown(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            for (auto i : Range<int>(16)) {
                data.unknown[i] = stream.readUint8();
            }
        }

        inline static auto process_ascent(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            data.ascent = stream.readInt32();
            data.ascent_padding = stream.readInt32();
            data.height = stream.readInt32();
            data.line_sepacing_offset = stream.readInt32();
            data.initialized = stream.readBoolean();
            data.default_point_size = stream.readInt32();
        }

        inline static auto process_character(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            auto character_count = stream.readUint32();
            data.character.reserve(static_cast<size_t>(character_count));
            for (auto i : Range<uint32_t>(character_count)) {
                data.character.emplace_back(CharacterItem{
                    stream.readCharByInt16(),
                    stream.readCharByInt16()
                });
            }
        }

        inline static auto process_layer(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            auto layer_count = stream.readUint32();
            data.layer.reserve(static_cast<size_t>(layer_count));
            for (auto i : Range<uint32_t>(layer_count)) {
                process_single_layer(stream, data);
            }
        }

        inline static auto process_single_layer(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            auto name = stream.readStringByInt32();
            auto tag_require = List<std::string>{};
            auto tag_exclude = List<std::string>{};
            auto kerning = List<FontKerning>{};
            auto characters = List<FontCharacter>{};
            process_tags(stream, tag_require);
            process_tags(stream, tag_exclude);
            process_kerning(stream, kerning);
            process_font_characters(stream, characters);
            data.layer.emplace_back(FontLayer{
                name,
                std::move(tag_require),
                std::move(tag_exclude),
                std::move(kerning),
                std::move(characters),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readStringByInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
                stream.readInt32(),
            });
        }

        inline static auto process_tags(
            DataStreamView& stream, 
            List<std::string>& tags
        ) -> void {
            auto tag_count = stream.readUint32();
            tags.reserve(static_cast<size_t>(tag_count));
            for (auto i : Range<uint32_t>(tag_count)) {
                tags.emplace_back(stream.readStringByInt32());
            }
        }

        inline static auto process_kerning(
            DataStreamView& stream, 
            List<FontKerning>& kerning
        ) -> void {
            auto kerning_count = stream.readUint32();
            kerning.reserve(static_cast<size_t>(kerning_count));
            for (auto i : Range<uint32_t>(kerning_count)) {
                kerning.emplace_back(FontKerning{
                    stream.readCharByInt16(),
                    static_cast<unsigned short>(stream.readUint16())
                });
            }
        }

        inline static auto process_font_characters(
            DataStreamView& stream, 
            List<FontCharacter>& characters
        ) -> void {
            auto character_count = stream.readUint32();
            characters.reserve(static_cast<size_t>(character_count));
            for (auto k : Range<uint32_t>(character_count)) {
                characters.emplace_back(FontCharacter{
                    stream.readCharByInt16(),
                    stream.readInt32(),
                    stream.readInt32(),
                    stream.readInt32(),
                    stream.readInt32(),
                    stream.readInt32(),
                    stream.readInt32(),
                    stream.readUint16(),
                    stream.readUint16(),
                    stream.readInt32(),
                    stream.readInt32()
                });
            }
        }

        inline static auto process_source(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            data.source_file = stream.readStringByInt32();
            data.error_header = stream.readStringByInt32();
            data.point_size = stream.readInt32();
        }

        inline static auto process_tag(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void 
        {
            auto tag_count = stream.readUint32();
            data.tag.reserve(static_cast<size_t>(tag_count));
            for (auto i : Range<uint32_t>(tag_count)) {
                data.tag.emplace_back(stream.readStringByInt32());
            }
        }

        inline static auto process_scale(
            DataStreamView& stream, 
            CharacterFontWidget2& data
        ) -> void
        {
            data.scale = stream.readDouble();
            data.force_scaled_image_white = stream.readBoolean();
            data.activate_all_layer = stream.readBoolean();
        }
    };

}