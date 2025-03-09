#include "explorer/command/method_list.hpp"
#include "explorer/utility/system.hpp"

using namespace std;

auto APIENTRY DllMain (
	HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
) -> BOOL {
	Sen::Environment::handle = hModule;
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH :
		case DLL_THREAD_ATTACH :
		case DLL_THREAD_DETACH :
		case DLL_PROCESS_DETACH :
			break;
		default: ;
	}
	return TRUE;
}

STDAPI DllCanUnloadNow (
) {
	return Module<InProc>::GetModule().GetObjectCount() == 0 ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject (
	_In_ REFCLSID rclsid,
	_In_ REFIID riid,
	_COM_Outptr_ void** instance
) {
	return Module<InProc>::GetModule().GetClassObject(rclsid, riid, instance);
}

STDAPI DllGetActivationFactory (
	_In_ HSTRING activatableClassId,
	_COM_Outptr_ IActivationFactory ** factory
) {
	return Module<InProc>::GetModule().GetActivationFactory(activatableClassId, factory);
}

using namespace Sen::Environment;

#define M_REGISTER_CONTEXT_MENU(_class)\
CoCreatableClass(_class)\
CoCreatableClassWrlCreatorMapInclude(_class)

M_REGISTER_CONTEXT_MENU(LaunchMethodInvokeCommand);
M_REGISTER_CONTEXT_MENU(JSEvaluateMethodInvokeCommand);