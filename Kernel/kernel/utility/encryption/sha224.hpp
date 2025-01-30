#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Encryption::Sha224 {
	
	/**
	 * message: the message need to hash
	 * return: hashed message
	*/

	inline static auto hash(
		const std::string & message
	) -> std::string
	{
		auto sha224 = Subprojects::SHA224CryptoServiceProvider::sha224{};
		auto result = sha224.Hashing(message);
		return result;
	}

	/**
	 * message: the source file need to hash
	 * return: hashed message
	*/

	inline static auto hash_fs(
		const std::string & source
	) -> std::string
	{
		return Encryption::Sha224::hash(FileSystem::read_file(source));
	}


}