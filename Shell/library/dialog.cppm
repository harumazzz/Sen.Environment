module;

#include "library/macro.hpp"
#if _WIN32
#include "deps/tinyfiledialogs/tinyfiledialogs.h"
#include <windows.h>
#include <ShObjIdl_core.h>
import <codecvt>;
import <string_view>;
#endif
import sen.environment.shell.library.standard;	
import sen.environment.shell.library.string;	
export module sen.environment.shell.library.dialog;

export namespace Sen::Shell {

	#if WINDOWS

		inline auto pick_path (
			bool is_directory,
			bool is_multiply
		) -> std::vector<std::string> {
			auto state_h = HRESULT{};
			auto result = std::vector<std::string>{};
			CoInitialize(nullptr);
			auto dialog = std::add_pointer_t<IFileOpenDialog>{nullptr};
			state_h = CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));
			auto option = FILEOPENDIALOGOPTIONS{};
			state_h = dialog->GetOptions(&option);
			option |= FOS_FORCEFILESYSTEM | FOS_NODEREFERENCELINKS | FOS_DONTADDTORECENT | FOS_FORCESHOWHIDDEN;
			if (is_directory) {
				option |= FOS_PICKFOLDERS;
			}
			if (is_multiply) {
				option |= FOS_ALLOWMULTISELECT;
			}
			state_h = dialog->SetOptions(option);
			state_h = dialog->Show(nullptr);
			if (state_h != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
				auto selection_item_list = std::add_pointer_t<IShellItemArray>{nullptr};
				state_h = dialog->GetResults(&selection_item_list);
				auto count = DWORD{0};
				state_h = selection_item_list->GetCount(&count);
				result.reserve(count);
				for (auto index = DWORD{0}; index < count; ++index) {
					auto item = std::add_pointer_t<IShellItem>{nullptr};
					auto display_name = LPWSTR{nullptr};
					state_h = selection_item_list->GetItemAt(index, &item);
					state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &display_name);
					auto display_name_8 = utf16_to_utf8(std::u16string_view{reinterpret_cast<char16_t const *>(display_name)});
					result.emplace_back(std::move(reinterpret_cast<std::string &>(display_name_8)));
					CoTaskMemFree(display_name);
					item->Release();
				}
				selection_item_list->Release();
			}
			dialog->Release();
			for (auto & path : result) {
				std::replace(path.begin(), path.end(), '\\', '/');
			}
			return result;
		}
	#endif

	inline auto constexpr tinyfd_openFileDialog = ::tinyfd_openFileDialog;

	inline auto constexpr tinyfd_notifyPopup = ::tinyfd_notifyPopup;

	inline auto constexpr tinyfd_selectFolderDialog = ::tinyfd_selectFolderDialog;

}