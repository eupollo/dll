#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetProcessIdByName(const wchar_t* processName) {
    PROCESSENTRY32W entry = { 0 };
    entry.dwSize = sizeof(entry);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

int main() {
    const wchar_t* processName = L"TestApp.exe";
    const char* dllPath = "C:\\Path\\To\\MessageDLL.dll"; // update path

    DWORD pid = GetProcessIdByName(processName);
    if (!pid) {
        std::cout << "Could not find process." << std::endl;
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cout << "Failed to open target process." << std::endl;
        return 1;
    }

    void* alloc = VirtualAllocEx(hProcess, nullptr, strlen(dllPath) + 1,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProcess, alloc, dllPath, strlen(dllPath) + 1, nullptr);

    HANDLE hThread = CreateRemoteThread(
        hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"),
        alloc, 0, nullptr);

    if (hThread) {
        std::cout << "DLL injected successfully!" << std::endl;
        CloseHandle(hThread);
    }
    else {
        std::cout << "Injection failed." << std::endl;
    }

    CloseHandle(hProcess);
    return 0;
}
