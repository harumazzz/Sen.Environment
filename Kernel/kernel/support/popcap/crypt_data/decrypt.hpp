#pragma once

#include "kernel/support/popcap/crypt_data/definition.hpp"

namespace Sen::Kernel::Support::PopCap::CryptData
{

	#pragma region decrypt

	struct Decrypt : public Definition {
		
		protected:

			using BasicDefinition = Definition;

		public:

			constexpr explicit Decrypt(

			) = default;

			constexpr ~Decrypt(

			) = default;

			inline static auto process(
				DataStreamView & source,
				DataStreamView& destination,
				std::string_view key
			) -> void
			{
				auto code = List<uint8_t>{key.begin(), key.end()};
				assert_conditional((source.readString(magic.size()) == std::string{magic.begin(), magic.end()}), fmt::format("{}", Kernel::Language::get("popcap.crypt_data.decrypt.mismatch_magic")), "process");
            	auto size = source.readUint64();
				if (source.size() > 0x112){
					auto index = 0;
					auto arysize = key.size();
					for (auto i : Range<int>(0x100))
					{
						destination.writeUint8((source.readUint8() ^ code[index++]));
						index %= arysize;
					}
				}
				destination.append(source.get(source.get_read_pos(), source.size()));
				return;
			}

			inline static auto process_fs(
				std::string_view source,
				std::string_view destination,
				std::string_view key
			) -> void
			{
				auto source_view = DataStreamView{ source };
				auto destination_view = DataStreamView{};
				Decrypt::process(source_view, destination_view, key);
				destination_view.out_file(destination);
				return;
			}

	};

	#pragma endregion
}