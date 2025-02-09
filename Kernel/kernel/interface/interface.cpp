#include "runtime.hpp"
#include "kernel/interface/context.hpp"
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
        auto context = Context{};
        Runtime::make_environment(context);
    }
    catch(...)
    {
        auto parameters = std::unique_ptr<CStringList, StringListFinalizer>(new CStringList(nullptr, 0), finalizer<CStringList>);
        construct_string_list(std::array<std::string, 4>{std::string{"display"}, std::string{"Unknown Exception"}, fmt::format("Caught unknown exception: {}", parse_exception().what()), std::string{"red"}}, parameters.operator*());
        Shell::callback(parameters.get(), nullptr);
        return 1;
    }
    return 0;
}