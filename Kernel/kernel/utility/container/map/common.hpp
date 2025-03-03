#pragma once

#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

    template <typename Key, typename Value>
    using HashMap = Subprojects::phmap::flat_hash_map<Key, Value>;

    template <typename Key, typename Value>
    using Map = HashMap<Key, Value>;

    template <typename Key, typename Value>
    struct is_map<HashMap<Key, Value>> : std::true_type {

    };

}

template<typename Json>
struct jsoncons::json_type_traits<Json, Sen::Kernel::HashMap<Sen::Kernel::String, typename Json::value_type>> {
    using class_type = Sen::Kernel::HashMap<Sen::Kernel::String, typename Json::value_type>;
    using allocator_type = typename Json::allocator_type;
    using char_type = typename Json::char_type;
  //  using string_view_type = typename Json::string_view_type;
    using mapped_type = typename Json::value_type;

    static bool is(const Json &ajson) noexcept {
        return ajson.is_object();
    }

    static auto as(const Json &ajson) -> class_type {
        if (!is(ajson)) throw conv_error(conv_errc::conversion_failed, "Not an Object");

        class_type class_instance;
        for (const auto& item : ajson.object_range()) {
            auto key = Sen::Kernel::String{item.key().data(), item.key().size()};
            class_instance[key] = item.value().template as<mapped_type>();
        }
        return class_instance;
    }

    static Json to_json(const class_type &class_instance, allocator_type alloc = allocator_type()) {
        Json ajson(jsoncons::json_object_arg, alloc);
        for (const auto& item : class_instance) {
            ajson.try_emplace(item.first, item.second);
        }
        return ajson;
    }
};
