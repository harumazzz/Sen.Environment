#pragma once

#include "kernel/utility/macro.hpp"
#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

	enum class Architecture : uint8_t {
		X64,
		ARM,
		INTEL,
		X86,
		UNKNOWN,
		ARM64,
	};

	struct Process {

		static auto run(
			const StringView& command
		) -> void
		{
			#if WINDOWS
			_wsystem(utf8_to_utf16(command).data());
			#else
			std::system(command.cbegin());
			#endif
		}

		static auto get_environment(
			const StringView& str
		) 
		{
			#if WINDOWS
			return _wgetenv(utf8_to_utf16(str).data());
			#else
			return std::getenv(str.cbegin());
			#endif
		}

		static auto is_exists_in_path_environment(
			const String& str
		) -> bool
		{
			#if WINDOWS
			return get_environment(str) != nullptr;
			#else
    		return get_environment(str) != nullptr;
			#endif
		}

		static auto get_path_environment(
			const StringView& str
		) -> String
		{
			auto environment = get_environment(str);
			#if WINDOWS
			return StringHelper::make_string(utf16_to_utf8({environment, wcslen(environment)}));
			#else
			return String{environment, std::strlen(environment)};
			#endif
		}

		static auto execute(
			const StringView& command
		) -> String
		{
			#if WINDOWS
				auto hStdOutRead = static_cast<HANDLE>(nullptr);
				auto hStdOutWrite = static_cast<HANDLE>(nullptr);
			    auto sa = SECURITY_ATTRIBUTES{sizeof(SECURITY_ATTRIBUTES),nullptr, TRUE};
			    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
			        throw RuntimeException{"Failed to create pipe for stdout.", std::source_location::current(), "execute"};
			    }
			    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
			        CloseHandle(hStdOutRead);
			        CloseHandle(hStdOutWrite);
			        throw RuntimeException{"Failed to set handle information", std::source_location::current(), "execute"};
			    }
			    auto pi = PROCESS_INFORMATION{};
			    auto si = STARTUPINFOW{
					.cb = sizeof(STARTUPINFOW),
			    	.hStdOutput = hStdOutWrite,
			    	.hStdError = hStdOutWrite,
			    };
			    si.dwFlags |= STARTF_USESTDHANDLES;
			    if (!CreateProcessW(
			            nullptr,
			            const_cast<wchar_t*>(utf8_to_utf16(command).data()),
			            nullptr,
			            nullptr,
			            TRUE,
			            0,
			            nullptr,
			            nullptr,
			            &si,
			            &pi
			        )) {
			        CloseHandle(hStdOutRead);
			        CloseHandle(hStdOutWrite);
			    	throw RuntimeException{"Failed to create a single process", std::source_location::current(), "execute"};
			    }
			    CloseHandle(hStdOutWrite);
			    auto result = String{};
			    auto buffer = std::array<char, 128>{};
			    auto bytesRead = DWORD{};
			    while (ReadFile(hStdOutRead, buffer.data(), sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0) {
			        result.append(buffer.data(), bytesRead);
			    }
			    WaitForSingleObject(pi.hProcess, INFINITE);
			    CloseHandle(pi.hProcess);
			    CloseHandle(pi.hThread);
			    CloseHandle(hStdOutRead);
			    return result;
			#else
				auto buffer = std::array<char, 128>{};
				auto result = String{};
				auto pipe = std::unique_ptr<FILE, decltype(&pclose)>(popen(command.cbegin(), "r"), pclose);
				if (pipe == nullptr) {
					throw RuntimeException("open process failed", std::source_location::current(), "execute");
				}
				while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
					result += buffer.data();
				}
				return result;
			#endif
		}
	};
}
