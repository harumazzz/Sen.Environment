#pragma once

#include "kernel/support/popcap/render_effects/common.hpp"
#include "kernel/support/popcap/render_effects/definition.hpp"

namespace Sen::Kernel::Support::PopCap::RenderEffects {

	#pragma region using

	using Sen::Kernel::DataStreamView;


	#pragma endregion
	

	#pragma region decode

	struct Decode : public Common {
		
		private:

			#pragma region using

			using RenderEffects = PopCapRenderEffects;

			using BasicDefinition = Common;

			#pragma endregion

		public:

			constexpr explicit Decode(

			) = default;

			constexpr ~Decode(

			) = default;
			
			inline static auto process(
				RenderEffects& render_effects, 
				DataStreamView& stream
			) -> void
			{
				auto info = BasicDefinition{};
				validate_magic_and_version(stream, info);
				read_block_sizes_and_offsets(stream, info);
				process_block_data(stream, info, render_effects);
				return;
			}

			inline static auto validate_magic_and_version(
				DataStreamView& stream, 
				BasicDefinition& info
			) -> void
			{
				assert_conditional(stream.readString(4) == magic, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.mismatch_magic")), "validate_magic_and_version");
				assert_conditional(stream.readUint32() == version, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.mismatch_version")), "validate_magic_and_version");
			}

			inline static auto read_block_sizes_and_offsets(
				DataStreamView& stream, 
				BasicDefinition& info
			) -> void
			{
				info.block1_size = stream.readUint32();
				info.block1_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block1SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_1_section_size")), "read_block_sizes_and_offsets");
				info.block2_size = stream.readUint32();
				info.block2_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block2SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_2_section_size")), "read_block_sizes_and_offsets");
				info.block3_size = stream.readUint32();
				info.block3_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block3SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_3_section_size")), "read_block_sizes_and_offsets");
				info.block4_size = stream.readUint32();
				info.block4_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block4SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_4_section_size")), "read_block_sizes_and_offsets");
				info.block5_size = stream.readUint32();
				info.block5_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block5SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_5_section_size")), "read_block_sizes_and_offsets");
				info.block6_size = stream.readUint32();
				info.block6_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block6SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_6_section_size")), "read_block_sizes_and_offsets");
				info.block7_size = stream.readUint32();
				info.block7_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block7SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_7_section_size")), "read_block_sizes_and_offsets");
				info.block8_size = stream.readUint32();
				info.block8_section_offset = stream.readUint32();
				assert_conditional(stream.readUint32() == info.Block8SectionSize, fmt::format("{}", Kernel::Language::get("popcap.render_effects.decode.invalid_block_8_section_size")), "read_block_sizes_and_offsets");
				info.string_section_offset = stream.readUint32();
			}

			inline static auto process_block_data(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				process_block1(stream, info, render_effects);
				process_block2(stream, info, render_effects);
				auto string_section = DataStreamView{ stream.get(info.string_section_offset, stream.size()) };
				process_block3(stream, string_section, info, render_effects);
				process_block4(stream, info, render_effects);
				process_block5(stream, info, render_effects);
				process_block6(stream, info, render_effects);
				process_block7(stream, info, render_effects);
				process_block8(stream, info, render_effects);
			}

			inline static auto process_block1(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block1_section_offset);
				render_effects.block_1.reserve(info.block1_size);
				for (auto i : Range<uint32_t>(info.block1_size)) {
					render_effects.block_1.emplace_back(Block1{
						stream.readUint32(), stream.readUint32(), stream.readUint32(),
						stream.readUint32(), stream.readUint32(), stream.readUint32()
					});
				}
			}

			inline static auto process_block2(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block2_section_offset);
				render_effects.block_2.reserve(info.block2_size);
				for (auto i : Range<uint32_t>(info.block2_size)) {
					render_effects.block_2.emplace_back(Block2{
						stream.readUint32(), stream.readUint32()
					});
				}
			}

			inline static auto process_block3(
				DataStreamView& stream, 
				DataStreamView& string_section, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block3_section_offset);
				render_effects.block_3.reserve(info.block3_size);
				for (auto i : Range<uint32_t>(info.block3_size)) {
					stream.readUint32();
					auto block_3 = Block3{};
					block_3.unknown_2 = stream.readUint32();
					string_section.read_position(stream.readUint32());
					block_3.string = string_section.readStringByEmpty();
					render_effects.block_3.emplace_back(block_3);
				}
			}

			inline static auto process_block4(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block4_section_offset);
				render_effects.block_4.reserve(info.block4_size);
				for (auto i : Range<uint32_t>(info.block4_size)) {
					render_effects.block_4.emplace_back(Block4{
						stream.readUint32(), stream.readUint32(), stream.readUint32(),
						stream.readUint32(), stream.readUint32()
					});
				}
			}

			inline static auto process_block5(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block5_section_offset);
				render_effects.block_5.reserve(info.block5_size);
				for (auto i : Range<uint32_t>(info.block5_size)) {
					render_effects.block_5.emplace_back(Block5{
						stream.readUint32(), stream.readUint32(), stream.readUint32(),
						stream.readUint32(), stream.readUint32(), stream.readUint32(),
						stream.readUint32()
					});
				}
			}

			inline static auto process_block6(
				DataStreamView& stream,
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block6_section_offset);
				render_effects.block_6.reserve(info.block6_size);
				for (auto i : Range<uint32_t>(info.block6_size)) {
					render_effects.block_6.emplace_back(Block6{
						stream.readUint32(), stream.readUint32(), stream.readUint32(),
						stream.readUint32(), stream.readUint32()
					});
				}
			}

			inline static auto process_block7(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block7_section_offset);
				render_effects.block_7.reserve(info.block7_size);
				for (auto i : Range<uint32_t>(info.block7_size)) {
					render_effects.block_7.emplace_back(Block7{
						stream.readUint32(), stream.readUint32()
					});
				}
			}

			inline static auto process_block8(
				DataStreamView& stream, 
				BasicDefinition& info, 
				RenderEffects& render_effects
			) -> void
			{
				stream.read_position(info.block8_section_offset);
				render_effects.block_8.reserve(info.block8_size);
				for (auto i : Range<uint32_t>(info.block8_size)) {
					render_effects.block_8.emplace_back(Block8{
						stream.readUint32(), stream.readUint32(), stream.readUint32(),
						stream.readUint32(), stream.readUint32()
					});
				}
			}

			inline static auto process_fs(
				std::string_view source, 
				std::string_view destination
			) -> void 
			{
				auto stream = DataStreamView{source};
				auto result = RenderEffects{};
				process(result, stream);
				FileSystem::write_json(destination, result);
			}

	};

	#pragma endregion
}