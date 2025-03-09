#pragma once

#include "explorer/explorer.h"

namespace Sen::Environment {

    inline auto get_selection_path (
        IShellItemArray* & selection
    ) -> std::vector<std::wstring> {
        auto result = std::vector<std::wstring>{};
        if (selection != nullptr) {
            auto count = DWORD{};
            auto state = selection->GetCount(&count);
            assert(SUCCEEDED(state), "Failed to get selection count");
            if (count > 0) {
                result.reserve(count);
                for (auto i = DWORD{0}; i < count; ++i) {
                    auto item = std::add_pointer_t<IShellItem>{nullptr};
                    if (SUCCEEDED(selection->GetItemAt(i, &item))) {
                        auto name = LPWSTR{};
                        state = item->GetDisplayName(SIGDN_FILESYSPATH, &name);
                        assert(SUCCEEDED(state), "Failed to get display name");
                        item->Release();
                        result.emplace_back(name);
                        CoTaskMemFree(name);
                    }
                }
            }
        }
        return result;
    }

	inline auto handle = HMODULE{};

    inline auto get_dll_path (
	) -> std::wstring {
		auto buffer = std::array<WCHAR, 1024>{};
		const auto length = GetModuleFileNameW(handle, buffer.data(), 1024);
        assert(length != 0, "Failed to get module file name");
		return std::wstring{buffer.begin(), buffer.begin() + length};
	}

}