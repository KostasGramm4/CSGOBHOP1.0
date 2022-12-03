#include "process.h"

DWORD GetProcessId(const wchar_t* processName)
{
	int processId = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(processEntry);

		if (Process32First(hSnap, &processEntry))
		{
			do
			{
				if (!_wcsicmp(processName, processEntry.szExeFile))
				{
					processId = processEntry.th32ProcessID;
					break;
				}
			

			} while (Process32Next(hSnap, &processEntry));
		}
	}

	CloseHandle(hSnap);

	return processId;
}

uintptr_t GetModuleBaseAddr(DWORD processId, const wchar_t* moduleName)
{
	uintptr_t moduleBase = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processId);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32First(hSnap, &moduleEntry))
		{
			do
			{
				if (!_wcsicmp(moduleName, moduleEntry.szModule))
				{
					moduleBase = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}

			} while (Module32Next(hSnap, &moduleEntry));
		}
	}
	CloseHandle(hSnap);

	return moduleBase;
}

uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int>offsets)
{
	uintptr_t addr = ptr;

	for (int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProcess, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}

	return addr;
}