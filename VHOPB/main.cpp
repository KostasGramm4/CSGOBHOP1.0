
#include "process.h"

namespace offset
{
	uintptr_t dwLocalPlayer = 0xDE7964;
	uintptr_t m_fFlags = 0x104;
	uintptr_t dwForceJump = 0x52B8BFC;
}

int main()
{
	DWORD processId = GetProcessId(L"csgo.exe");


	if (processId)
	{
		std::cout << "Process Id:" << processId << std::endl;

		uintptr_t moduleBase = GetModuleBaseAddr(processId, L"client.dll");

		std::cout << "ModuleBase:" << moduleBase << std::endl;

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

		uintptr_t localPlayer = (moduleBase + offset::dwLocalPlayer);

		std::cout << localPlayer << std::endl;

		std::vector<unsigned int>flagsOffsets{ offset::m_fFlags };

		uintptr_t flagsAddr = FindDMAAddy(hProcess, localPlayer, flagsOffsets);

		uintptr_t jumpAddr = (moduleBase + offset::dwForceJump);

		unsigned int readFlag = 0;
		unsigned int writeJump = 6;

		while (true)
		{
			ReadProcessMemory(hProcess, (BYTE*)flagsAddr, &readFlag, sizeof(readFlag), 0);

			if (GetAsyncKeyState(VK_SPACE) && readFlag == 257)
			{
				WriteProcessMemory(hProcess, (BYTE*)jumpAddr, &writeJump, sizeof(writeJump), 0);
			}

			Sleep(1);
		}
	}
}
