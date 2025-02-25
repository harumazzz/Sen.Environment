﻿#include "kernel/interface/context.hpp"
#include "kernel/interface/runtime.hpp"
using namespace Sen::Kernel;
using namespace Sen::Kernel::Interface;

M_EXPORT_API
auto execute (
    Message* message,
    Service* service
) -> int
{
    try {
        std::setlocale(LC_ALL, "C");
        Runtime::bind_service(service);
        auto context = Context{service};
        Runtime::make_environment(context);
        auto arguments = CArray<String>{};
        destruct_message(message, arguments);
        const auto main = context.evaluate_fs(arguments[2]);
        const auto result = main.call(as_lvalue(context.make_argument(as_move(arguments))));
        while (context.runtime().has_promise()) {
            context.runtime().call_promise(as_lvalue(Context::new_context()));
        }
        assert_conditional(result.is_undefined(), "Thread has not been completed", "execute");
    }
    catch(...) {
        auto exception_message = Runtime::make_exception();
        auto error = std::unique_ptr<Message, decltype(&free_message)>{nullptr, &free_message};
        construct_message(exception_message, error);
        std::invoke(*service->callback, service, error.get(), nullptr);
    }
    return 0;
}