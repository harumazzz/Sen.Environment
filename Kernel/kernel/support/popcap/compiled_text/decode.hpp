#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/uncompress.hpp"

namespace Sen::Kernel::Support::PopCap::CompiledText
{

	using namespace Sen::Kernel;

	using namespace Sen::Kernel::Encryption;

	template <auto UseVariant>
	using Zlib = Sen::Kernel::Support::PopCap::Zlib::Uncompress<UseVariant>;

	struct Decode {

		constexpr explicit Decode(

		) = default;

		constexpr ~Decode(

		) = default;

		template <auto UseVariant>
		inline static auto process(
			DataStreamView &source,
			std::string_view key,
			std::string_view iv,
			DataStreamView& destination
		) -> void
		{
			auto decoded_base64 = DataStreamView{};
			auto source_data = Uint8Array{source.size()};
			std::memcpy(source_data.data(), source.value().data(), source.size());
			auto destination_data = Uint8Array{};
			Base64::Decode::process(steal_reference<UCharacterArray>(source_data), destination_data);
			decoded_base64.fromString(std::string{reinterpret_cast<const char*>(destination_data.data()), destination_data.size()});
			destination.append<unsigned char>(Zlib<UseVariant>::process(Rijndael::decrypt<std::uint64_t, Rijndael::Mode::CBC>(reinterpret_cast<char *>(decoded_base64.getBytes(0x02, decoded_base64.size()).data()), key, iv, decoded_base64.size() - 0x02)));
			return;
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination,
			std::string_view key,
			std::string_view iv,
			bool use_64_bit_variant
		) -> void
		{
			auto source_view = DataStreamView{source};
			auto destination_view = DataStreamView{};
			if (use_64_bit_variant) {
				process<true>(source_view, key, iv, destination_view);
			}
			else {
				process<false>(source_view, key, iv, destination_view);
			}
			destination_view.out_file(destination);
			return;
		}
	};
}