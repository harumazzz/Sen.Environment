#pragma once

#include "kernel/support/popcap/render_effects/common.hpp"
#include "kernel/support/popcap/render_effects/definition.hpp"

namespace Sen::Kernel::Support::PopCap::RenderEffects {

	#pragma region using

	using Sen::Kernel::DataStreamView;

	#pragma endregion

	#pragma region encode

	struct Encode : public Common {

		private:

			#pragma region using

			using RenderEffects = PopCapRenderEffects;

			using BasicDefinition = Common;

			#pragma endregion

		public:

			constexpr explicit Encode(

			) = default;

			constexpr ~Encode(

			) = default;

			inline static auto process (
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void
			{
				auto block_offset = BlockOffset{};
				auto dataStream = DataStreamView{};
				write_null_data(dataStream, stream);
				write_block1_data(dataStream, block_offset, stream, data);
				write_block5_data(dataStream, block_offset, stream, data);
				write_block6_data(dataStream, block_offset, stream, data);
				write_block2_data(dataStream, block_offset, stream, data);
				auto string_section = DataStreamView{};
				write_block3_data(dataStream, block_offset, stream, data, string_section);
				write_block4_data(dataStream, block_offset, stream, data);
				write_block7_data(dataStream, block_offset, stream, data);
				write_block8_data(dataStream, block_offset, stream, data);
				forward_stream(stream, block_offset, dataStream, data, string_section);
			}

			inline static auto write_null_data(
				DataStreamView& dataStream, 
				DataStreamView& stream
			) -> void
			{
				dataStream.writeNull(Common::BlockSectionOffset);
				stream.writeString(std::string{ magic.data(), magic.size() });
				stream.writeUint32(version);
			}

			inline static auto write_block1_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void
			{
				block_offset.block1_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_1) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
					dataStream.writeUint32(item.unknown_3);
					dataStream.writeUint32(item.unknown_4);
					dataStream.writeUint32(item.unknown_5);
					dataStream.writeUint32(item.unknown_6);
				}
			}

			inline static auto write_block5_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void
			{
				block_offset.block5_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_5) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
					dataStream.writeUint32(item.unknown_3);
					dataStream.writeUint32(item.unknown_4);
					dataStream.writeUint32(item.unknown_5);
					dataStream.writeUint32(item.unknown_6);
					dataStream.writeUint32(item.unknown_7);
				}
			}

			inline static auto write_block6_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void 
			{
				block_offset.block6_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_6) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
					dataStream.writeUint32(item.unknown_3);
					dataStream.writeUint32(item.unknown_4);
					dataStream.writeUint32(item.unknown_5);
				}
			}

			inline static auto write_block2_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void 
			{
				block_offset.block2_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_2) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
				}
			}

			inline static auto write_block3_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data, 
				DataStreamView& string_section
			) -> void
			{
				block_offset.block3_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_3) {
					dataStream.writeUint32(static_cast<uint32_t>(item.string.size()));
					dataStream.writeUint32(item.unknown_2);
					dataStream.writeUint32(string_section.write_position());
					string_section.writeStringByEmpty(item.string);
				}
			}

			inline static auto write_block4_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void 
			{
				block_offset.block4_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_4) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
					dataStream.writeUint32(item.unknown_3);
					dataStream.writeUint32(item.unknown_4);
					dataStream.writeUint32(item.unknown_5);
				}
			}

			inline static auto write_block7_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void 
			{
				block_offset.block7_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_7) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
				}
			}

			inline static auto write_block8_data(
				DataStreamView& dataStream, 
				BlockOffset& block_offset, 
				DataStreamView& stream, 
				const RenderEffects& data
			) -> void 
			{
				block_offset.block8_section_offset = static_cast<uint32_t>(dataStream.write_position());
				for (auto& item : data.block_8) {
					dataStream.writeUint32(item.unknown_1);
					dataStream.writeUint32(item.unknown_2);
					dataStream.writeUint32(item.unknown_4);
					dataStream.writeUint32(item.unknown_5);
					dataStream.writeUint32(item.unknown_3);
				}
			}

			inline static auto forward_stream(
				DataStreamView& stream, 
				BlockOffset& block_offset, 
				DataStreamView& dataStream, 
				const RenderEffects& data,
				DataStreamView& string_section
			) -> void
			{
				stream.writeUint32(static_cast<uint32_t>(data.block_1.size()));
				stream.writeUint32(block_offset.block1_section_offset);
				stream.writeUint32(Common::Block1SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_2.size()));
				stream.writeUint32(block_offset.block2_section_offset);
				stream.writeUint32(Common::Block2SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_3.size()));
				stream.writeUint32(block_offset.block3_section_offset);
				stream.writeUint32(Common::Block3SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_4.size()));
				stream.writeUint32(block_offset.block4_section_offset);
				stream.writeUint32(Common::Block4SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_5.size()));
				stream.writeUint32(block_offset.block5_section_offset);
				stream.writeUint32(Common::Block5SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_6.size()));
				stream.writeUint32(block_offset.block6_section_offset);
				stream.writeUint32(Common::Block6SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_7.size()));
				stream.writeUint32(block_offset.block7_section_offset);
				stream.writeUint32(Common::Block7SectionSize);
				stream.writeUint32(static_cast<uint32_t>(data.block_8.size()));
				stream.writeUint32(block_offset.block8_section_offset);
				stream.writeUint32(Common::Block8SectionSize);
				stream.writeUint32(static_cast<uint32_t>(BlockSectionOffset));
				stream.writeBytes(dataStream.get(static_cast<uint64_t>(Common::BlockSectionOffset), dataStream.size()));
				stream.writeBytes(string_section.get(0, string_section.size()));
			}

			inline static auto process_fs(
				std::string_view source,
				std::string_view destination
			) -> void
			{
				auto stream = DataStreamView{};
				auto render_effects = FileSystem::read_json(source);
				Encode::process(stream, render_effects);
				stream.out_file(destination);
				return;
			}
	};

	#pragma endregion
}
