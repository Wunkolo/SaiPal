#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <fcntl.h> //_O_TEXT
#include <io.h> //_open_osfhandle

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "SaiPal.h"

void ProcessLayerList(void* Layer, unsigned char TabLevel = 0)
{
	if (!Layer)
	{
		return;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << std::string(TabLevel, '\t') << "Layer Name: " << (unsigned char*)((unsigned char*)Layer + 0x17C) << std::endl;
	std::cout << std::string(TabLevel, '\t') << "-NextLayer: " << *(unsigned int*)((unsigned char*)Layer + 0x0) << std::endl;

	*(unsigned short*)((unsigned char*)Layer + 0x5b) = !*(unsigned short*)((unsigned char*)Layer + 0x5b);

	if (*(unsigned short*)((unsigned char*)Layer + 0x5b) == 1)
	{
		*(unsigned int*)((unsigned char*)Layer + 0x5b) = 0x90;
	}
	else
	{
		*(unsigned int*)((unsigned char*)Layer + 0x5b) = 0;
	}

	//Children Layers
	if ((void*)*(unsigned int*)((unsigned char*)Layer + 0xC) != nullptr)
	{
		ProcessLayerList((void*)*(unsigned int*)((unsigned char*)Layer + 0x0), ++TabLevel);
	}
	//Next Layer
	if ((void*)*(unsigned int*)((unsigned char*)Layer + 0x0) != nullptr)
	{
		ProcessLayerList((void*)*(unsigned int*)((unsigned char*)Layer + 0x0), TabLevel);
	}
}

int Thread()
{
	int(cdecl *SaveCanvasToFile)(void* Session, void* Canvas, const char* FileName, bool, bool PromptOption) =
		(int(*)(void*, void*, const char*, bool, bool))((unsigned int)GetBasePointer() + 0x3180);
	int(cdecl *RenderLayers)(void* Canvas, void* ActiveLayer) =
		(int(*)(void*, void*))((unsigned int)GetBasePointer() + 0x80C70);
	int(cdecl *UpdateGUILayer)(void* Layer) =
		(int(*)(void*))((unsigned int)GetBasePointer() + 0x66180);

	int(*win_redraw)(HWND hWnd, int flags) = (int(*)(HWND, int))GetProcAddress(LoadLibraryA("sfl.dll"), "win_redraw");

	Clock::time_point PrevTime, CurTime;
	while (1)
	{
		PrevTime = CurTime;
		CurTime = Clock::now();
		SaiPal::Instance().Tick(CurTime - PrevTime);
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD Reason, LPVOID Misc)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		::DisableThreadLibraryCalls(hDLL);

		//Set up new console window
		::AllocConsole();
		::SetConsoleTitleA("SaiPal");
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE), SC_CLOSE | SC_MINIMIZE, MF_GRAYED);
		::DrawMenuBar(::GetConsoleWindow());
		//setup CIN
		//::GetConsoleScreenBufferInfo(::GetStdHandle(STD_INPUT_HANDLE), &consoleInfo);
		//consoleInfo.dwSize.X = 1024;
		//SetConsoleScreenBufferSize(::GetStdHandle(STD_INPUT_HANDLE), consoleInfo.dwSize);
		HANDLE hStd = ::GetStdHandle(STD_INPUT_HANDLE); // STD in Handle
		unsigned int hConsole = _open_osfhandle((intptr_t)hStd, _O_TEXT); // Console Handle
		FILE* fIn = ::_fdopen(hConsole, "r");
		*stdin = *fIn;
		setvbuf(stdin, NULL, _IONBF, 0);
		//setup COUT
		::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
		consoleInfo.dwSize.Y = 25;
		consoleInfo.dwSize.X = 30;
		::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), consoleInfo.dwSize);
		hStd = ::GetStdHandle(STD_OUTPUT_HANDLE); // STD out Handle
		hConsole = _open_osfhandle((intptr_t)hStd, _O_TEXT); // Console Handle
		FILE* fOut = _fdopen(hConsole, "w");
		*stdout = *fOut;
		setvbuf(stdout, NULL, _IONBF, 0);
		std::ios::sync_with_stdio();

		::CreateThread(
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)&Thread,
			NULL,
			0,
			NULL);
		return true;
	}
	return true;
}