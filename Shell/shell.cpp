#include "library/macro.hpp"

import sen.environment.shell.library.loader;

MAIN_FUNCTION
{
    using namespace Sen::Shell;
    return KernelLoader(size, argc).run();
}