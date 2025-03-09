#include "kernel/interface/executor.hpp"

using Message = Sen::Kernel::Interface::Message;
using Service = Sen::Kernel::Interface::Service;
M_EXPORT_API
auto execute (
    Message* message,
    Service* service
) -> int {
    Sen::Kernel::Interface::Runtime::bind_service(service);
    Sen::Kernel::Interface::Executor::safe_executor(
            service,
    [&]() -> void {
                Sen::Kernel::Interface::Executor::execute(message, service);
            }
    );
    Sen::Kernel::Interface::Runtime::unbind_service(service);
    return 0;
}