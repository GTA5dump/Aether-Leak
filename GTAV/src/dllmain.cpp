#include "pch.h"
#include "core/core.h"

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserve) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(module);
        if (HANDLE hThread = CreateThread(nullptr, NULL, base::core::load, module, NULL, nullptr)) {
            CloseHandle(hThread);
        }
        break;
    
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}

