#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <cassert>
#include <filesystem>
#include <format>
#include <shellapi.h>
#include <Shlwapi.h>
#include <ShlObj_core.h>
#include <wrl/implements.h>
#include <array>
using namespace Microsoft::WRL;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

// Define the CLSID for the command
const CLSID CLSID_SenExplorerCommand = { 0x7ce53b67, 0xac53, 0x4eb1, { 0xa9, 0x29, 0x7f, 0x5f, 0x79, 0x3a, 0xd8, 0x31 } };

// Ensure that the UUID is specified correctly
class __declspec(uuid("7ce53b67-ac53-4eb1-a929-7f5f793ad831")) SenExplorerCommand : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IExplorerCommand, IObjectWithSite>
{
protected:
    Microsoft::WRL::ComPtr<IUnknown> m_site;  // For IObjectWithSite
    std::wstring m_application_name;  // For example purpose
    std::wstring m_application_logo;  // Placeholder for application logo
    std::wstring m_state_file;  // Placeholder for state file

public:
    explicit SenExplorerCommand() :
        m_site{},
        m_application_name{ L"Open With Sen" },  // Set application name
        m_application_logo{ L"sen_logo.ico" },  // Set application logo
        m_state_file{} // Set state file (this is an example)
    {
    }

    // IExplorerCommand Methods

    virtual IFACEMETHODIMP GetTitle(IShellItemArray* /*psiItemArray*/, LPWSTR* ppszName) override {
        if (!ppszName) return E_POINTER;
        *ppszName = nullptr;  // Initialize to null
        size_t titleLength = wcslen(L"Open With Sen") + 1;
        *ppszName = (LPWSTR)::CoTaskMemAlloc(titleLength * sizeof(wchar_t));
        if (*ppszName) {
            wcscpy_s(*ppszName, titleLength, L"Open With Sen");
            return S_OK;
        }
        return E_OUTOFMEMORY;
    }

    virtual IFACEMETHODIMP GetIcon(IShellItemArray* /*psiItemArray*/, LPWSTR* ppszIcon) override {
        if (!ppszIcon) return E_POINTER;
        size_t iconLength = m_application_logo.length() + 1;
        *ppszIcon = (LPWSTR)::CoTaskMemAlloc(iconLength * sizeof(wchar_t));
        if (*ppszIcon) {
            wcscpy_s(*ppszIcon, iconLength, m_application_logo.c_str());
            return S_OK;
        }
        return E_OUTOFMEMORY;
    }

    virtual IFACEMETHODIMP GetToolTip(IShellItemArray* /*psiItemArray*/, LPWSTR* /*ppszInfotip*/) override {
        // Not implementing tooltip for simplicity
        return S_OK;
    }

    virtual IFACEMETHODIMP GetCanonicalName(GUID* pguidCommandName) override {
        *pguidCommandName = GUID_NULL;  // Use NULL GUID for simplicity
        return S_OK;
    }

    virtual IFACEMETHODIMP GetState(IShellItemArray* /*psiItemArray*/, BOOL /*fOkToBeSlow*/, EXPCMDSTATE* pCmdState) override {
        *pCmdState = ECS_ENABLED;  // Command is enabled
        return S_OK;
    }

    virtual IFACEMETHODIMP Invoke(IShellItemArray* psiItemArray, IBindCtx* /*pbc*/) override {
        DWORD itemCount;
        psiItemArray->GetCount(&itemCount);
        std::vector<std::wstring> paths;

        for (DWORD i = 0; i < itemCount; ++i) {
            IShellItem* item = nullptr;
            psiItemArray->GetItemAt(i, &item);
            if (item) {
                LPWSTR displayName = nullptr;
                item->GetDisplayName(SIGDN_FILESYSPATH, &displayName);
                if (displayName) {
                    paths.push_back(displayName);
                    CoTaskMemFree(displayName);
                }
                item->Release();
            }
        }

        // Check if we have at least one path selected
        if (!paths.empty()) {
            // Retrieve the launcher path from the registry
            std::wstring launcherPath = GetLauncherPath();
            if (launcherPath.empty()) {
                // Handle the error: launcher path not found
                MessageBoxW(nullptr, L"Launcher path not found in the registry.", L"Error", MB_OK | MB_ICONERROR);
                return E_FAIL;
            }

            // Construct the command using the retrieved launcher path
            std::wstring command = L"\"" + launcherPath + L"\"";
            for (const auto& path : paths) {
                command += L" \"" + path + L"\"";  // Add each path as an argument
            }

            // Call the command (this launches Launcher.exe with the file paths)
            STARTUPINFOW si = { sizeof(si) };
            PROCESS_INFORMATION pi = {};
            BOOL success = CreateProcessW(
                nullptr,
                &command[0],  // Command line
                nullptr,
                nullptr,
                FALSE,
                0,
                nullptr,
                nullptr,
                &si,
                &pi
            );

            if (success) {
                // Optionally wait for the process to finish
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }

        return S_OK;
    }



    virtual IFACEMETHODIMP GetFlags(EXPCMDFLAGS* pFlags) override {
        *pFlags = ECF_DEFAULT;  // Default flags for the command
        return S_OK;
    }

    virtual IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand** ppEnum) override {
        *ppEnum = nullptr;  // No subcommands for this example
        return S_OK;
    }

    // IObjectWithSite Methods

    virtual IFACEMETHODIMP SetSite(IUnknown* pUnkSite) override {
        m_site = pUnkSite;
        return S_OK;
    }

    virtual IFACEMETHODIMP GetSite(REFIID riid, void** ppvSite) override {
        return m_site.CopyTo(riid, ppvSite);
    }

    std::wstring GetLauncherPath() {
        HKEY hKey;
        const std::wstring registryPath = L"Software\\Haruma\\Sen"; // Adjust this path
        const std::wstring valueName = L"LauncherPath"; // The value where the path is stored

        if (RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            WCHAR buffer[MAX_PATH];
            DWORD bufferSize = sizeof(buffer);
            if (RegQueryValueEx(hKey, valueName.c_str(), nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return std::wstring(buffer); // Return the launcher path from the registry
            }
            RegCloseKey(hKey);
        }

        return L""; // Return an empty string if not found
    }

    // Utility methods

    auto get_library_file_path() -> std::wstring {
        auto state_d = DWORD{};
        auto handle = reinterpret_cast<HMODULE>(&__ImageBase);
        auto result = std::wstring{};
        auto result_data = std::array<wchar_t, 1024>{};
        state_d = GetModuleFileNameW(handle, result_data.data(), static_cast<DWORD>(result_data.size()));
        assert(state_d != 0 && state_d != result_data.size());
        result = std::wstring{ result_data.data(), state_d };
        return result;
    }

    auto get_roaming_directory_path() -> std::wstring {
        auto state_h = DWORD{};
        auto result = std::wstring{};
        auto result_data = LPWSTR{};
        state_h = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &result_data);
        assert(state_h == S_OK);
        result = std::wstring{ result_data };
        CoTaskMemFree(result_data);
        return result;
    }

    auto get_file_long_path(std::wstring const& source) -> std::wstring {
        auto state_d = DWORD{};
        auto destination = std::wstring(1024, L'\0'); // Create a mutable string with enough space
        if (!source.empty()) {
            state_d = GetLongPathNameW(source.data(), &destination[0], static_cast<DWORD>(destination.size()));
            if (state_d == 0) {
                return {};  // Return empty string on failure
            }
            destination.resize(state_d);  // Resize the string to the actual length of the long path
        }
        return destination;
    }

};
