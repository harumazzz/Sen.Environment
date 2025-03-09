#pragma once

#include <string_view>
#include <string>
#include <Windows.h>
#include <stdexcept>

inline constexpr auto hash_string (
	std::string_view const & string
) -> std::uint64_t {
	constexpr auto offset = std::uint64_t{14695981039346656037ull};
	constexpr auto prime = std::uint64_t{1099511628211ull};
	auto result = offset;
	for (auto & element : string) {
		result ^= static_cast<std::uint8_t>(element);
		result *= prime;
	}
	return result;
}

inline auto utf8_to_utf16(
	const std::string& utf8
) -> std::wstring
{
	auto size_needed = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0));
	auto utf16 = std::wstring{};
	utf16.resize(size_needed);
	auto state = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), utf16.data(), static_cast<int>(size_needed));
	if (state <= 0) {
        throw std::runtime_error("Failed to convert UTF-8 to UTF-16.");
    }
	return utf16;
}

inline auto execute_windows_command (
	const std::wstring& command
) -> void {
	auto processInfo = PROCESS_INFORMATION{};
	ZeroMemory(&processInfo, sizeof(processInfo));
	auto startupInfo = STARTUPINFOW{};
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	if (!CreateProcessW(
		nullptr,                   
		const_cast<LPWSTR>(command.data()), 
		nullptr,                   
		nullptr,                  
		FALSE,                     
		CREATE_NEW_CONSOLE,  
		nullptr,                   
		nullptr,                   
		&startupInfo,              
		&processInfo               
	)) {
		throw std::runtime_error{ "Failed to execute command via CreateProcessW" };
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}
