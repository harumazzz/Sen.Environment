#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/js_string.hpp"

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

            constexpr explicit Value(
            ) : m_context{nullptr}, m_value{Subprojects::quickjs::$JS_UNINITIALIZED} {

            }

            inline static auto new_ref (
                const Pointer<JSContext> &context,
                const Subprojects::quickjs::JSValue &other
            ) -> Value;

            inline static auto new_value (
                const Pointer<JSContext> &context
            ) -> Value;

            inline static auto new_owner (
                const Pointer<JSContext> &context,
                const Subprojects::quickjs::JSValue& other
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
                if (this != &other) {
                    thiz.m_context = other.m_context;
                    thiz.m_value = other.m_value;
                    other.m_context = nullptr;
                    other.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
                }
                return thiz;
            }

            Value (
                Value &&other
            ) noexcept : m_context{other.m_context}, m_value{other.m_value} {
                other.m_context = nullptr;
                other.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
            }

            inline auto is_uninitialized(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsUninitialized(thiz.m_value));
            }

            inline auto _context (

            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

            inline auto is_undefined(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsUndefined(thiz.m_value));
            }

            inline auto is_array_buffer(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsArrayBuffer(thiz.m_value));
            }

            inline auto is_null(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsNull(thiz.m_value));
            }

            inline auto is_bigint(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsBigInt(thiz.m_context, thiz.m_value));
            }

            inline auto is_function(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsFunction(thiz.m_context, thiz.m_value));
            }

            inline auto is_promise(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsPromise(thiz.m_value));
            }

            inline auto is_exception(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsException(thiz.m_value));
            }

            inline auto is_boolean(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsBool(thiz.m_value));
            }

            inline auto is_number(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsNumber(thiz.m_value));
            }

            inline auto is_string(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsString(thiz.m_value));
            }

            inline auto is_object(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsObject(thiz.m_value));
            }

            inline auto is_object_of_object (

            ) const -> bool
            {
                if (!thiz.is_object()) {
                    return false;
                }
                auto name = String{};
                auto name_property = thiz.get_property("constructor"_sv).get_property("name"_sv);
                name_property.template get<String>(name);
                return static_cast<bool>(name == "Object"_sv);
            }

            inline auto is_array(

            ) const -> bool
            {
                return static_cast<bool>(Subprojects::quickjs::JS_IsArray(thiz.m_context, thiz.m_value));
            }

            inline auto set_value (
                const Subprojects::quickjs::JSValue& new_value
            ) -> void
            {
                if (thiz.m_context != nullptr) {
                    Subprojects::quickjs::JS_FreeValue(thiz.m_context, thiz.m_value);
                }
                thiz.m_value = new_value;
            }

            inline auto set_array_buffer (
                const Uint8Array& value
            ) -> void
            {
                thiz.set_value(Subprojects::quickjs::JS_NewArrayBufferCopy(thiz.m_context, value.cbegin(), value.size()));
            }

            template <auto is_new_owner> requires std::is_same_v<type_of<is_new_owner>, bool>
            inline auto set_array_buffer_view (
                const Uint8ArrayView& value
            ) -> void
            {
                thiz.set_value(Subprojects::quickjs::JS_NewArrayBuffer(
                    thiz.m_context,
                    value.cbegin(),
                    value.size(),
                    !is_new_owner ? nullptr : [](Subprojects::quickjs::JSRuntime* rt, void * opaque, void * ptr) -> void {
                        delete[] static_cast<std::uint8_t*>(ptr);
                    },
                    nullptr,
                    static_cast<int>(false)
                ));
            }

            inline auto release (
            ) -> Subprojects::quickjs::JSValue
            {
                const auto old_value = thiz.m_value;
                thiz.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
                return old_value;
            }

            template <typename T>
            inline auto set (
                T&& value
            ) -> void {
                return Trait<T>::to_value(std::forward<T>(value), thiz);
            }

            template <typename T>
            inline auto set (
                T& value
            ) -> void {
                return Trait<T>::to_value(value, thiz);
            }

            template <typename T>
            inline auto get (
                T& value
            ) -> void {
                return Trait<T>::from_value(thiz, value);
            }

            inline auto value (
            ) const -> Subprojects::quickjs::JSValue {
                return thiz.m_value;
            }

            inline auto set_object(

            ) -> void
            {
                return thiz.set_value(Subprojects::quickjs::JS_NewObject(thiz.m_context));
            }

            inline auto set_undefined(

            ) -> void
            {
                return thiz.set_value(Subprojects::quickjs::$JS_UNDEFINED);
            }

            inline auto set_null(

            ) -> void
            {
                return thiz.set_value(Subprojects::quickjs::$JS_NULL);
            }

            inline auto set_array(

            ) -> void
            {
                return thiz.set_value(Subprojects::quickjs::JS_NewArray(thiz.m_context));
            }

            inline auto new_object (

            ) const -> Value
            {
                return Value{thiz.m_context, Subprojects::quickjs::JS_NewObject(thiz.m_context)};
            }

            inline auto new_value (

            ) const -> Value
            {
                return Value{thiz.m_context, Subprojects::quickjs::$JS_UNINITIALIZED};
            }

            inline auto define_property(
                const String& name,
                Value&& value
            ) const -> void
            {
                return thiz.define_property(name, value.release());
            }

            inline auto define_property(
                const String& name,
                const Subprojects::quickjs::JSValue& value
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                Subprojects::quickjs::JS_DefinePropertyValue(thiz.m_context, thiz.m_value, atom, value, Subprojects::quickjs::$JS_PROP_C_W_E);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto define_property(
                const u32 index,
                Value&& value
            ) const -> void
            {
                return thiz.define_property(index, value.release());
            }

            inline auto define_property(
                const u32 index,
                const Subprojects::quickjs::JSValue& value
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                Subprojects::quickjs::JS_DefinePropertyValue(thiz.m_context, thiz.m_value, atom, value, Subprojects::quickjs::$JS_PROP_C_W_E);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto define_property(
                const String& value,
                Value&& getter,
                Value&& setter
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, value.cbegin(), value.size());
                Subprojects::quickjs::JS_DefinePropertyGetSet(thiz.m_context, thiz.m_value, atom, getter.release(), setter.release(), Subprojects::quickjs::$JS_PROP_C_W_E);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto set_property(
                const String& name,
                Value&& value
            ) const -> void
            {
                return thiz.set_property(name, value.release());
            }

            inline auto set_property(
                const String& name,
                const Subprojects::quickjs::JSValue& value
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                Subprojects::quickjs::JS_SetProperty(thiz.m_context, thiz.m_value, atom, value);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto set_property(
                const std::string_view& name,
                const Subprojects::quickjs::JSValue& value
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.data(), name.size());
                Subprojects::quickjs::JS_SetProperty(thiz.m_context, thiz.m_value, atom, value);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto set_property(
                const u32 index,
                Value&& value
            ) const -> void
            {
                return thiz.set_property(index, value.release());
            }

            inline auto set_property(
                const u32 index,
                const Subprojects::quickjs::JSValue& value
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                Subprojects::quickjs::JS_SetProperty(thiz.m_context, thiz.m_value, atom, value);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto get_property(
                const String& name
            ) const -> Value
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                auto value = new_owner(thiz.m_context, Subprojects::quickjs::JS_GetProperty(thiz.m_context, thiz.m_value, atom));
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return value;
            }

            inline auto get_property(
                const std::string_view& name
            ) const -> Value
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.data(), name.size());
                auto value = new_owner(thiz.m_context, Subprojects::quickjs::JS_GetProperty(thiz.m_context, thiz.m_value, atom));
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return value;
            }

            inline auto get_property(
                const u32 index
            ) const -> Value
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                auto value = new_owner(thiz.m_context, Subprojects::quickjs::JS_GetProperty(thiz.m_context, thiz.m_value, atom));
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return value;
            }

            inline auto has_property (
                const String& name
            ) const -> bool
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                const auto value = Subprojects::quickjs::JS_HasProperty(thiz.m_context, thiz.m_value, atom);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return static_cast<bool>(value);
            }

            inline auto has_property (
                const u32 index
            ) const -> bool
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                const auto value = Subprojects::quickjs::JS_HasProperty(thiz.m_context, thiz.m_value, atom);
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
                return static_cast<bool>(value);
            }

            inline auto delete_property (
                const u32 index
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomUInt32(thiz.m_context, index);
                Subprojects::quickjs::JS_DeleteProperty(thiz.m_context, thiz.m_value, atom, int{Subprojects::quickjs::$JS_PROP_THROW});
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto delete_property (
                const String& name
            ) const -> void
            {
                const auto atom = Subprojects::quickjs::JS_NewAtomLen(thiz.m_context, name.cbegin(), name.size());
                Subprojects::quickjs::JS_DeleteProperty(thiz.m_context, thiz.m_value, atom, int{Subprojects::quickjs::$JS_PROP_THROW});
                Subprojects::quickjs::JS_FreeAtom(thiz.m_context, atom);
            }

            inline auto context (
            ) const -> Context;

            template <auto function, auto is_constructor> requires is_global_function_v<function> &&
                std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
                std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType> && std::is_same_v<type_of<is_constructor>, bool>
            inline auto add_function(
                const String& name
            ) const -> void;

            template <auto function, auto is_constructor> requires is_global_function_v<function> &&
                std::is_same_v<typename is_global_function<std::decay_t<type_of<function>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
                std::is_void_v<typename is_global_function<std::decay_t<type_of<function>>>::ReturnType> && std::is_same_v<type_of<is_constructor>, bool>
            inline auto set_function (
                const String& name
            ) -> void;

            inline auto set_date (
                const f64& value
            ) -> void {
                thiz.set_value(Subprojects::quickjs::JS_NewDate(thiz.m_context, value));
            }

            inline auto set_promise (
                const Detail::Promise& promise
            ) -> void {
                auto resolving_functions = std::array<Subprojects::quickjs::JSValue, 2>{promise.resolve, promise.reject};
                thiz.set_value(Subprojects::quickjs::JS_NewPromiseCapability(thiz.m_context, resolving_functions.data()));
            }

            inline auto call (
                List<Value>& value
            ) const -> Value {
                assert_conditional(thiz.is_function(), "Expected the value to be function, but the actual type is not", "call");
                auto args = std::views::transform(value, [](auto&& arg) -> Subprojects::quickjs::JSValue {
                    return arg.release();
                });
                auto arguments = Array<Subprojects::quickjs::JSValue>{args.begin(), args.end()};
                const auto result = Subprojects::quickjs::JS_Call(thiz.m_context, thiz.m_value, JS_UNDEFINED, static_cast<int>(arguments.size()), arguments.data());
                return new_owner(thiz.m_context, result);
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