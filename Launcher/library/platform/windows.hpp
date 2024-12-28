#pragma once
#include "library/utility.hpp"

#ifdef _WIN32
namespace Sen::Launcher {
	auto current_home(
	) -> std::wstring 
	{
		auto path = std::array<wchar_t, MAX_PATH>{};
		GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
		auto execPath = std::wstring(path.data(), path.size());
		auto pos = execPath.find_last_of(L"\\/");
		if (pos != std::wstring::npos) {
			execPath = execPath.substr(0, pos);
		}
		return execPath;
	}


	auto execute_windows_command(
		const std::wstring& command,
		const std::wstring& arguments
	) -> void
	{
		auto processInfo = PROCESS_INFORMATION{};
		ZeroMemory(&processInfo, sizeof(processInfo));
		auto startupInfo = STARTUPINFOW{};
		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		startupInfo.dwFlags = STARTF_USESTDHANDLES;
		startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		auto fullCommand = command + L" " + arguments;
		if (!CreateProcessW(
			nullptr,                  
			const_cast<LPWSTR>(fullCommand.c_str()),
			nullptr,                  
			nullptr,                 
			TRUE,                     
			0,                        
			nullptr,                  
			nullptr,                  
			&startupInfo,            
			&processInfo             
		)) {
			throw std::runtime_error{ "Failed to execute command via CreateProcessW" };
		}
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}

	auto get_download_directory(
	) -> std::string
	{
		auto path = PWSTR{};
		auto hr = SHGetKnownFolderPath(FOLDERID_Downloads, 0, nullptr, &path);
		if (SUCCEEDED(hr)) {
			auto mbstr = std::array<char, MAX_PATH>{};
			auto convertedChars = size_t{ 0 };
			wcstombs_s(&convertedChars, mbstr.data(), mbstr.size(), path, mbstr.size() - 1);
			CoTaskMemFree(path); 
			return std::string{mbstr.data(), mbstr.size()};
		}
		return "";
	}
}
#endif

