#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        MessageBoxA(NULL, "Hello from DLL!", "Injected", MB_OK | MB_ICONINFORMATION);
    }
    return TRUE;
}
