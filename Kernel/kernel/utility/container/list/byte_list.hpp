#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/list.hpp"

namespace Sen::Kernel {

    using Uint8List = CList<u8>;

    using Uint16List = CList<u16>;

    using Uint32List = CList<u32>;

    using Uint64List = CList<u64>;

    using Int8List = CList<i8>;

    using Int16List = CList<i16>;

    using Int32List = CList<i32>;

    using Int64List = CList<i64>;

    using Float32List = CList<f32>;

    using Float64List = CList<f64>;

    using SizeList = CList<usize>;

    using ByteList = Uint8List;

}