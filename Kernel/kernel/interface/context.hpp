#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Interface {

    class Context {

    protected:

        Javascript::Runtime m_runtime;

        Javascript::Context m_context;

    public:

        explicit Context(
            const Pointer<Service>& service
        ) : m_runtime{Javascript::Runtime::new_instance()},
            m_context{m_runtime.context()}
        {
            thiz.m_context.set_opaque<Service>(service);
        }

        auto runtime (
        ) -> Javascript::Runtime& {
            return thiz.m_runtime;
        }

        auto context (
        ) -> Javascript::Context& {
            return thiz.m_context;
        }

        [[nodiscard]] auto evaluate (
            const StringView& source,
            const StringView& name
        ) const -> Javascript::Value {
            return thiz.m_context.evaluate(source, name);
        }

        auto evaluate_fs (
            const StringView& source
        ) const -> Javascript::Value {
            auto script = String{};
            FileSystem::read_file(source, script);
            return thiz.evaluate(script, source);
        }

        static auto new_context (
        ) -> Javascript::Context {
            return Javascript::Context::new_ref(nullptr);
        }

        auto make_argument (
            List<String>&& arguments
        ) -> List<Javascript::Value> {
            auto make_data = [&]() -> Javascript::Value {
                auto data = Javascript::Value::new_value(thiz.context().context());
                data.set_object();
                auto set_home = [&]() -> void {
                    auto home = data.new_value();
                    home.template set<std::string_view>(arguments[2].view());
                    data.define_property("home"_s, home.release());
                };
                auto set_argument = [&]() -> void {
                    auto argument = data.new_value();
                    argument.template set<List<String>>(as_move(arguments));
                    data.define_property("arguments"_s, argument.release());
                };
                auto set_error = [&]() -> void {
                    auto error = data.new_value();
                    error.set_undefined();
                    data.define_property("error"_s, error.release());
                };
                set_home();
                set_argument();
                set_error();
                return data;
            };
            auto result = List<Javascript::Value>{1_size};
            result.append(make_data());
            return result;
        }

        ~Context(
        ) = default;

    };

}		