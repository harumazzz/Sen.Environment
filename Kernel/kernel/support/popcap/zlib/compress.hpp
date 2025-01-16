#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/common.hpp"

namespace Sen::Kernel::Support::PopCap::Zlib {

	template <auto UseVariant>
	struct Compress : Common {

		static_assert(sizeof(UseVariant) == sizeof(bool));

		static_assert(UseVariant == true or UseVariant == false);

		constexpr Compress(

		) = default;


		constexpr ~Compress(

		) = default;


		inline static auto process(
			const List<uint8_t> &source
		) -> List<uint8_t>
		{
			auto stream = DataStreamView{};
			if constexpr (UseVariant) {
				stream.allocate(sizeof(magic) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + compressBound(source.size()));
			}
			else {
				stream.allocate(sizeof(magic) + sizeof(uint32_t) + compressBound(source.size()));
			}
			stream.writeUint32(static_cast<uint32_t>(magic));
			if constexpr (UseVariant)
			{
				stream.writeUint32(static_cast<uint32_t>(0x00));
			}
			stream.writeUint32(static_cast<uint32_t>(source.size()));
			if constexpr (UseVariant)
			{
				stream.writeUint32(static_cast<uint32_t>(0x00));
			}
			stream.writeBytes(Compression::Zlib::compress<Compression::Zlib::Level::LEVEL_9>(source));
			return stream.getBytes(0, stream.size());
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void
		{
			FileSystem::write_binary<uint8_t>(destination, Compress::process(FileSystem::read_binary<uint8_t>(source)));
			return;
		}

	};

}