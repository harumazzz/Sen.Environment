#include "library/loader.hpp"

using namespace Sen::Shell;

MAIN_FUNCTION
{
    return Sen::Shell::KernelLoader(size, argc).run();
}
