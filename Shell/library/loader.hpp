#pragma once

#include "library/callback.hpp"
#include "library/macro.hpp"

#if WINDOWS
#include <windows.h>
#include <VersionHelpers.h>
#endif

namespace Sen::Shell {

	#if WINDOWS

	using RtlGetVersionPtr = LONG (WINAPI *)(PRTL_OSVERSIONINFOW);

	
	inline auto is_windows10(

	) -> bool 
	{
		auto rovi = RTL_OSVERSIONINFOW{0};
		rovi.dwOSVersionInfoSize = sizeof(rovi);
		auto hMod = GetModuleHandleW(L"ntdll.dll");
		if (hMod != nullptr) {
			auto pRtlGetVersion = reinterpret_cast<RtlGetVersionPtr>(GetProcAddress(hMod, "RtlGetVersion"));
			if (pRtlGetVersion != nullptr) {
				pRtlGetVersion(&rovi);
				return (rovi.dwMajorVersion > 10) || 
					(rovi.dwMajorVersion == 10 && rovi.dwMinorVersion >= 0);
			}
		}
		return false;
	}
	#endif

	using Callback = void(*)(CStringList* list, CStringView* destination);

	using Execute = int(*)(CStringView* script, CStringList* argument, Callback m_callback);

	class KernelLoader {
	public:
		KernelLoader(
			int argc, 
		#if WINDOWS
			wchar_t** argv
		#else
			char** argv
		#endif
		)
			: argc(argc), argv(argv), result(1), hinst_lib(nullptr) {}

		inline auto run(

		) -> int
		{
			try {
				this->validate_os();
				this->initialize_console();
				this->validate_arguments();
				this->set_entry();
				this->load_kernel();
				auto execute_method = this->lookup_symbols<Execute>();
				this->prepare_arguments();
				result = this->execute_kernel<Execute, Callback>(execute_method);
			} catch (const std::runtime_error& e) {
				Console::print(std::string{"Exception found: "} +  std::string{e.what()}, "", Interactive::Color::RED);
			} catch (...) {
				Console::print("Unknown exception found", "", Interactive::Color::RED);
			}
			return result;
		}
		
		~KernelLoader() {
			this->cleanup();
		}

	private:
		int argc;
	#if WINDOWS
		wchar_t** argv;
	#else
		char** argc;
	#endif
		int result;
		std::string kernel;
		std::string script;

	#if WINDOWS
		HMODULE hinst_lib;
	#else
		void* hinst_lib;
	#endif

		MemoryBuffer copy{1}; 

		inline auto initialize_console(

		) -> void const {
		#if WINDOWS
			SetConsoleCP(CP_UTF8);
			SetConsoleOutputCP(CP_UTF8);
		#endif
		}

		inline auto validate_arguments(

		) -> void const {
			if (argc < 3) {
				throw std::runtime_error{"Please use launcher to launch sen"};
			}
		}

		inline auto validate_os (

		) -> void
		{
			#if WINDOWS
				if (!is_windows10()) {
					throw std::runtime_error{"Windows 10 or later is required"};
				}
			#endif
			return;
		}

		inline auto set_entry(

		) -> void
		{
			#if WINDOWS
			kernel = utf16_to_utf8(argv[1]);
			script = utf16_to_utf8(argv[2]);
			#else
			kernel = std::string{argv[1], std::strlen(argv[1])};
			script = std::string{argv[2], std::strlen(argv[2])};
			#endif
		}

		inline auto load_kernel(

		) -> void
		{
			#if WINDOWS
			auto utf16_kernel = u8_to_u16(kernel);
			hinst_lib = LoadLibraryW(utf16_kernel.data());
			#else
			hinst_lib = dlopen(kernel.data(), RTLD_LAZY | RTLD_LOCAL);
			#endif
			if (hinst_lib == nullptr) {
				throw std::runtime_error{"kernel cannot be loaded"};
			}
		}

		template <typename function>
		inline auto lookup_symbols(

		) -> function 
		{
			#if WINDOWS
			auto execute_method = reinterpret_cast<void*>(GetProcAddress(hinst_lib, "execute"));
			#else
			auto execute_method = reinterpret_cast<void*>(dlsym(hinst_lib, "execute"));
			#endif
			if (execute_method == nullptr) {
				this->cleanup_kernel();
				throw std::runtime_error{"execute method not found"};
			}
			return static_cast<function>(execute_method);
		}

		inline auto prepare_arguments(

		) -> void
		{
			this->argument_list = std::make_unique<CStringList>();
			this->argument_list->size = static_cast<size_t>(argc);
			this->argument_list->value = new CStringView[argument_list->size];
			this->argument_list->value[0] = CStringView{kernel.size(), kernel.data()};
			this->argument_list->value[1] = CStringView{script.size(), script.data()};
			for (auto i = 0; i != 1, i != 2, i < argc; ++i) {
				#if WINDOWS
					auto arg_value = utf16_to_utf8(argv[i]);
					auto value_copy = new char[arg_value.size() + 1];
					std::memcpy(value_copy, arg_value.data(), arg_value.size());
					value_copy[arg_value.size()] = '\0';
					this->argument_list->value[i] = CStringView{arg_value.size(), value_copy};
					#else
					argument_list->value[i] = CStringView{std::strlen(argv[i]), argv[i]};
					#endif
			}
			return;
		}

		template <typename function, typename callable> requires std::is_invocable_r<int, function, CStringView*, CStringList*, callable>::value && std::is_invocable_r<void, callable, CStringList*, CStringView*>::value
		inline auto execute_kernel(
			function execute_method
		) -> int 
		{
			auto script_pointer = CStringView{script.size(), script.data()};
			return execute_method(&script_pointer, this->argument_list.get(), callback);
		}

		inline auto cleanup(

		) -> void
		{
			if (argument_list.get() != nullptr) {
				for (auto i = 0; i < argc; ++i) {
					if (argument_list->value[i].value != nullptr) {
						delete[] argument_list->value[i].value;
						argument_list->value[i].value = nullptr;
					}
				}
				delete[] argument_list->value;
			}
			this->cleanup_kernel();
		}

		inline auto cleanup_kernel(

		) -> void 
		{
			if (hinst_lib != nullptr) {
				#if WINDOWS
				FreeLibrary(hinst_lib);
				#else
				dlclose(hinst_lib);
				#endif
			}
		}

		std::unique_ptr<CStringList> argument_list{nullptr};
	};

}
