#include "kernel/interface/callback.hpp"
using namespace Sen::Kernel;
using namespace Sen::Kernel::Interface;

M_EXPORT_API
auto execute (
    StringList* argument,
    Callback callback
) -> int
{
    try
    {
        std::setlocale(LC_ALL, "C");
        auto& script = argument->value[2];
        Shell::callback = callback;
        Executor::register_external(&script, argument);
        auto kernel = Runtime{};
        kernel.prepare();
        kernel.execute();
        Executor::unregister_external();
        Executor::clean_arguments();
    }
    catch(...)
    {
        auto parameters = construct_string_list(std::array<std::string, 4>{std::string{"display"}, fmt::format("Runtime Exception found: {}", parse_exception().what()), std::string{""}, std::string{"red"}});
        Shell::callback(parameters.get(), nullptr);
        return 1;
    }
    return 0;
}