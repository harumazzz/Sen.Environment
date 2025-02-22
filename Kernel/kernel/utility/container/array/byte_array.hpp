#pragma once

#include "kernel/utility/container/array/array.hpp"

namespace Sen::Kernel {

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

	using SizeArray = CArray<usize>;


	template <typename T> requires is_numeric_v<T>
	inline static auto forward_bytes (
		const T& arg,
		Uint8Array& buffer,
		size_t& offset
	) -> void {
		std::memcpy(buffer.begin() + offset, &arg, sizeof(T));
		offset += sizeof(T);
	}

}