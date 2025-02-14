#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"

namespace Sen::Kernel::Javascript {

    template <typename T> requires is_numeric_v<T> && !is_boolean_v<T>
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
    struct Trait<JSString> {

        static auto from_value(
            Value& source,
            JSString& destination
        ) -> void {
            assert_conditional(source.is_string(), "Expected the value to be string, but the actual type is not", "from_value");
            destination.assign_from(source);
        }

        static auto to_value(
            const JSString& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewStringLen(destination._context(), source.cbegin(), source.size()));
        }

    };

    template <>
    struct Trait<String> {

        static auto from_value(
            Value& source,
            String& destination
        ) -> void {
            assert_conditional(source.is_string(), "Expected the value to be string, but the actual type is not", "from_value");
            auto size = usize{};
            auto buffer = Subprojects::quickjs::JS_ToCStringLen(source._context(), &size, source.value());
            auto movable_string = String{buffer, size};
            Subprojects::quickjs::JS_FreeCString(source._context(), buffer);
            destination.take_ownership(movable_string);
        }

        static auto to_value(
            auto&& source,
            Value& destination
        ) -> void {
            destination.set_value(Subprojects::quickjs::JS_NewStringLen(destination._context(), source.cbegin(), source.size()));
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
    struct Trait<CList<T>> {

        static auto from_value(
            Value& source,
            CList<T>& destination
        ) -> void {
            assert_conditional(source.is_array(), "Expected the value to be Array, but the actual type is not", "from_value");
            auto length = u32{};
            Subprojects::quickjs::JS_ToUint32(source._context(), &length, source.get_property("length"_s).release());
            destination.allocate(length);
            for (auto index : Range{static_cast<usize>(length)}) {
                auto value = T{};
                auto current_value = source.get_property(index);
                Trait<T>::from_value(current_value, value);
                destination.append(value);
            }
        }

        static auto to_value(
            const CList<T>& source,
            Value& destination
        ) -> void {
            destination.set_array();
            // TODO : Refactor code with quickjs api : 0.9.0 for fast array
            for (auto index : Range{source.size()}) {
                auto value = Value::new_value(destination._context());
                Trait<T>::to_value(source[index], value);
                destination.set_property(index, value.release());
            }
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
            destination.set_value(Subprojects::quickjs::JS_NewArrayBufferCopy(destination._context(), source.cbegin(), source.size()));
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
            auto value = Subprojects::quickjs::JS_NewObjectClass(destination._context(), static_cast<int>(Detail::class_id<T>));
            Subprojects::quickjs::JS_SetOpaque(value, source);
            destination.set_value(value);
        }
    };

}