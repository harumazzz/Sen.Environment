#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/array_view/array_view.hpp"

namespace Sen::Kernel {

    template <typename T>
    using ArrayView = CArrayView<T>;

    using Uint8ArrayView = ArrayView<u8>;

    using Uint16ArrayView = ArrayView<u16>;

    using Uint32ArrayView = ArrayView<u32>;

    using Uint64ArrayView = ArrayView<u64>;

    using ArrayBuffer = Uint8ArrayView;

    using Int8ArrayView = ArrayView<i8>;

    using Int16ArrayView = ArrayView<i16>;

    using Int32ArrayView = ArrayView<i32>;

    using Int64ArrayView = ArrayView<i64>;

    using Float32ArrayView = ArrayView<f32>;

    using Float64ArrayView = ArrayView<f64>;

    using SizeArrayView = ArrayView<usize>;

    using CharacterArrayView = ArrayView<char>;

    using UCharacterArrayView = ArrayView<uchar>;

}