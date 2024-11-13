#include "library/download.hpp"
#include "library/string.hpp"
#include "library/zip.hpp"
#include "library/version.hpp"
#include "library/platform/windows.hpp"
#include "library/platform/unix.hpp"

using namespace Sen::Launcher;

MAIN{
    try {
        #ifdef _WIN32
            auto home = get_executable_path();
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
            auto home = get_executable_path();
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
        std::println(std::cout, "error found: {}", e.what());
    }
    std::system("pause");
    return -1;
}