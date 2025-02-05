#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/assert.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Encryption::FNV {

	enum class Mode : u8 {
		FNV0,
		FNV1,
		FNV1a,
	};

	enum class BitCount : u8 {
		B32,
		B64,
	};

	template <auto bit_count> requires std::is_same_v<type_of<bit_count>, BitCount>
	struct Parameter;

	template <>
	struct Parameter<BitCount::B32> {

		using Value = uint32_t;

		static constexpr auto prime = 16777619u;

		static constexpr auto offset = 2166136261u;
	};

	template <>
	struct Parameter<BitCount::B64> {

		using Value = uint64_t;

		static constexpr auto prime = 1099511628211ull;

		static constexpr auto offset = 14695981039346656037ull;
	};
	

}
