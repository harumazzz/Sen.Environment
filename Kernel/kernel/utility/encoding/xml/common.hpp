#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/map/common.hpp"

namespace Sen::Kernel::Encoding::XML {

    template <typename T>
    using List = CList<T>;

    struct Node;

    struct Comment;

    struct Text;

    struct Element;

}