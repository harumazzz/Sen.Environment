#pragma once

#include "kernel/support/popcap/character_font_widget_2/definition.hpp"

namespace Sen::Kernel::Support::PopCap::CharacterFontWidget2
{

	#pragma region encode
	struct Encode {

		constexpr explicit Encode(

		) = default;

		constexpr ~Encode(

		) = default;

		inline static auto process_unknown(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			for (auto i : Range<int>(data.unknown.size())) {
				stream.writeUint8(data.unknown[i]);
			}
		}

		inline static auto process_ascend(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			stream.writeInt32(data.ascent);
			stream.writeInt32(data.ascent_padding);
			stream.writeInt32(data.height);
			stream.writeInt32(data.line_sepacing_offset);
		}

		inline static auto process_initialized(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			stream.writeBoolean(data.initialized);
			stream.writeInt32(data.default_point_size);
		}

		inline static auto process_character(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			auto characterCount = data.character.size();
			stream.writeUint32(static_cast<uint32_t>(characterCount));
			for (auto i : Range<uint32_t>(static_cast<uint32_t>(characterCount))) {
				stream.writeCharByInt16(data.character[i].index);
				stream.writeCharByInt16(data.character[i].value);
			}
		}

		inline static auto process_layers(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			auto layerCount = data.layer.size();
			stream.writeUint32(static_cast<uint32_t>(layerCount));
			for (auto i : Range<uint32_t>(static_cast<uint32_t>(layerCount))) {
				auto &layer = data.layer[i];
				stream.writeStringByInt32(layer.name);
				auto tag_require_count = layer.tag_require.size();
				stream.writeUint32(static_cast<uint32_t>(tag_require_count));
				for (auto k : Range<uint32_t>(static_cast<uint32_t>(tag_require_count))) {
					stream.writeStringByInt32(layer.tag_require[k]);
				}
				auto tag_exclude_count = layer.tag_exclude.size();
				stream.writeUint32(static_cast<uint32_t>(tag_exclude_count));
				for (auto k : Range<uint32_t>(static_cast<uint32_t>(tag_exclude_count))) {
					stream.writeStringByInt32(layer.tag_exclude[k]);
				}
				auto kerning_count = layer.kerning.size();
				stream.writeUint32(static_cast<uint32_t>(kerning_count));
				for (auto k : Range<uint32_t>(static_cast<uint32_t>(kerning_count))) {
					stream.writeUint16(layer.kerning[k].offset);
					stream.writeCharByInt16(layer.kerning[k].index);
				}
				auto character_count = layer.character.size();
				stream.writeUint32(static_cast<uint32_t>(character_count));
				for (auto k : Range<uint32_t>(static_cast<uint32_t>(character_count))) {
					auto &character = layer.character[k];
					stream.writeCharByInt16(character.index);
					stream.writeInt32(character.image_rect_x);
					stream.writeInt32(character.image_rect_y);
					stream.writeInt32(character.image_rect_width);
					stream.writeInt32(character.image_rect_height);
					stream.writeInt32(character.image_offset_x);
					stream.writeInt32(character.image_offset_y);
					stream.writeUint16(character.kerning_count);
					stream.writeUint16(character.kerning_first);
					stream.writeInt32(character.width);
					stream.writeInt32(character.order);
				}
				stream.writeInt32(layer.multiply_red);
				stream.writeInt32(layer.multiply_green);
				stream.writeInt32(layer.multiply_blue);
				stream.writeInt32(layer.multiply_alpha);
				stream.writeInt32(layer.add_red);
				stream.writeInt32(layer.add_green);
				stream.writeInt32(layer.add_blue);
				stream.writeInt32(layer.add_alpha);
				stream.writeStringByInt32(layer.image_file);
				stream.writeInt32(layer.draw_mode);
				stream.writeInt32(layer.offset_x);
				stream.writeInt32(layer.offset_y);
				stream.writeInt32(layer.spacing);
				stream.writeInt32(layer.minimum_point_size);
				stream.writeInt32(layer.maximum_point_size);
				stream.writeInt32(layer.point_size);
				stream.writeInt32(layer.ascent);
				stream.writeInt32(layer.ascent_padding);
				stream.writeInt32(layer.height);
				stream.writeInt32(layer.default_height);
				stream.writeInt32(layer.line_spacing_offset);
				stream.writeInt32(layer.base_order);
			}
		}

		inline static auto process_tags(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			stream.writeStringByInt32(data.source_file);
			stream.writeStringByInt32(data.error_header);
			stream.writeInt32(data.point_size);
			auto tagCount = data.tag.size();
			stream.writeUint32(static_cast<uint32_t>(tagCount));
			for (auto i : Range<uint32_t>(static_cast<uint32_t>(tagCount))) {
				stream.writeStringByInt32(data.tag[i]);
			}
			stream.writeDouble(data.scale);
			stream.writeBoolean(data.force_scaled_image_white);
			stream.writeBoolean(data.activate_all_layer);
		}

		inline static auto process(
			const CharacterFontWidget2 &data, 
			DataStreamView &stream
		) -> void
		{
			process_unknown(data, stream);
			process_ascend(data, stream);
			process_initialized(data, stream);
			process_character(data, stream);
			process_layers(data, stream);
			process_tags(data, stream);
		}

		inline static auto process_fs (
			std::string_view source, 
			std::string_view destination
		) -> void
		{
			auto source_view = FileSystem::read_json(source);
			auto destination_view = DataStreamView{};
			Encode::process(source_view, destination_view);
			destination_view.out_file(destination);
		}
	};

	#pragma endregion encode

}