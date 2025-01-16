#pragma once

#include "kernel/support/popcap/crypt_data/definition.hpp"

namespace Sen::Kernel::Support::PopCap::CryptData
{

	#pragma region encrypt

	struct Encrypt : public Definition {

		protected:

			using BasicDefinition = Definition;

		public:

			constexpr explicit Encrypt(

			) = default;


			constexpr ~Encrypt(

			) = default;

			inline static auto process(
				DataStreamView & source,
				DataStreamView& destination,
				std::string_view key
			) -> void
			{
            	auto size = source.size();
				destination.append<uint8_t, magic.size()>(magic);
				destination.writeUint64(size);
				auto code = List<uint8_t>{key.begin(), key.end()};
				if (size >= 0x100)
				{
					auto index = 0;
					auto arysize = key.size();
					for (auto i : Range<int>(0x100))
					{
						destination.writeUint8((source.readUint8() ^ code[index++]));
						index %= arysize;
					}
				}
				destination.append(source.get(source.get_write_pos(), source.size()));
				return;
			}

			inline static auto process_fs(
				std::string_view source,
				std::string_view destination,
				std::string_view key
			) -> void
			{
				auto source_view = DataStreamView{source};
				auto destination_view = DataStreamView{};
				Encrypt::process(source_view, destination_view, key);
				destination_view.out_file(destination);
				return;
			}

	};


	#pragma endregion
}
