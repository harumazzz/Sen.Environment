#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/map/common.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Compression::Zip {

    struct Common {

        static auto on_extract(
            void *arg,
            uint64_t offset,
            const void *data,
            size_t size
        ) -> size_t
        {
            auto destination = static_cast<Uint8Array*>(arg);
            std::memcpy(destination->begin(), data, size);
            return size;
        }

    };

}

