#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::Miscellaneous::Project::StreamCompressedGroup {

    struct Encode : Common {
        static auto process_whole() -> void {
        }

        static auto process_fs(
                std::string_view source,
                std::string_view destination) -> void {
        }
    };
}