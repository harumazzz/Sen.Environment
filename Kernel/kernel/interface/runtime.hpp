#pragma once

#include "kernel/interface/context.hpp"
#include "kernel/interface/api/method.hpp"
#include "kernel/utility/javascript/proxy.hpp"

namespace Sen::Kernel::Interface::Runtime {


    inline auto make_environment (
        Context& context
    ) -> void {
        context.context().global_object().add_function<&Javascript::proxy_native_function_wrapper<&API::version>>(
            "version"_s,
            false
        );
        context.context().global_object().add_function<&Javascript::proxy_native_function_wrapper<&FileSystem::is_file>>(
            "is_file"_s,
            false
        );
        context.context().global_object().add_function<
            Javascript::normalize_lambda_to_function<[]() { return 42_size; }>()
        >(
            "double_x"_s,
            false
        );

    }

}		