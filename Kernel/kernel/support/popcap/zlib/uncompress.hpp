#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/common.hpp"

namespace Sen::Kernel::Support::PopCap::Zlib {
	
	template <auto UseVariant>
	struct Uncompress : Common {

		static_assert(sizeof(UseVariant) == sizeof(bool));

		static_assert(UseVariant == true or UseVariant == false);

		constexpr explicit Uncompress(

		) = default;

		constexpr ~Uncompress(

		) = default;

		inline static auto process (
			const List<unsigned char> & source 
		) -> List<unsigned char> 
		{
			auto stream = DataStreamView{source};
			auto magic = stream.readUint32();
			assert_conditional(magic == static_cast<uint32_t>(Uncompress::magic), fmt::format("{}: 0x{:X}", Language::get("popcap.zlib.uncompress.mismatch_zlib_magic"), Uncompress::magic), "process");
			auto cut_offset = static_cast<size_t>(8);
			if constexpr (UseVariant){
				cut_offset += 8;
				stream.readUint32();
				stream.readUint32();
			}
			auto result = Compression::Zlib::uncompress(stream.getBytes(cut_offset, stream.size()));
			return result;
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void
		{
			auto uncompressed_data = Uncompress::process(FileSystem::read_binary<unsigned char>(source));
			FileSystem::write_binary<unsigned char>(destination, uncompressed_data);
			return;
		}
	};
}