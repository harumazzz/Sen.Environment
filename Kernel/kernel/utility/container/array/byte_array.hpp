#pragma once

#include "kernel/utility/container/array/array.hpp"

namespace Sen::Kernel {

	using u8 = std::uint8_t;

	using u16 = std::uint16_t;

	using u32 = std::uint32_t;

	using u64 = std::uint64_t;

	using i8 = std::int8_t;

	using i16 = std::int16_t;

	using i32 = std::int32_t;

	using i64 = std::int64_t;

	using usize = std::size_t;

	using f32 = float;

	using f64 = double;

	using Uint8Array = CArray<u8>;

	using Uint16Array = CArray<u16>;

	using Uint32Array = CArray<u32>;

	using Uint64Array = CArray<u64>;

	using Int8Array = CArray<i8>;

	using Int16Array = CArray<i16>;

	using Int32Array = CArray<i32>;

	using Int64Array = CArray<i64>;

	using Float32Array = CArray<f32>;

	using Float64Array = CArray<f64>;

	using CharacterArray = CArray<char>;

	using UCharacterArray = CArray<unsigned char>;

    using ByteArray = Uint8Array;

}