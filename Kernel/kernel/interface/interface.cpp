#include "kernel/interface/runtime.hpp"
#include "kernel/interface/context.hpp"
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
        auto context = Context{service};
        Runtime::make_environment(context);
        auto arguments = List<String>{};
        destruct_message(message, arguments);
        {
            auto clock = Clock{};
            clock.start_safe();
            auto source = Uint8Array{};
            FileSystem::read_file("C:/Users/Admin/Downloads/Telegram Desktop/main.889.com.ea.game.pvz2_row.obb.bin"_s, source);
            auto destination = Uint8Array{};
            Encryption::Base64::Decode::process(steal_reference<UCharacterArray>(source), destination);
            FileSystem::write_file("C:/Users/Admin/Downloads/Telegram Desktop/main.889.com.ea.game.pvz2_row.obb.1"_s, destination);
            clock.stop_safe();
            debug(fmt::format("Time spent: {}s", clock.duration_as_seconds()));
        }
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
        std::invoke(*service->callback, error.get(), nullptr);
    }
    return 0;
}