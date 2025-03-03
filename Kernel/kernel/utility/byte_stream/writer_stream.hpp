#pragma once
#include "kernel/subprojects/decodeless.hpp"

namespace Sen::Kernel {

    template <typename T>
    using WriterStream = decodeless::file_writer<T>;
}
