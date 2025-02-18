#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list_view/list_view.hpp"

namespace Sen::Kernel {

    template <typename T>
    using ListView = CListView<T>;

    using Uint8ListView = ListView<u8>;

    using Uint16ListView = ListView<u16>;

    using Uint32ListView = ListView<u32>;

    using Uint64ListView = ListView<u64>;

    using Int8ListView = ListView<i8>;

    using Int16ListView = ListView<i16>;

    using Int32ListView = ListView<i32>;

    using Int64ListView = ListView<i64>;

    using Float32ListView = ListView<f32>;

    using Float64ListView = ListView<f64>;

    using SizeListView = ListView<usize>;

}