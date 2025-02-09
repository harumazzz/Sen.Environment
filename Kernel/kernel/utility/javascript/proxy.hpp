#pragma once

#include "context.hpp"
#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"

namespace Sen::Kernel::Javascript {

    template <auto Callable> requires is_global_function_v<Callable> &&
        std::is_same_v<typename is_global_function<std::decay_t<type_of<Callable>>>::Arguments, std::tuple<Context&, Value&, Array<Value>&, Value&>> &&
        std::is_void_v<typename is_global_function<std::decay_t<type_of<Callable>>>::ReturnType>
    inline auto proxy_native_function (
        Subprojects::quickjs::JSContext* ctx,
        Subprojects::quickjs::JSValue this_val,
        int argc,
        Subprojects::quickjs::JSValue* argv
    ) -> Subprojects::quickjs::JSValue {
        auto result = Value::new_value(ctx);
        auto context = Context::new_ref(ctx);
        auto value = Value::new_ref(ctx, this_val);
        auto arguments = Array<Value>{static_cast<usize>(argc)};
        for (auto i = 0; i < argc; ++i) {
            arguments[i] = Value::new_ref(ctx, argv[i]);
        }
        Callable(context, value, arguments, result);
        return result.release();
    }

}