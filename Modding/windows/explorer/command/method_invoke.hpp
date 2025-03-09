#pragma once

#include "explorer/command/seperator_command.hpp"
#include "explorer/utility/system.hpp"

namespace Sen::Environment {

    struct MethodInvoke {
        std::wstring name;
        std::optional<bool> type;
        std::optional<std::wregex> rule;
        std::optional<std::wstring> method;
        std::wstring argument;
    };

    struct MethodInvokeGroup {
        std::wstring name;
        std::vector<MethodInvoke> child;
        std::vector<std::size_t> separator;
    };

    inline auto test_single_path (
        const MethodInvoke& config,
        const std::wstring& path
    ) -> bool {
        auto result = bool{true};
        if (config.type) {
            if (config.type.value()) {
                result &= std::filesystem::is_directory(std::filesystem::path{path});
            } else {
                result &= std::filesystem::is_regular_file(std::filesystem::path{path});
            }
        }
        if (result && config.rule) {
            result &= std::regex_search(path, config.rule.value());
        }
        return result;
    }

    inline auto make_single_code (
        const MethodInvoke& config,
        const std::wstring& path
    ) -> std::wstring {
        if (!config.method) {
            return std::format(LR"("{}" "-argument" "{}")", path, config.argument);
        } else {
            return std::format(LR"("{}" "-method" "{}" "-argument" "{}")", path, config.method.value(), config.argument);
        }
    }

	inline constexpr auto k_launch_file = std::wstring_view{L"C:\\Users\\Admin\\Documents\\Sen.Environment\\Launcher.exe"};

    class MethodInvokeCommand : public BaseCommand {

	protected:

		MethodInvoke const & m_config;
		bool m_has_icon;

	public:

		explicit MethodInvokeCommand (
			MethodInvoke const & config,
			bool const & has_icon = true
		):
			m_config{config},
			m_has_icon{has_icon} {
		}

		virtual IFACEMETHODIMP Invoke (
			_In_opt_ IShellItemArray* selection,
			_In_opt_ IBindCtx* _
		) override {
			try {
				auto parameter = std::wstring{};
				if (selection != nullptr) {
					const auto path_list = get_selection_path(selection);
					parameter.reserve(2 + 1 + 2 + k_launch_file.size() + 2 + path_list.size() * 256);
					parameter.append(L"/C ");
					parameter.append(1, L'"');
					parameter.append(1, L'"');
					parameter.append(k_launch_file);
					parameter.append(1, L'"');
					for (auto & path : path_list) {
						parameter.append(1, L' ');
						parameter.append(make_single_code(this->m_config, path));
					}
					parameter.append(1, L'"');
				}
				parameter.append(1, L'\0');
				auto startup_info = STARTUPINFOW{};
				auto process_information = PROCESS_INFORMATION{};
				ZeroMemory(&startup_info, sizeof(startup_info));
				ZeroMemory(&process_information, sizeof(process_information));
				CreateProcessW(L"C:\\Windows\\System32\\cmd.exe", parameter.data(), nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &startup_info, &process_information);
				return S_OK;
			}
			CATCH_RETURN()
		}

		virtual auto title (
		) -> LPCWSTR override {
			return this->m_config.name.c_str();
		}

		virtual auto icon (
		) -> LPCWSTR override {
			static auto dll_path = std::wstring{};
			if (this->m_has_icon) {
				dll_path = get_dll_path();
				return dll_path.data();
			} else {
				return nullptr;
			}
		}

		virtual auto state (
			_In_opt_ IShellItemArray* selection
		) -> EXPCMDSTATE override {
			for (const auto path_list = get_selection_path(selection); auto & path : path_list) {
				if (!test_single_path(this->m_config, path)) {
					return ECS_DISABLED;
				}
			}
			return ECS_ENABLED;
		}

	};

	class MethodInvokeCommandEnum : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IEnumExplorerCommand> {

	protected:

		std::vector<ComPtr<IExplorerCommand>>                 m_commands;
		std::vector<ComPtr<IExplorerCommand>>::const_iterator m_current;

	public:

		explicit MethodInvokeCommandEnum (
			const MethodInvokeGroup& config
		) {
			auto separator_index = std::size_t{0};
			auto current_separator_section_count = std::size_t{0};
			this->m_commands.reserve(config.child.size() + config.separator.size());
			for (auto & element : config.child) {
				if (separator_index < config.separator.size() && current_separator_section_count == config.separator[separator_index]) {
					current_separator_section_count = 0;
					this->m_commands.emplace_back(Make<SeparatorCommand>());
					++separator_index;
				}
				this->m_commands.emplace_back(Make<MethodInvokeCommand>(element, false));
				++current_separator_section_count;
			}
			this->m_current = this->m_commands.cbegin();
		}

		virtual IFACEMETHODIMP Next (
			ULONG celt,
			__out_ecount_part(celt, *pceltFetched) IExplorerCommand** pUICommand,
			__out_opt ULONG* pceltFetched
		) override {
			auto fetched = ULONG{0};
			wil::assign_to_opt_param(pceltFetched, 0ul);
			for (auto i = ULONG{0}; i < celt && this->m_current != this->m_commands.cend(); i++) {
				this->m_current->CopyTo(&pUICommand[0]);
				++this->m_current;
				++fetched;
			}
			wil::assign_to_opt_param(pceltFetched, fetched);
			return fetched == celt ? S_OK : S_FALSE;
		}

		virtual IFACEMETHODIMP Skip (
			ULONG celt
		) override {
			return E_NOTIMPL;
		}

		virtual IFACEMETHODIMP Reset (
		) override {
			this->m_current = this->m_commands.cbegin();
			return S_OK;
		}

		virtual IFACEMETHODIMP Clone (
			__deref_out IEnumExplorerCommand** ppenum
		) override {
			*ppenum = nullptr;
			return E_NOTIMPL;
		}

	};

	class MethodInvokeGroupCommand : public BaseCommand {

	protected:

		MethodInvokeGroup const & m_config;

	public:

		explicit MethodInvokeGroupCommand (
			MethodInvokeGroup const & config
		):
			m_config{config} {
		}

		virtual IFACEMETHODIMP EnumSubCommands (
			_COM_Outptr_ IEnumExplorerCommand ** ppEnum
		) override {
			*ppEnum = nullptr;
			auto e = Make<MethodInvokeCommandEnum>(this->m_config);
			return e->QueryInterface(IID_PPV_ARGS(ppEnum));
		}

		virtual auto title (
		) -> LPCWSTR override {
			return this->m_config.name.c_str();
		}

		virtual auto icon (
		) -> LPCWSTR override {
			static auto dll_path = std::wstring{};
			dll_path = get_dll_path();
			return dll_path.data();
		}

		virtual auto state (
			_In_opt_ IShellItemArray * selection
		) -> EXPCMDSTATE override {
			const auto path_list = get_selection_path(selection);
			for (auto & config : this->m_config.child) {
				auto state = bool{true};
				for (auto & path : path_list) {
					if (!test_single_path(config, path)) {
						state = false;
						break;
					}
				}
				if (state) {
					return ECS_ENABLED;
				}
			}
			return ECS_DISABLED;
		}

		virtual auto flags (
		) -> EXPCMDFLAGS override {
			return ECF_HASSUBCOMMANDS;
		}

	};

}
