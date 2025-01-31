#pragma once 

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel::Encryption::MD5 
{
	
	inline static auto hash(
		const std::span<const uint8_t> &message
	) -> std::string
	{
		auto md5 = Subprojects::md5::MD5{ message };
		auto result = std::string{ reinterpret_cast<const char*>(md5.getDigest()), 16_size };
		return result;
	}

}