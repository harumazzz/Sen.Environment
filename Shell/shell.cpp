#include "library/macro.hpp"
#include "library/library.hpp"
#include "library/miscellaneous.hpp"

using namespace Sen::Shell;

MAIN_FUNCTION {
    try {
        const auto arguments = parse_arguments(argc, argv);
        assert_conditional(arguments.size() >= 2, "Please use Launcher to launch Sen");
        auto library = Library{arguments[1]};
        library.execute(arguments);
    } catch (const std::runtime_error& e) {
        println(color_of(std::string{"Exception found: "} +  std::string{e.what()}, {"red"}));
    } catch (...) {
        println(color_of("Unknown exception found:\n\tCaught an unhandled exception", {"red"}));
    }
    return 0;
}