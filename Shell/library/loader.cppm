#include "library/macro.hpp"
import sen.environment.shell.library.standard;
import sen.environment.shell.library.callback;
import sen.environment.shell.library.string;
import sen.environment.shell.bridge.data;
import sen.environment.shell.bridge.executor;

#if WINDOWS
#include <windows.h>
#include <VersionHelpers.h>
#endif

export module sen.environment.shell.library.loader;

export namespace Sen::Shell {

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
					print(std::string{"Exception found: "} +  std::string{e.what()}, "", Color::RED);
				} catch (...) {
					print("Unknown exception found", "caught an unhandled exception", Color::RED);
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

		#if WINDOWS
			HMODULE hinst_lib;
		#else
			void* hinst_lib;
		#endif

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
				#else
				kernel = std::string{argv[1], std::strlen(argv[1])};
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
				this->argument_list = std::make_unique<StringList>();
				this->argument_list->size = static_cast<size_t>(argc);
				this->argument_list->value = new String[argument_list->size];
				for (auto i = std::size_t{0}; i < argc; ++i) {
					#if WINDOWS
						auto arg_value = utf16_to_utf8(argv[i]);
						auto value_copy = std::make_unique<uint8_t[]>(arg_value.size() + 1);
						std::memcpy(value_copy.get(), arg_value.data(), arg_value.size());
						value_copy[arg_value.size()] = '\0';
						this->argument_list->value[i] = String{value_copy.release(), arg_value.size()};
						#else
						argument_list->value[i] = String{argv[i], std::strlen(argv[i])};
						#endif
				}
				return;
			}

			template <typename function, typename callable> requires std::is_invocable_r<int, function, StringList*, callable>::value && std::is_invocable_r<int, callable, StringList*, String*>::value
			inline auto execute_kernel(
				function execute_method
			) -> int 
			{
				return execute_method(this->argument_list.get(), callback);
			}

			inline auto cleanup(

			) -> void
			{
				if (argument_list.get() != nullptr) {
					for (auto i = std::size_t{0}; i < argc; ++i) {
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

		std::unique_ptr<StringList> argument_list{nullptr};
	};

}
