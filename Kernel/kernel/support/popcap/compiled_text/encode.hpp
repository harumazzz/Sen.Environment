#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/compress.hpp"

namespace Sen::Kernel::Support::PopCap::CompiledText {

	using namespace Sen::Kernel;

	using namespace Sen::Kernel::Encryption;

	struct Encode {

		constexpr explicit Encode(

		) = default;

		constexpr ~Encode(

		) = default;

		template <auto UseVariant>
		inline static auto process(
			DataStreamView &stream,
			std::string_view key,
			std::string_view iv,
			DataStreamView &result
		) -> void {
			using Rijndael = Kernel::Encryption::Rijndael;
			auto buffer = DataStreamView{};
			buffer.writeBytes(PopCap::Zlib::Compress<UseVariant>::process(stream.getBytes(0, stream.size())));
			Encryption::fill_rijndael_block(buffer, iv);
			auto decoded_base64 = DataStreamView{};
			decoded_base64.writeUint8(0x10);
			decoded_base64.writeUint8(0x00);
			decoded_base64.writeBytes(Rijndael::encrypt<std::uint64_t, Rijndael::Mode::CBC>(reinterpret_cast<char *>(buffer.begin_pointer()), key, iv, buffer.size()));
			auto encoded_base64 = DataStreamView{};
			encoded_base64.fromString(Base64::encode(decoded_base64.begin_pointer(), decoded_base64.size()));
			result.writeBytes(encoded_base64.getBytes(0, encoded_base64.size()));
		}

		inline static auto process_fs (
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