#pragma once

#include "kernel/interface/context.hpp"
#include "kernel/interface/runtime.hpp"

namespace Sen::Kernel::Interface::Executor {

    template <typename Callable>
    inline auto handle_exception (
        Service* service,
        Callable&& callable
    ) -> void {
        auto exception_message = callable();
        auto error = std::unique_ptr<Message, decltype(&free_message)>{nullptr, &free_message};
        construct_message(exception_message, error);
        std::invoke(*service->callback, service, error.get(), nullptr);
    }

    inline auto execute (
        Message* message,
        Service* service
    ) -> void {
        try {
            std::setlocale(LC_ALL, "C");
            auto context = Context{service};
            Runtime::make_environment(context);
            auto arguments = CArray<String>{};
            destruct_message(message, arguments);
            const auto main = context.evaluate_fs(arguments[2]);
            const auto result = main.call(as_lvalue(context.make_argument(as_move(arguments))));
            while (context.runtime().has_promise()) {
                context.runtime().call_promise(as_lvalue(Context::new_context()));
            }
        }
        catch(...) {
            handle_exception(service, Runtime::make_runtime_exception);
        }
    }

    #if WINDOWS
    inline static auto constexpr error_code = EXCEPTION_EXECUTE_HANDLER;
    #else
    inline auto jump_buffer = std::jmp_buf{};
    using SignalHandler = struct sigaction;
    #endif

    template <typename Callable>
    inline auto spawn (
        Service* service,
        Callable&& executor
    ) -> void {

        #if WINDOWS
        __try {
            executor.operator()();
        }
        __except (error_code) {
            handle_exception(service, Runtime::make_unhandled_exception);
        }
        #else
        auto sa = SignalHandler{};
        sa.sa_handler = [](int signal) -> void {
            std::longjmp(jump_buffer, 1);
        };
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGSEGV, &sa, nullptr);
        sigaction(SIGFPE, &sa, nullptr);
        sigaction(SIGBUS, &sa, nullptr);
        if (setjmp(jump_buffer) == 0) {
            executor.operator()();
        } else {
            handle_exception(service, Runtime::make_unhandled_exception);
        }
        #endif
    }

}