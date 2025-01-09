#include "library/macro.hpp"
#include "library/standard.hpp"

import sen.environment.launcher.library.string;
import sen.environment.launcher.library.version;
import sen.environment.launcher.library.platform.windows;
import sen.environment.launcher.library.platform.unix;

MAIN {
    using namespace Sen::Launcher;
    try {
        #ifdef _WIN32
            auto home = current_home();
            if (home.empty()) {
                throw std::runtime_error{"Failed to get the executable path."};
            }
            auto shell_path = L"\"" + home + L"\\Shell.exe" + L"\"";
            auto arguments = L"\"" + home + L"\\Kernel.dll\" \"" + home + L"\\Script\\main.js\"";
            for (auto i = 1; i < argc; ++i) {
                auto arg = std::wstring(argv[i], argv[i] + wcslen(argv[i]));
                arguments += L" \"" + arg + L"\"";
            }
            execute_windows_command(shell_path, arguments);
        #else
            auto home = current_home();
            if (home.empty()) {
                throw std::runtime_error{"Failed to get the executable path."};
                return 1;
            }

            auto command = "\"" + home + "/Shell\" \"" + home + "/Kernel\" \"" + home + "/Script/main.js\"";
            for (auto i = 1; i < argc; ++i) {
                command += " \"" + std::string(argv[i]) + "\"";
            }
            std::system(command.c_str());
        #endif
    }
    catch (const std::runtime_error& e) {
        #if _WIN32
        std::println(std::cout, "error found: {}", e.what());
        #else
        std::cout << "error found: " << e.what() << '\n' << std::flush;
        #endif
    }
    std::system("pause");
    return 0;
}