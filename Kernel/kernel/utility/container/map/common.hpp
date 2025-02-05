#pragma once

#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

    template <typename Key, typename Value>
    using HashMap = Subprojects::phmap::flat_hash_map<Key, Value>;

    template <typename Key, typename Value>
    using Map = HashMap<Key, Value>;

}