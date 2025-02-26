#pragma once


#include "deps/tinyfiledialogs/tinyfiledialogs.h"
#include "library/string.hpp"

#if WINDOWS
#include <shobjidl_core.h>
#endif

namespace Sen::Shell {

    inline auto readline (

	) -> std::string {
		auto text = std::string{};
		#if WINDOWS
		const auto handle = GetStdHandle(STD_INPUT_HANDLE);
		auto handle_mode = DWORD{};
		if (GetConsoleMode(handle, &handle_mode)) {
			auto text_16 = std::array<char16_t, 0x1000>{};
			auto length = DWORD{};
			ReadConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
			auto text_8 = u16_to_u8(std::u16string_view{text_16.data(), length - 2});
			text = std::move(reinterpret_cast<std::string &>(text_8));
		}
		else {
			std::getline(std::cin, text);
		}
		#else
		std::getline(std::cin, text);
		#endif
		trim(text);
		return text;
	}

	inline auto print (
		const std::string& text
	) -> void {
		#if WINDOWS
    	const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    	auto handle_mode = DWORD{};
    	if (GetConsoleMode(handle, &handle_mode)) {
    		const auto text_16 = u8_to_u16(text);
    		WriteConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), nullptr, nullptr);
    	}
    	else {
    		std::cout << text << std::flush;
    	}
		#else
    	std::cout << text << std::flush;
		#endif
    }

	inline auto println (
		const std::string& text
	) -> void {
	    return print(text + '\n');
    }

	inline auto print_if_not_empty (
		const std::string& text
	) -> void {
    	if (text.empty()) {
    		return;
    	}
    	return println(text);
    }

	inline auto static constexpr color_map = std::array<std::pair<std::string_view, std::string_view>, 8>{{
		{"red", "31"},
		{"green", "32"},
		{"yellow", "33"},
		{"blue", "34"},
		{"magenta", "35"},
		{"cyan", "36"},
		{"white", "37"},
		{"default", "0"}
	}};

	inline auto constexpr get_color(
		const std::string_view& color
	) -> std::string_view {
		for (const auto& [name, code] : color_map) {
			if (name == color) {
				return code;
			}
		}
		return "0";
	}

	inline auto color_of(
		const std::string& text,
		const std::string_view& color
	) -> std::string {
		return "\033[" + std::string(get_color(color)) + "m" + text + "\033[0m";
	}

	inline auto print_arguments (
		const std::vector<std::string>& arguments
	) -> void {
	    switch (arguments.size()) {
	    	case 2:
	    		println(arguments[1]);
	    		break;
	    	case 3:
	    		println(color_of(arguments[1], arguments[2]));
	    		break;
	    	case 4:
	    		println(color_of(arguments[1], arguments[2]));
	    		print_if_not_empty(arguments[3]);
	    		break;
		    default: {
			    break;
		    }
	    }
    }

	inline auto push_notification (
		const std::string & title,
		const std::string & description
	) -> void {
		#if WINDOWS || LINUX || MACINTOSH
    	tinyfd_notifyPopup(title.data(), description.data(), "info");
		#else
    	assert_conditional(false, "NOT IMPLEMENTED");
		#endif
    }

	enum struct StorageType : u8 {
		pick_file,
		pick_directory,
		save_file
	};

	template <auto type> requires std::is_same_v<decltype(type), StorageType>
	inline auto open_pick_dialog (
	) -> std::optional<std::string> {
		auto target = std::optional<std::string>{};
		#if WINDOWS
		auto state_h = HRESULT{};
		auto _ = CoInitialize(nullptr);
		auto dialog = std::add_pointer_t<IFileDialog>{nullptr};
		if constexpr (type == StorageType::pick_file || type == StorageType::pick_directory) {
			state_h = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, __uuidof(*dialog), IID_PPV_ARGS_Helper(&dialog));
		}
		else {
			state_h = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, __uuidof(*dialog), IID_PPV_ARGS_Helper(&dialog));
		}
		auto option = FILEOPENDIALOGOPTIONS{};
		option |= FOS_NOCHANGEDIR | FOS_NOVALIDATE | FOS_NODEREFERENCELINKS | FOS_DONTADDTORECENT | FOS_FORCESHOWHIDDEN;
		if constexpr (type == StorageType::pick_file || type == StorageType::pick_directory) {
			option |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST;
		}
		if constexpr (type == StorageType::pick_directory) {
			option |= FOS_PICKFOLDERS;
		}
		if constexpr (type == StorageType::save_file) {
			option |= FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT;
		}
		state_h = dialog->SetOptions(option);
		state_h = dialog->Show(GetForegroundWindow());
		if (state_h == S_OK) {
			auto result_item = std::add_pointer_t<IShellItem>{nullptr};
			state_h = dialog->GetResult(&result_item);
			auto display_name = LPWSTR{nullptr};
			state_h = result_item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
			if (state_h == S_OK) {
				auto display_name_8 = u16_to_u8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
				std::ranges::replace(display_name_8, u8'\\', u8'/');
				target.emplace(std::move(reinterpret_cast<std::string &>(display_name_8)));
				CoTaskMemFree(display_name);
			}
			result_item->Release();
		}
		dialog->Release();
		CoUninitialize();
		#else
		auto target_data = std::add_pointer_t<char>{nullptr};
		if constexpr (type == StorageType::pick_file) {
			target_data = tinyfd_openFileDialog("", nullptr, 0, nullptr, nullptr, false);
		}
		else if constexpr (type == StorageType::pick_directory) {
			target_data = tinyfd_selectFolderDialog("", nullptr);
		}
		else {
			target_data = tinyfd_saveFileDialog("", nullptr, 0, nullptr, nullptr);
		}
		if (target_data != nullptr) {
			auto target_value = std::string{target_data};
			#if MACINTOSH
			if (target_value.size() > 1 && target_value.back() == '/') {
				target_value.pop_back();
			}
			#endif
			target.emplace(std::move(target_value));
		}
		#endif
		return target;
	}

}