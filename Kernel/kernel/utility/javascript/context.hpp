#pragma once

#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/common.hpp"

namespace Sen::Kernel::Javascript {

    struct Context {

        protected:

            Pointer<Subprojects::quickjs::JSContext> m_context;

            bool m_module;

            bool m_owner;

            explicit Context(
                const Pointer<Subprojects::quickjs::JSContext>& context,
                const bool owner
            ) : m_context{context}, m_module{false}, m_owner{owner} {

            }

        public:

            Context (
                Context&& other
            ) noexcept : m_context{other.m_context}, m_module{other.m_module}, m_owner{other.m_owner} {
                other.m_context = nullptr;
                other.m_owner = false;
                other.m_module = false;
            }

            auto operator = (
                Context&& other
            ) noexcept -> Context & {
                thiz.m_context = other.m_context;
                thiz.m_owner = other.m_owner;
                thiz.m_module = other.m_module;
                other.m_context = nullptr;
                other.m_owner = false;
                other.m_module = false;
                return thiz;
            }

            auto operator = (
                const Context & that
            ) -> Context & = delete;

            Context (
                const Context & other
            ) = delete;

            ~Context(
            ) {
                if (thiz.m_owner) {
                    Subprojects::quickjs::JS_FreeContext(thiz.m_context);
                }
            }

            inline static auto new_context (
                const Pointer<Subprojects::quickjs::JSRuntime>& runtime
            ) -> Context {
                return Context{Subprojects::quickjs::JS_NewContext(runtime), true};
            }

            inline auto runtime (
            ) const -> Runtime;

            inline auto context (
            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

            inline auto throw_exception (
                Value&& value
            ) const -> void {
                Subprojects::quickjs::JS_Throw(thiz.m_context, value.release());
            }

            inline static auto new_ref (
                const Pointer<JSContext>& context
            ) -> Context {
                return Context{context, false};
            }

            auto set_module (
                const bool value
            ) -> void {
                thiz.m_module = value;
            }

            auto get_module (
            ) const -> bool {
                return thiz.m_module;
            }

            inline auto evaluate (
                const StringView& source,
                const StringView& name
            ) const -> Value {
                const auto result = Subprojects::quickjs::JS_Eval(
                    thiz.m_context,
                    source.begin(),
                    source.size(),
                    name.begin(),
                   thiz.m_module ? Subprojects::quickjs::$JS_EVAL_TYPE_MODULE : Subprojects::quickjs::$JS_EVAL_TYPE_GLOBAL | Subprojects::quickjs::$JS_EVAL_FLAG_STRICT
                );
                return Value::new_owner(thiz.m_context, result);
            }

            inline auto global_object (

            ) const -> Value {
                return Value::new_owner(thiz.m_context, Subprojects::quickjs::JS_GetGlobalObject(thiz.m_context));
            }

            inline auto catch_exception (
            ) const -> Value {
                return Value::new_owner(thiz.m_context, Subprojects::quickjs::JS_GetException(thiz.m_context));
            }

            inline auto get_class_proto (
                const u32& id
            ) const -> Value {
                return Value::new_owner(thiz.m_context, Subprojects::quickjs::JS_GetClassProto(thiz.m_context, static_cast<Subprojects::quickjs::JSClassID>(id)));
            }

            inline auto set_class_proto (
                const u32& id,
                Value && value
            ) const -> void {
                Subprojects::quickjs::JS_SetClassProto(thiz.m_context, static_cast<Subprojects::quickjs::JSClassID>(id), value.release());
            }

            inline auto new_value (
            ) const -> Value {
                return Value::new_value(thiz.m_context);
            }

            template<typename T>
            inline auto set_opaque (
                const Pointer<T>& opaque
            ) const -> void {
                Subprojects::quickjs::JS_SetContextOpaque(thiz.m_context, opaque);
            }

            template<typename T>
            inline auto get_opaque (

            ) const -> Pointer<T> {
                auto opaque = Subprojects::quickjs::JS_GetContextOpaque(thiz.m_context);
                assert_not_null(opaque != nullptr, "Expected the value to be opaque, but the actual type is not", "get_opaque");
                return static_cast<T*>(opaque);
            }

    };

    inline auto Value::new_ref (
        const Pointer<JSContext>& context,
        const Subprojects::quickjs::JSValue& other
    ) -> Value {
        return Value{context, Subprojects::quickjs::JS_DupValue(context, other)};
    }

    inline auto Value::new_value (
        const Pointer<JSContext>& context
    ) -> Value {
        return Value{context};
    }

    inline auto Value::new_owner (
        const Pointer<JSContext>& context,
        const Subprojects::quickjs::JSValue& other
    ) -> Value {
        return Value{context, other};
    }

    inline auto Value::context(
    ) const -> Context {
        return Context::new_ref(thiz.m_context);
    }


}