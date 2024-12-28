#pragma once

#include "library/callback.hpp"
#include "library/macro.hpp"

namespace Sen::Shell {

	class KernelLoader {
	public:
		KernelLoader(int size, 
		#if WINDOWS
			wchar_t** argc
		#else
			char** argc
		#endif
		)
			: size(size), argc(argc), result(1), hinst_lib(nullptr) {}

		inline auto run(

		) -> int
		{
			try {
				this->initialize_console();
				this->validate_arguments();
				this->set_entry();
				this->load_kernel();
				auto execute_method = this->lookup_symbols();
				this->prepare_arguments();
				result = this->execute_kernel(execute_method);
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
		int size;
	#if WINDOWS
		wchar_t** argc;
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
			if (size < 3) {
				throw std::runtime_error{"Please use launcher to launch sen"};
			}
		}

		inline auto set_entry(

		) -> void
		{
			#if WINDOWS
			kernel = utf16_to_utf8(argc[1]);
			script = utf16_to_utf8(argc[2]);
			#else
			kernel = std::string{argc[1], std::strlen(argc[1])};
			kernel = std::string{argc[2], std::strlen(argc[2])};
			#endif
		}

		inline auto load_kernel(

		) -> void
		{
			#if WINDOWS
			hinst_lib = LoadLibrary(kernel.c_str());
			#else
			hinst_lib = dlopen(kernel.c_str(), RTLD_LAZY);
			#endif
			if (hinst_lib == nullptr) {
				throw std::runtime_error{"kernel cannot be loaded"};
			}
		}

		inline auto lookup_symbols(

		) -> execute 
		{
			#if WINDOWS
			auto execute_method = reinterpret_cast<execute>(GetProcAddress(hinst_lib, "execute"));
			#else
			auto execute_method = reinterpret_cast<execute>(dlsym(hinst_lib, "execute"));
			#endif
			if (execute_method == nullptr) {
				this->cleanup_kernel();
				throw std::runtime_error{"execute method not found"};
			}
			return execute_method;
		}

		inline auto prepare_arguments(

		) -> void
		{
			this->argument_list = std::make_unique<CStringList>();
			this->argument_list->size = static_cast<size_t>(size);
			this->argument_list->value = new CStringView[argument_list->size];
			for (auto i = 0; i < size; ++i) {
				if (i == 1) {
					this->argument_list->value[i] = CStringView{kernel.size(), kernel.data()};
				} else if (i == 2) {
					this->argument_list->value[i] = CStringView{script.size(), script.data()};
				} else {
					#if WINDOWS
					auto arg_value = utf16_to_utf8(argc[i]);
					auto value_copy = new char[arg_value.size() + 1];
					std::memcpy(value_copy, arg_value.data(), arg_value.size());
					value_copy[arg_value.size()] = '\0';
					this->argument_list->value[i] = CStringView{arg_value.size(), value_copy};
					#else
					argument_list->value[i] = CStringView{std::strlen(argc[i]), argc[i]};
					#endif
				}
			}
			return;
		}

		inline auto execute_kernel(
			execute execute_method
		) -> int 
		{
			auto script_pointer = CStringView{script.size(), script.data()};
			return execute_method(&script_pointer, this->argument_list.get(), callback);
		}

		inline auto cleanup(

		) -> void
		{
			if (argument_list.get() != nullptr) {
				delete[] argument_list->value;
				for (auto i = 3; i < size; ++i) {
					delete[] argument_list->value[i].value;
				}
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

		std::unique_ptr<CStringList> argument_list;
	};

}
