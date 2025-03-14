#include "virtual.h"

int main() {
    auto h_module = LoadLibraryW(L"explorer.dll");
    if (h_module == nullptr) {
        MessageBoxA(nullptr, "Failed to load the DLL!", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    using DllEntry = HRESULT(*)();
    auto entry = static_cast<DllEntry>(reinterpret_cast<void*>(GetProcAddress(h_module, "DllRegisterServer")));
    if (entry != nullptr) {
        entry();
    }
    MessageBoxA(nullptr, "DLL is hosted!", "Info", MB_OK);
    return 0;
}

