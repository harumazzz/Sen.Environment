#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"

namespace Sen::Kernel::Javascript {

    template <typename T> requires is_numeric_v<T> && (!is_boolean_v<T>)
    struct Trait<T> {

        static auto from_value(
            Value& source,
            T& destination
        ) -> void {
            if constexpr (std::is_floating_point_v<T>) {
                auto value = f64{};
                assert_conditional(source.is_number(), "Expected the value to be number, but the actual type is not", "from_value");
                Subprojects::quickjs::JS_ToFloat64(source._context(), &value, source.value());
                destination = static_cast<T>(value);
            }
            else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T>) {
                auto value = i64{};
                assert_conditional(source.is_bigint(), "Expected the value to be bigint, but the actual type is not", "from_value");
                Subprojects::quickjs::JS_ToBigInt64(source._context(), &value, source.value());
                destination = static_cast<T>(value);
            }
            else {
                auto value = u64{};
                assert_conditional(source.is_bigint(), "Expected the value to be bigint, but the actual type is not", "from_value");
                Subprojects::quickjs::JS_ToBigUint64(source._context(), &value, source.value());
                destination = static_cast<T>(value);
            }
        }

        static auto to_value(
            const T& source,
            Value& destination
        ) -> void {
            if constexpr (std::is_floating_point_v<T>) {
                destination.set_value(Subprojects::quickjs::JS_NewFloat64(destination._context(), source));
            }
            else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T>) {
                destination.set_value(Subprojects::quickjs::JS_NewBigInt64(destination._context(), source));
            }
            else {
                destination.set_value(Subprojects::quickjs::JS_NewBigUint64(destination._context(), source));
            }
        }

    };

    template <>
    struct Trait<NativeString> {

        static auto from_value(
            Value& source,
            NativeString& destination
        ) -> void {
            assert_conditional(source.is_string(), "Expected the value to be string, but the actual type is not", "from_value");
            destination.assign_from(source);
        }

        static auto to_value(
            const NativeString& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewStringLen(destination._context(), source.begin(), source.size()));
        }

    };

    template <>
    struct Trait<String> {

        static auto from_value(
            Value& source,
            String& destination
        ) -> void {
            auto size = usize{};
            const auto buffer = Subprojects::quickjs::JS_ToCStringLen(source._context(), &size, source.value());
            auto movable_string = String{buffer, size};
            Subprojects::quickjs::JS_FreeCString(source._context(), buffer);
            destination.take_ownership(movable_string);
        }

        static auto to_value(
            auto&& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewStringLen(destination._context(), source.begin(), source.size()));
        }

    };

    template <typename Key, typename MapValue>
    struct Trait<HashMap<Key, MapValue>> {

        static auto from_value(
            Value& source,
            HashMap<Key, MapValue>& destination
        ) -> void {
            assert_conditional(source.is_object(), "Expected the value to be object, but the actual type is not", "from_value");
            auto property_count = u32{};
            auto property_enum = std::add_pointer_t<Subprojects::quickjs::JSPropertyEnum>{nullptr};
            Subprojects::quickjs::JS_GetOwnPropertyNames(source._context(), &property_enum, &property_count, source.value(), Subprojects::quickjs::$JS_GPN_STRING_MASK);
            for (const auto index : Range{property_count}) {
                const auto key = Subprojects::quickjs::JS_AtomToCString(source._context(), property_enum[index].atom);
                if (auto element = source.get_property(StringHelper::make_string_view(key)); !element.is_undefined()) {
                    auto make_value = [&element]() -> MapValue {
                        auto value = MapValue{};
                        Trait<MapValue>::from_value(element, value);
                        return value;
                    };
                    destination.emplace(StringHelper::make_string_view(key), make_value());
                }
                Subprojects::quickjs::JS_FreeCString(source._context(), key);
            }
            Subprojects::quickjs::JS_FreePropertyEnum(source._context(), property_enum, property_count);
        }

        static auto to_value(
            HashMap<Key, MapValue>& source,
            Value& destination
        ) -> void {
            destination.set_object();
            for (const auto& [key, value] : source) {
                auto make_value = [&destination, &value]() -> Value {
                    auto element = destination.new_value();
                    Trait<MapValue>::to_value(value, element);
                    return element;
                };
                destination.set_property(key, make_value());
            }
        }

    };

    template <>
    struct Trait<std::string> {

        static auto from_value(
            Value& source,
            std::string& destination
        ) -> void {
            auto size = usize{};
            const auto buffer = Subprojects::quickjs::JS_ToCStringLen(source._context(), &size, source.value());
            destination.assign(buffer, size);
            Subprojects::quickjs::JS_FreeCString(source._context(), buffer);
        }

    };

    template <>
    struct Trait<std::string_view> {

        static auto to_value(
            auto&& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewStringLen(destination._context(), source.data(), source.size()));
        }

    };

    template <>
    struct Trait<StringView> {

        static auto to_value(
            auto&& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewStringLen(destination._context(), source.data(), source.size()));
        }

    };

    template <>
    struct Trait<bool> {

        static auto from_value(
            Value& source,
            bool& destination
        ) -> void {
            assert_conditional(source.is_boolean(), "Expected the value to be boolean, but the actual type is not", "from_value");
            destination = Subprojects::quickjs::JS_ToBool(source._context(), source.value());
        }

        static auto to_value(
            const bool& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewBool(destination._context(), source));
        }

    };

    template <typename T>
    struct Trait<List<T>> {

        static auto from_value(
            Value& source,
            List<T>& destination
        ) -> void {
            assert_conditional(source.is_array(), "Expected the value to be Array, but the actual type is not", "from_value");
            auto length = u32{};
            Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).value());
            destination.allocate(length);
            for (const auto index : Range{static_cast<usize>(length)}) {
                auto value = T{};
                auto current_value = source.get_property(index);
                Trait<T>::from_value(current_value, value);
                destination.append(value);
            }
        }

        static auto to_value(
            const List<T>& source,
            Value& destination
        ) -> void {
            auto args = std::views::transform(source, [&](auto&& e) -> Subprojects::quickjs::JSValue {
                auto value = Value::new_value(destination._context());
                Trait<T>::to_value(e, value);
                return value.release();
            });
            const auto arguments = Array<Subprojects::quickjs::JSValue>{args.begin(), args.end()};
            destination.set_value(Subprojects::quickjs::JS_NewArrayFrom(destination._context(), static_cast<int>(arguments.size()), arguments.begin()));
        }

    };

    template <typename T>
    struct Trait<std::vector<T>> {

        static auto from_value(
            Value& source,
            List<T>& destination
        ) -> void {
            assert_conditional(source.is_array(), "Expected the value to be Array, but the actual type is not", "from_value");
            auto length = u32{};
            Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).value());
            destination.allocate(length);
            for (const auto index : Range{static_cast<usize>(length)}) {
                auto value = T{};
                auto current_value = source.get_property(index);
                Trait<T>::from_value(current_value, value);
                destination.append(value);
            }
        }

        static auto to_value(
            const List<T>& source,
            Value& destination
        ) -> void {
            auto args = std::views::transform(source, [&](auto&& e) -> Subprojects::quickjs::JSValue {
                auto value = Value::new_value(destination._context());
                Trait<T>::to_value(e, value);
                return value.release();
            });
            const auto arguments = Array<Subprojects::quickjs::JSValue>{args.begin(), args.end()};
            destination.set_value(Subprojects::quickjs::JS_NewArrayFrom(destination._context(), static_cast<int>(arguments.size()), arguments.begin()));
        }

    };

    template <typename T>
    struct Trait<CArray<T>> {

        static auto from_value(
            Value& source,
            CArray<T>& destination
        ) -> void {
            assert_conditional(source.is_array(), "Expected the value to be Array, but the actual type is not", "from_value");
            auto length = u32{};
            Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).value());
            destination.allocate(length);
            for (auto index : Range{static_cast<usize>(length)}) {
                auto make_value = [&]() -> T {
                    auto value = T{};
                    auto current_value = source.get_property(index);
                    Trait<T>::from_value(current_value, value);
                    return value;
                };
                destination.append(make_value());
            }
        }

        static auto to_value(
            const CArray<T>& source,
            Value& destination
        ) -> void {
            auto args = std::views::transform(source, [&](auto&& e) -> Subprojects::quickjs::JSValue {
                auto value = Value::new_value(destination._context());
                Trait<T>::to_value(e, value);
                return value.release();
            });
            const auto arguments = Array<Subprojects::quickjs::JSValue>{args.begin(), args.end()};
            destination.set_value(Subprojects::quickjs::JS_NewArrayFrom(destination._context(), static_cast<int>(arguments.size()), arguments.begin()));
        }

    };

    template <typename T, auto N> requires (std::is_same_v<type_of<N>, usize>)
    struct Trait<std::array<T, N>> {

        static auto from_value(
            Value& source,
            std::array<T, N>& destination
        ) -> void {
            assert_conditional(source.is_array(), "Expected the value to be Array, but the actual type is not", "from_value");
            auto length = u32{};
            Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).value());
            assert_conditional(static_cast<u32>(array_size_v<decltype(destination)>) == length, "The array size does not match", "from_value");
            [&]<auto... Indices> requires (std::is_same_v<type_of<Indices>, usize> && ...) (std::index_sequence<Indices...>) -> void {
                ((destination[Indices] = [] (const Value& source_value, const usize& i) -> T {
                    auto value = T{};
                    auto current_value = source_value.get_property(i);
                    Trait<T>::from_value(current_value, value);
                    return value;
                }(source, Indices)), ...);
            }(std::make_index_sequence<array_size_v<decltype(destination)>>{});
        }

        static auto to_value(
            const std::array<T, N>& source,
            Value& destination
        ) -> void {
            destination.set_array();
            [&]<auto... Indices> requires (std::is_same_v<type_of<Indices>, usize> && ...) (std::index_sequence<Indices...>) -> void {
                ((destination.set_property(Indices, [&] () -> Subprojects::quickjs::JSValue {
                    auto value = Value::new_value(destination._context());
                    Trait<T>::to_value(source[Indices], value);
                    return value.release();
                }())), ...);
            }(std::make_index_sequence<array_size_v<decltype(source)>>{});
        }

    };

    template <>
    struct Trait<Value> {

        static auto from_value(
            Value& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_DupValue(source._context(), source.value()));
        }

        static auto to_value(
            const Value& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_DupValue(source._context(), source.value()));
        }

    };

    template <>
    struct Trait<Uint8Array> {

        static auto from_value(
            Value& source,
            Uint8Array& destination
        ) -> void {
            assert_conditional(source.is_array_buffer(), "Expected the value to be ArrayBuffer, but the actual type is not", "from_value");
            auto size = usize{};
            const auto buffer = Subprojects::quickjs::JS_GetArrayBuffer(source._context(), &size, source.value());
            destination.allocate(size);
            std::memcpy(destination.data(), buffer, size);
        }

        static auto to_value(
            const Uint8Array& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewArrayBufferCopy(destination._context(), source.begin(), source.size()));
        }

    };

    template <typename T> requires std::is_class_v<T>
    struct Trait<Pointer<T>> {

        static auto from_value(
            Value& source,
            Pointer<T>& destination
        ) -> void {
            assert_conditional(Detail::class_id<T> != Detail::k_invalid_class_id, "Class ID is not registered", "from_value");
            const auto value = source.value();
            auto opaque = Subprojects::quickjs::JS_GetOpaque(value, static_cast<Subprojects::quickjs::JSClassID>(Detail::class_id<T>));
            assert_conditional(opaque != nullptr, "Expected the value to be opaque, but the actual type is not", "from_value");
            destination = static_cast<T*>(opaque);
        }

        static auto to_value(
            const Pointer<T>& source,
            Value& destination
        ) -> void {
            assert_conditional(source != nullptr, "Expected the value to be opaque, but the actual type is not", "to_value");
            const auto value = Subprojects::quickjs::JS_NewObjectClass(destination._context(), static_cast<int>(Detail::class_id<T>));
            Subprojects::quickjs::JS_SetOpaque(value, source);
            destination.set_value(value);
        }

        static auto to_value(
            Pointer<T>&& source,
            Value& destination
        ) -> void {
            assert_conditional(source != nullptr, "Expected the value to be opaque, but the actual type is not", "to_value");
            const auto value = Subprojects::quickjs::JS_NewObjectClass(destination._context(), static_cast<int>(Detail::class_id<T>));
            Subprojects::quickjs::JS_SetOpaque(value, source);
            destination.set_value(value);
        }
    };

    template <>
    struct Trait<Subprojects::jsoncons::json> {

        static auto from_value (
            Value& source,
            Subprojects::jsoncons::json& destination
        ) -> void {
            switch (source.get_type()) {
                case ValueType::array: {
                    destination = Subprojects::jsoncons::json::array();
                    auto length = u32{};
                    Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).value());
                    destination.resize(length);
                    for (const auto index : Range{length}) {
                        auto current = source.get_property(index);
                        from_value(current, destination[index]);
                    }
                    break;
                }
                case ValueType::object: {
                    destination = Subprojects::jsoncons::json::object();
                    auto property_count = u32{};
                    auto property_enum = std::add_pointer_t<Subprojects::quickjs::JSPropertyEnum>{nullptr};
                    Subprojects::quickjs::JS_GetOwnPropertyNames(source._context(), &property_enum, &property_count, source.value(), Subprojects::quickjs::$JS_GPN_STRING_MASK);
                    destination.resize(property_count);
                    for (const auto index : Range{property_count}) {
                        const auto key = Subprojects::quickjs::JS_AtomToCString(source._context(), property_enum[index].atom);
                        if (auto element = source.get_property(StringHelper::make_string_view(key)); !element.is_undefined()) {
                            auto make_element = [&]() -> Subprojects::jsoncons::json  {
                                auto current_element = Subprojects::jsoncons::json{};
                                from_value(element, current_element);
                                return current_element;
                            };
                            destination[key] = make_element();
                        }
                        Subprojects::quickjs::JS_FreeCString(source._context(), key);
                    }
                    Subprojects::quickjs::JS_FreePropertyEnum(source._context(), property_enum, property_count);
                    break;
                }
                case ValueType::bigint: {
                    auto value = i64{};
                    source.template get<i64>(value);
                    destination = value;
                    break;
                }
                case ValueType::boolean: {
                    auto value = bool{};
                    source.template get<bool>(value);
                    destination = value;
                    break;
                }
                case ValueType::number: {
                    auto value = f64{};
                    source.template get<f64>(value);
                    destination = value;
                    break;
                }
                case ValueType::null: {
                    destination = Subprojects::jsoncons::json::null();
                    break;
                }
                case ValueType::string: {
                    auto value = std::string{};
                    source.template get<std::string>(value);
                    destination = value;
                    break;
                }
                default: {
                    assert_conditional(false, "Unsupported JSON value", "from_value");
                }
            }
        }

        static auto to_value (
            const Subprojects::jsoncons::json& source,
            Value& destination
        ) -> void {
            switch (source.type()) {
                case Subprojects::jsoncons::json_type::array_value: {
                    destination.set_array();
                    auto index = u32{};
                    for (const auto& item : source.array_range()) {
                        auto value = Value::new_value(destination._context());
                        Trait<Subprojects::jsoncons::json>::to_value(item, value);
                        destination.set_property(index, value.release());
                        ++index;
                    }
                    break;
                }
                case Subprojects::jsoncons::json_type::object_value: {
                    destination.set_object();
                    for (auto& [key, value] : source.object_range()) {
                        auto new_value = Value::new_value(destination._context());
                        Trait<Subprojects::jsoncons::json>::to_value(value, new_value);
                        destination.set_property(key, new_value.release());
                    }
                    break;
                }
                case Subprojects::jsoncons::json_type::string_value: {
                    destination.template set<std::string_view>(source.template as<std::string>());
                    break;
                }
                case Subprojects::jsoncons::json_type::bool_value: {
                    destination.template set<bool>(source.template as<bool>());
                    break;
                }
                case Subprojects::jsoncons::json_type::double_value: {
                    destination.template set<double>(source.template as<double>());
                    break;
                }
                case Subprojects::jsoncons::json_type::int64_value: {
                    destination.template set<int64_t>(source.template as<int64_t>());
                    break;
                }
                case Subprojects::jsoncons::json_type::uint64_value: {
                    destination.template set<uint64_t>(source.template as<uint64_t>());
                    break;
                }
                case Subprojects::jsoncons::json_type::null_value: {
                    destination.set_null();
                    break;
                }
                default: {
                    assert_conditional(false, "Unsupported JSON value", "to_value");
                }
            }
        }

    };

    template <>
    struct Trait<Subprojects::jsoncons::json_stream_cursor> {

        static auto to_value (
            Subprojects::jsoncons::json_stream_cursor& source,
            Value& destination
        ) -> void {
            switch (auto& event = source.current(); event.event_type()) {
                case Subprojects::jsoncons::staj_event_type::begin_array: {
                    destination.set_array();
                    auto index = 0_u32;
                    source.next();
                    while (!source.done()) {
                        if (source.current().event_type() == Subprojects::jsoncons::staj_event_type::end_array) {
                            break;
                        }
                        auto value = destination.new_value();
                        to_value(source, value);
                        destination.set_property(index, value.release());
                        ++index;
                        source.next();
                    }
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::begin_object: {
                    destination.set_object();
                    source.next();
                    while (!source.done()) {
                        auto& current_event = source.current();
                        if (current_event.event_type() == Subprojects::jsoncons::staj_event_type::end_object) {
                            break;
                        }
                        auto key = current_event.template get<std::string>();
                        source.next();
                        auto value = destination.new_value();
                        to_value(source, value);
                        destination.set_property(key, value.release());
                        source.next();
                    }
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::null_value: {
                    destination.set_null();
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::bool_value: {
                    destination.template set<bool>(event.template get<bool>());
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::double_value: {
                    destination.template set<f64>(event.template get<f64>());
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::int64_value: {
                    destination.template set<i64>(event.template get<i64>());
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::uint64_value: {
                    destination.template set<u64>(event.template get<u64>());
                    break;
                }
                case Subprojects::jsoncons::staj_event_type::string_value: {
                    destination.template set<std::string_view>(event.template get<std::string>());
                    break;
                }
                default: {
                    assert_conditional(false, "Unsupported JSON value", "to_value");
                }
            }
        }
    };

    template <>
    struct Trait<Subprojects::jsoncons::json_stream_encoder> {

        static auto from_value (
            Value& source,
            Subprojects::jsoncons::json_stream_encoder& destination
        ) -> void {
            switch (source.get_type()) {
                case ValueType::array: {
                    destination.begin_array();
                    auto length = u32{};
                    Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).value());
                    for (const auto index : Range{length}) {
                        auto current = source.get_property(index);
                        from_value(current, destination);
                    }
                    destination.end_array();
                    break;
                }
                case ValueType::object: {
                    destination.begin_object();
                    auto property_count = u32{};
                    auto property_enum = std::add_pointer_t<Subprojects::quickjs::JSPropertyEnum>{nullptr};
                    Subprojects::quickjs::JS_GetOwnPropertyNames(source._context(), &property_enum, &property_count, source.value(), Subprojects::quickjs::$JS_GPN_STRING_MASK);
                    for (const auto index : Range{property_count}) {
                        const auto key = Subprojects::quickjs::JS_AtomToCString(source._context(), property_enum[index].atom);
                        if (auto element = source.get_property(StringHelper::make_string_view(key)); !element.is_undefined()) {
                            destination.key(StringHelper::make_string_view(key));
                            from_value(element, destination);
                        }
                        Subprojects::quickjs::JS_FreeCString(source._context(), key);
                    }
                    Subprojects::quickjs::JS_FreePropertyEnum(source._context(), property_enum, property_count);
                    destination.end_object();
                    break;
                }
                case ValueType::bigint: {
                    auto value = i64{};
                    source.template get<i64>(value);
                    destination.int64_value(value);
                    break;
                }
                case ValueType::boolean: {
                    auto value = bool{};
                    source.template get<bool>(value);
                    destination.bool_value(value);
                    break;
                }
                case ValueType::number: {
                    auto value = f64{};
                    source.template get<f64>(value);
                    destination.double_value(value);
                    break;
                }
                case ValueType::null: {
                    destination.null_value();
                    break;
                }
                case ValueType::string: {
                    auto value = String{};
                    source.template get<String>(value);
                    destination.string_value(value.view());
                    break;
                }
                default: {
                    assert_conditional(false, "Unsupported JSON value", "from_value");
                }
            }
        }
    };

    template <>
    struct Trait<Uint8ArrayView> {

        static auto from_value(
            Value& source,
            Uint8ArrayView& destination
        ) -> void {
            assert_conditional(source.is_array_buffer(), "Expected the value to be ArrayBuffer, but the actual type is not", "from_value");
            auto size = usize{};
            const auto buffer = Subprojects::quickjs::JS_GetArrayBuffer(source._context(), &size, source.value());
            destination = Uint8ArrayView{buffer, size};
        }

        static auto to_value(
            const Uint8ArrayView& source,
            Value& destination
        ) -> void {
            destination.template set_array_buffer_view<false>(source);
        }

        static auto to_value(
            Uint8ArrayView&& source,
            Value& destination
        ) -> void {
            destination.template set_array_buffer_view<true>(source);
        }

    };

}