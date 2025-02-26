#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/container/list/list.hpp"

namespace Sen::Kernel {

    template <typename Key, typename Value>
    using Pair = std::pair<Key, Value>;

    template <typename Key, typename Value>
    class LinearMap : public List<Pair<Key, Value>>{

    public:

        using key_type = Key;

        using value_type = Value;

    protected:

        using Base = List<Pair<Key, Value>>;

    public:

        LinearMap() = default;

        auto operator[](
            const Key& key
        ) -> Value& {
            for (auto& pair : thiz) {
                if (pair.first == key) {
                    return pair.second;
                }
            }
            assert_conditional(false, "Key not found", "operator[]");
        }

        auto insert(
            const Key& key,
            const Value& value
        ) -> void {
            for (auto& pair : thiz) {
                if (pair.first == key) {
                    pair.second = value;
                    return;
                }
            }
            thiz.append({key, value});
        }

        auto find(
            const Key& key
        ) -> Value* {
            for (auto& pair : thiz) {
                if (pair.first == key) {
                    return &pair.second;
                }
            }
            return nullptr;
        }

        auto contains(
            const Key& key
        ) const -> bool {
            for (const auto& pair : thiz) {
                if (pair.first == key) {
                    return true;
                }
            }
            return false;
        }


    };

    template <typename Key, typename Value>
    struct is_map<LinearMap<Key, Value>> : std::true_type {

    };

}


namespace jsoncons {
    template<typename Json, typename K, typename V>
    struct json_type_traits<Json, Sen::Kernel::LinearMap<K, V>> {
        using class_type = Sen::Kernel::LinearMap<K, V>;
        using allocator_type = typename Json::allocator_type;

        static bool is(const Json& ajson) noexcept {
            return ajson.is_object();
        }

        static auto as(const Json& ajson) -> class_type {
            if (!is(ajson)) throw conv_error(conv_errc::conversion_failed, "Not a Map");
            class_type class_instance;
            for (const auto& item : ajson.object_range()) {
                class_instance.append(std::make_pair(item.key(), item.value().template as<V>()));
            }
            return class_instance;
        }

        static Json to_json(const class_type& class_instance, allocator_type alloc = allocator_type()) {
            Json obj(jsoncons::json_object_arg, alloc);
            for (const auto& [key, value] : class_instance) {
                obj.try_emplace(key, value);
            }
            return obj;
        }
    };
}
