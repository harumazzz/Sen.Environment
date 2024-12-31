#include "library/loader.hpp"
#include <iostream>

using namespace Sen::Shell;

MAIN_FUNCTION
{
    return Sen::Shell::KernelLoader(size, argc).run();
}