#include "library/macro.hpp"
#include "library/loader.hpp"

MAIN_FUNCTION
{
    using namespace Sen::Shell;
    return KernelLoader(size, argc).run();
}