#pragma once

#include "explorer/explorer.h"

namespace Sen::Environment {

    class BaseCommand : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IExplorerCommand, IObjectWithSite> {

	protected:

		ComPtr<IUnknown> m_site;

	public:

		IFACEMETHODIMP GetTitle (
			_In_opt_ IShellItemArray* psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR* ppszName
		) override {
			*ppszName = nullptr;
			const auto title_data = this->title();
			if (title_data == nullptr) {
				return S_FALSE;
			}
			auto title_s = wil::make_cotaskmem_string_nothrow(title_data);
			RETURN_IF_NULL_ALLOC(title_s);
			*ppszName = title_s.release();
			return S_OK;
		}

		IFACEMETHODIMP GetIcon (
			_In_opt_ IShellItemArray* psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR * ppszIcon
		) override {
			*ppszIcon = nullptr;
			const auto icon_data = this->icon();
			if (icon_data == nullptr) {
				return E_NOTIMPL;
			}
			auto icon_s = wil::make_cotaskmem_string_nothrow(icon_data);
			RETURN_IF_NULL_ALLOC(icon_s);
			*ppszIcon = icon_s.release();
			return S_OK;
		}

		IFACEMETHODIMP GetToolTip (
			_In_opt_ IShellItemArray* psiItemArray,
			_Outptr_result_nullonfailure_ PWSTR* ppszInfotip
		) override {
			*ppszInfotip = nullptr;
			return E_NOTIMPL;
		}

		IFACEMETHODIMP GetCanonicalName (
			_Out_ GUID* pguidCommandName
		) override {
			*pguidCommandName = GUID_NULL;
			return S_OK;
		}

		IFACEMETHODIMP GetState (
			_In_opt_ IShellItemArray* psiItemArray,
			_In_ BOOL fOkToBeSlow,
			_Out_ EXPCMDSTATE* pCmdState
		) override {
			*pCmdState = this->state(psiItemArray);
			return S_OK;
		}

		IFACEMETHODIMP Invoke (
			_In_opt_ IShellItemArray* psiItemArray,
			_In_opt_ IBindCtx* pbc
		) override {
			return S_OK;
		}

		IFACEMETHODIMP GetFlags (
			_Out_ EXPCMDFLAGS* pFlags
		) override {
			*pFlags = this->flags();
			return S_OK;
		}

		IFACEMETHODIMP EnumSubCommands (
			_COM_Outptr_ IEnumExplorerCommand** ppEnum
		) override {
			*ppEnum = nullptr;
			return E_NOTIMPL;
		}

		IFACEMETHODIMP SetSite (
			_In_ IUnknown* pUnkSite
		) override {
			this->m_site = pUnkSite;
			return S_OK;
		}

		IFACEMETHODIMP GetSite (
			_In_ REFIID riid,
			_COM_Outptr_ void** ppvSite
		) override {
			return this->m_site.CopyTo(riid, ppvSite);
		}

		virtual auto title (
		) -> LPCWSTR {
			return nullptr;
		}

		virtual auto icon (
		) -> LPCWSTR {
			return nullptr;
		}

		virtual auto state (
			_In_opt_ IShellItemArray* selection
		) -> EXPCMDSTATE {
			return ECS_ENABLED;
		}

		virtual auto flags (
		) -> EXPCMDFLAGS {
			return ECF_DEFAULT;
		}

	};

}