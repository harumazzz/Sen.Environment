#pragma once 

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Encryption::MD5 
{

	using byte = unsigned char;
	
	inline static auto hash(
		const std::span<const byte> &message
	) -> std::string
	{
		auto md5 = Dependencies::md5::MD5{ message };
		auto result = md5.toStr();
		return result;
	}

	inline static auto hash_fs(
		std::string_view source
	) -> std::string
	{
		auto str = FileSystem::read_binary<unsigned char>(source);
		auto result = Encryption::MD5::hash(str);
		return result;
	}

}