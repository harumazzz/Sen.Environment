#pragma once

#include "value.hpp"
#include "kernel/utility/javascript/common.hpp"

namespace Sen::Kernel::Javascript {

    struct Context {

        protected:

            Pointer<Subprojects::quickjs::JSContext> m_context;

            bool m_owner;

            explicit Context(
                const Pointer<Subprojects::quickjs::JSContext>& context,
                const bool owner
            ) : m_context{context}, m_owner{owner} {

            }

        public:

            Context (
                Context&& other
            ) noexcept : m_context{other.m_context}, m_owner{other.m_owner} {
                other.m_context = nullptr;
                other.m_owner = false;
            }

            auto operator = (
                Context&& other
            ) noexcept -> Context & {
                thiz.m_context = other.m_context;
                thiz.m_owner = other.m_owner;
                other.m_context = nullptr;
                other.m_owner = false;
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

}