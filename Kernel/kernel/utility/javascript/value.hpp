#pragma once

#include "js_string.hpp"
#include "kernel/utility/javascript/common.hpp"

namespace Sen::Kernel::Javascript {

    struct Value {

        protected:

            Pointer<Subprojects::quickjs::JSContext> m_context;

            Subprojects::quickjs::JSValue m_value;

            explicit Value (
                const Pointer<Subprojects::quickjs::JSContext>& context,
                const Subprojects::quickjs::JSValue& value
            ) : m_context{context}, m_value{value} {

            }

            explicit Value (
                const Pointer<Subprojects::quickjs::JSContext>& context
            ) : Value{context, Subprojects::quickjs::$JS_UNINITIALIZED} {

            }

        public:

            inline static auto new_ref (
                const Pointer<JSContext> &context,
                const Subprojects::quickjs::JSValue &other
            ) -> Value;

            inline static auto new_value (
                const Pointer<JSContext> &context
            ) -> Value;

            ~Value (

            ) {
                if (thiz.m_context != nullptr) {
                    Subprojects::quickjs::JS_FreeValue(thiz.m_context, thiz.m_value);
                }
            }

            auto operator=(
                const Value &other
            ) -> Value& = delete;

            Value(
                const Value &other
            ) = delete;

            auto operator=(
                Value &&other
            ) noexcept -> Value& {
                thiz.m_context = other.m_context;
                thiz.m_value = other.m_value;
                other.m_context = nullptr;
                other.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
                return thiz;
            }

            Value (
                Value &&other
            ) noexcept : m_context{other.m_context}, m_value{other.m_value} {
                other.m_context = nullptr;
                other.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
            }

            auto is_uninitialized(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsUninitialized(thiz.m_value));
            }

            auto _context (

            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

            auto is_undefined(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsUndefined(thiz.m_value));
            }

            auto is_array_buffer(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsArrayBuffer(thiz.m_value));
            }

            auto is_null(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsNull(thiz.m_value));
            }

            auto is_bigint(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsBigInt(thiz.m_context, thiz.m_value));
            }

            auto is_function(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsFunction(thiz.m_context, thiz.m_value));
            }

            auto is_promise(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsPromise(thiz.m_value));
            }

            auto is_exception(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsException(thiz.m_value));
            }

            auto is_boolean(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsBool(thiz.m_value));
            }

            auto is_number(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsNumber(thiz.m_value));
            }

            auto is_string(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsString(thiz.m_value));
            }

            auto is_object(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsObject(thiz.m_value));
            }

            auto is_array(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsArray(thiz.m_context, thiz.m_value));
            }

            auto set_value (
                const JSValue& new_value
            ) -> void
            {
                if (thiz.m_context != nullptr) {
                    Subprojects::quickjs::JS_FreeValue(thiz.m_context, thiz.m_value);
                }
                thiz.m_value = new_value;
            }

            auto release (
            ) -> JSValue
            {
                const auto old_value = thiz.m_value;
                thiz.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
                return old_value;
            }

            template <typename T>
            auto set (
                T&& value
            ) -> void {
                return Trait<T>::from_value(thiz, value);
            }

            template <typename T>
            auto get (
                T& value
            ) -> void {
                return Trait<T>::to_value(std::forward<T>(value), thiz);
            }

            auto value (
            ) const -> JSValue {
                return thiz.m_value;
            }

            auto set_object(

            ) -> void
            {
                return thiz.set_value(Subprojects::quickjs::JS_NewObject(thiz.m_context));
            }

            auto set_array(

            ) -> void
            {
                return thiz.set_value(Subprojects::quickjs::JS_NewArray(thiz.m_context));
            }

            auto define_property(
                const String& name,
                Value&& value
            ) const -> void
            {
                auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                Subprojects::quickjs::JS_DefinePropertyValue(thiz.m_context, thiz.m_value, atom, value.release(), Subprojects::quickjs::$JS_PROP_C_W_E);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            auto define_property(
                const u32 index,
                Value&& value
            ) const -> void
            {
                auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                Subprojects::quickjs::JS_DefinePropertyValue(thiz.m_context, thiz.m_value, atom, value.release(), Subprojects::quickjs::$JS_PROP_C_W_E);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            auto set_property(
                const String& name,
                Value&& value
            ) const -> void
            {
                auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                Subprojects::quickjs::JS_SetProperty(thiz.m_context, thiz.m_value, atom, value.release());
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            auto set_property(
                const u32 index,
                Value&& value
            ) const -> void
            {
                auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                Subprojects::quickjs::JS_SetProperty(thiz.m_context, thiz.m_value, atom, value.release());
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            auto get_property(
                const String& name
            ) const -> Value
            {
                auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                auto value = new_ref(thiz.m_context, JS_GetProperty(thiz.m_context, thiz.m_value, atom));
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return value;
            }

            auto get_property(
                const u32 index
            ) const -> Value
            {
                auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                auto value = new_ref(thiz.m_context, JS_GetProperty(thiz.m_context, thiz.m_value, atom));
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return value;
            }

    };

    inline auto JSString::assign_from (
        const Value& other
    ) -> void {
        thiz.m_context = other._context();
        thiz.m_value = Subprojects::quickjs::JS_DupValue(thiz.m_context, other.value());
        thiz.value = const_cast<char*>(Subprojects::quickjs::JS_ToCStringLen(thiz.m_context, &thiz._size, thiz.m_value));
    }

}