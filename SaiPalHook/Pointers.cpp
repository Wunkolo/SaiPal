#include "Pointers.h"
#include <Windows.h>
#include <TlHelp32.h> //GetModuleBase

void* GetModuleBase()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,
		GetCurrentProcessId());
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		return nullptr;
	}
	MODULEENTRY32 lpModuleEntry;
	lpModuleEntry.dwSize = sizeof(MODULEENTRY32);
	int bRet = Module32First(hSnapShot, &lpModuleEntry);
	CloseHandle(hSnapShot);
	return (bRet != 0) ? (void*)lpModuleEntry.modBaseAddr : nullptr;
}

void* GetModuleBase(const std::string& ModuleName)
{
	MODULEENTRY32 lpModuleEntry;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,
		GetCurrentProcessId());
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	if (!hSnapShot)
		return 0;
	lpModuleEntry.dwSize = sizeof(MODULEENTRY32);
	BOOL bModule = Module32First(hSnapShot, &lpModuleEntry);
	while (bModule)
	{
		//If module name matches: return it
		if (!ModuleName.compare(lpModuleEntry.szModule))
		{
			CloseHandle(hSnapShot);
			return (void*)lpModuleEntry.modBaseAddr;
		}
		bModule = Module32Next(hSnapShot, &lpModuleEntry);
	}
	CloseHandle(hSnapShot);
	return nullptr;
}