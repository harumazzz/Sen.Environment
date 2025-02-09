#pragma once

#include "kernel/interface/context.hpp"
#include "kernel/interface/api/method.hpp"
#include "kernel/utility/javascript/proxy.hpp"

namespace Sen::Kernel::Interface::Runtime {

    inline auto make_environment (
        Context& context
    ) -> void {
        context.context().global_object().define_property(
            "version"_s,
            Javascript::Value::new_ref(context.context().context(),
                JS_NewCFunction2(context.context().context(), &Javascript::proxy_native_function<Kernel::Interface::API::version>, "version",
                0, JS_CFUNC_generic, int{JS_PROP_C_W_E}))
            );
    }

}		