#include "kernel/interface/executor.hpp"

using namespace Sen::Kernel::Interface;

M_EXPORT_API
auto execute (
    Message* message,
    Service* service
) -> int {
    Runtime::bind_service(service);
    Executor::spawn(
        service,
        [&]() -> void {
            return Executor::execute(message, service);
        }
    );
    Runtime::unbind_service(service);
    return 0;
}