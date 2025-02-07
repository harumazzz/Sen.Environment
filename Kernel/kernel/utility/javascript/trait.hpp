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
                assert_conditional(source.is_number(), "Expected the value to be number, but the actual type is not", "from_value");
                Subprojects::quickjs::JS_ToFloat64(source._context(), &destination, source.value());
            }
            else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T>) {
                assert_conditional(source.is_bigint(), "Expected the value to be bigint, but the actual type is not", "from_value");
                Subprojects::quickjs::JS_ToBigInt64(source._context(), &destination, source.value());
            }
            else {
                assert_conditional(source.is_bigint(), "Expected the value to be bigint, but the actual type is not", "from_value");
                Subprojects::quickjs::JS_ToBigUint64(source._context(), &destination, source.value());
            }
        }

        static auto to_value(
            T& source,
            Value& destination
        ) -> void {
            if constexpr (std::is_floating_point_v<T>) {
                destination = Subprojects::quickjs::JS_NewFloat64(source._context(), source);
            }
            else if constexpr (std::is_integral_v<T> && !std::is_unsigned_v<T>) {
                destination = Subprojects::quickjs::JS_NewBigInt64(source._context(), source);
            }
            else {
                destination = Subprojects::quickjs::JS_NewBigUint64(source._context(), source);
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
            JSString& source,
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
            bool& source,
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
            source.get_property("length"_s).template get<u32>(length);
            destination.allocate(length);
            auto context = source._context();
            for (auto index : Range{length}) {
                destination.append(Value::new_ref(context, source.get_property(index).get<T>()));
            }
        }

        static auto to_value(
            CList<T>& source,
            Value& destination
        ) -> void {
            destination.set_array();
            // TODO : Refactor code with quickjs api : 0.9.0 for fast array
            for (auto index : Range{source.size()}) {
                destination.define_property(index, Value::new_ref(destination._context(), source[index]));
            }
        }

    };

}