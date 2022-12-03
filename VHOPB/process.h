#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

DWORD GetProcessId(const wchar_t* processName);

uintptr_t GetModuleBaseAddr(DWORD processId, const wchar_t* moduleName);

uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int>offsets);

