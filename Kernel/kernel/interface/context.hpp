#pragma once

#include "kernel/interface/api/method.hpp"
#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/interface/shell.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::Interface {

    class Context {

    protected:

        Javascript::Runtime m_runtime;

        Javascript::Context m_context;

        bool m_module;

    public:

        explicit Context(
        ) : m_runtime{Javascript::Runtime::new_instance()},
            m_context{m_runtime.context()},
            m_module{false}
        {

        }

        auto set_module (
            const bool value
        ) -> void {
            m_module = value;
        }

        auto get_module (
        ) const -> bool {
            return m_module;
        }

        auto runtime (
        ) -> Javascript::Runtime& {
            return thiz.m_runtime;
        }

        auto context (
        ) -> Javascript::Context& {
            return thiz.m_context;
        }

        auto evaluate (
            const String& source,
            const String& name
        ) const -> Javascript::Value {
            return thiz.m_context.evaluate(source, name, thiz.m_module);
        }

        auto execute (
        ) -> void {
        }

        ~Context(
        ) = default;

    };

}		