#pragma once

#if defined MSVC_COMPILER
#pragma warning(push)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic push
#endif

#include "subprojects/parallel-hashmap/phmap.h"

#if defined MSVC_COMPILER
#pragma warning(pop)
#endif
#if defined CLANG_COMPILER
#pragma clang diagnostic pop
#endif

namespace Sen::Kernel::Subprojects::phmap {

	template <typename Key, typename Value>
	using flat_hash_map = ::phmap::flat_hash_map<Key, Value>;

	template <typename Key>
	using flat_hash_set = ::phmap::flat_hash_set<Key>;

	template <typename Key, typename Value>
	using node_hash_map = ::phmap::node_hash_map<Key, Value>;

}

