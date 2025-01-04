#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Encryption::XOR 
{

	/**
	 * key: the key to encrypt
	 * plain: the plain text
	 * return: cipher text
	*/

	inline static auto encrypt (
		const List<std::uint8_t> & plain,
		const List<std::uint8_t> & key
	) -> List<std::uint8_t>
	{
		auto result = List<std::uint8_t>(plain.size(), 0x00_byte);
		for (auto i : Range(plain.size())) {
			result[i] = plain[i] ^ key[i % key.size()];
		}
		return result;
	}

	/**
	 * @param source: source file
	 * @param destination: destination file
	 * @param key: key
	 * @returns: encrypted file
	*/

	inline static auto encrypt_fs(
		std::string_view source,
		std::string_view destination,
		const List<std::uint8_t> & key
	) -> void
	{
		FileSystem::write_binary(destination, XOR::encrypt(FileSystem::read_binary<std::uint8_t>(source), List<std::uint8_t>{key.begin(), key.end()}));
		return;
	}
	

}