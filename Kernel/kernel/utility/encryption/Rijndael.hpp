#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/dependencies/Rijndael.hpp"

namespace Sen::Kernel::Encryption 
{

	// Rijndael Struct

	struct Rijndael {

		private:
			using CRijndael = Sen::Kernel::Dependencies::Rijndael::CRijndael;

		protected:

			template <typename T> requires std::is_integral<T>::value
			inline static auto constexpr is_padded_size(
				T size, 
				T block_size
			) noexcept -> bool
			{
    			return size % block_size == 0;
			}

			template <typename T> requires std::is_integral<T>::value
			inline static auto constexpr is_valid_block_size(
				T block_size
			) noexcept -> bool {
    			return block_size == 128 || block_size == 160 || block_size == 192 || block_size == 224 || block_size == 256;
			}

		public:

			Rijndael(

			) = default;

			~Rijndael(

			) = default;

			// Rijndael Mode

			enum class Mode : uint8_t {
				ECB,
				CBC,
				CFB,
			};
		
			template <typename T, auto mode> requires std::is_integral<T>::value
			inline static auto encrypt(
				char const* plain,
				std::string_view key,
				std::string_view iv,
				T plain_size
			) -> List<unsigned char>
			{
				static_assert(mode == Mode::CBC || mode == Mode::CFB || mode == Mode::ECB, "mode is invalid, expected cbc, cfb or ecb");
				assert_conditional(is_valid_block_size<std::size_t>(key.size() * 8), fmt::format("{}", Language::get("rijndael.key_size_is_not_valid")), "encrypt");
				assert_conditional(is_valid_block_size<std::size_t>(iv.size() * 8), fmt::format("{}", Language::get("rijndael.iv_size_is_not_valid")), "encrypt");
				assert_conditional(is_padded_size<std::size_t>(key.size(), 32), fmt::format("{}", Language::get("rijndael.key_is_not_padded")), "encrypt");
				assert_conditional(plain_size != 0, fmt::format("{}", Language::get("rijndael.encrypt.plain_is_empty")), "encrypt");
				auto rijndael = std::make_shared<CRijndael>();
				rijndael->MakeKey(key.data(), iv.data(), static_cast<int>(key.size()), static_cast<int>(iv.size()));
				auto result = std::make_unique<char[]>(plain_size);
				rijndael->Encrypt(plain, result.get(), plain_size, static_cast<int>(mode));
				auto m_result = List<unsigned char>{reinterpret_cast<unsigned char*>(result.get()), reinterpret_cast<unsigned char*>(result.get() + plain_size)};
				return m_result;
			}

			template <typename T, auto mode> requires std::is_integral<T>::value
			inline static auto decrypt(
				char const* cipher,
				std::string_view key,
				std::string_view iv,
				T cipher_len
			) -> List<unsigned char>
			{
				static_assert(mode == Mode::CBC || mode == Mode::CFB || mode == Mode::ECB, "mode is invalid, expected cbc, cfb or ecb");
				assert_conditional(is_valid_block_size<std::size_t>(key.size() * 8), fmt::format("{}", Language::get("rijndael.key_size_is_not_valid")), "decrypt");
				assert_conditional(is_valid_block_size<std::size_t>(iv.size() * 8), fmt::format("{}", Language::get("rijndael.iv_size_is_not_valid")), "decrypt");
				assert_conditional(is_padded_size<std::size_t>(key.size(), 32), fmt::format("{}", Language::get("rijndael.key_is_not_padded")), "decrypt");
				assert_conditional(cipher_len != 0, fmt::format("{}", Language::get("rijndael.decrypt.cipher_is_empty")), "decrypt");
				assert_conditional((iv.size() - ((cipher_len + iv.size() - 1) % iv.size() + 1)) == 0, fmt::format("{}", Language::get("rijndael.decrypt.cipher_size_is_invalid_to_decrypt")), "decrypt");
				auto rijndael = std::make_shared<CRijndael>();
				rijndael->MakeKey(key.data(), iv.data(), static_cast<int>(key.size()), static_cast<int>(iv.size()));
				auto result = std::make_unique<char[]>(cipher_len);
				rijndael->Decrypt(cipher, result.get(), cipher_len, static_cast<int>(mode));
				auto m_result = List<unsigned char>{reinterpret_cast<unsigned char*>(result.get()), reinterpret_cast<unsigned char*>(result.get() + cipher_len)};
				return m_result;
			}

			inline static auto encrypt_fs(
				std::string_view source,
				std::string_view destination,
				std::string_view key,
				std::string_view iv,
				Mode mode
			) -> void
			{
				auto source_data = FileSystem::read_binary<char>(source);
				switch (mode) {
					case Mode::CBC:{
						FileSystem::write_binary<unsigned char>(destination, Rijndael::encrypt<std::uint64_t, Mode::CBC>(source_data.data(), key, iv, source_data.size()));
						break;
					}
					case Mode::CFB:{
						FileSystem::write_binary<unsigned char>(destination, Rijndael::encrypt<std::uint64_t, Mode::CFB>(source_data.data(), key, iv, source_data.size()));
						break;
					}
					case Mode::ECB:{
						FileSystem::write_binary<unsigned char>(destination, Rijndael::encrypt<std::uint64_t, Mode::ECB>(source_data.data(), key, iv, source_data.size()));
						break;
					}
				}
				return;
			}

			inline static auto decrypt_fs(
				std::string_view source,
				std::string_view destination,
				std::string_view key,
				std::string_view iv,
				Mode mode
			) -> void
			{
				auto source_data = FileSystem::read_binary<char>(source);
				switch (mode) {
					case Mode::CBC:{
						FileSystem::write_binary<unsigned char>(destination, Rijndael::decrypt<std::uint64_t, Mode::CBC>(source_data.data(), key, iv, source_data.size()));
						break;
					}
					case Mode::CFB:{
						FileSystem::write_binary<unsigned char>(destination, Rijndael::decrypt<std::uint64_t, Mode::CFB>(source_data.data(), key, iv, source_data.size()));
						break;
					}
					case Mode::ECB:{
						FileSystem::write_binary<unsigned char>(destination, Rijndael::decrypt<std::uint64_t, Mode::ECB>(source_data.data(), key, iv, source_data.size()));
						break;
					}
				}
				return;
			}

	};

	inline static auto fill_rijndael_block(
		DataStreamView &raw, 
		std::string_view iv
	) -> void
	{
		auto padding = iv.size() - ((raw.size() + iv.size() - 1) % iv.size() + 1);
		raw.writeNull(padding);
		return;
	}
}