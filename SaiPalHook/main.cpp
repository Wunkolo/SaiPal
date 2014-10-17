#include <windows.h>
#include <fcntl.h> //_O_TEXT
#include <io.h> //_open_osfhandle

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "SaiPal.h"

//Stb implementations
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

int Thread()
{
	Clock::time_point PrevTime, CurTime;
	HANDLE MainThread = OpenThread(THREAD_ALL_ACCESS, 0, GetMainThreadId());
	DWORD MainThreadStatus = WAIT_TIMEOUT;
	while( MainThreadStatus == WAIT_TIMEOUT )
	{
		MainThreadStatus = WaitForSingleObject(MainThread, 0);
		PrevTime = CurTime;
		CurTime = Clock::now();
		SaiPal::Instance().Tick(CurTime - PrevTime);
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD Reason, LPVOID Misc)
{
	if( Reason == DLL_PROCESS_ATTACH )
	{
		::DisableThreadLibraryCalls(hDLL);
		//Set up new console window
		//Input
		::AllocConsole();
		::SetConsoleTitleA("SaiPal");
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE),
						 SC_CLOSE | SC_MINIMIZE,
						 MF_GRAYED);
		::DrawMenuBar(::GetConsoleWindow());
		HANDLE hStd = ::GetStdHandle(STD_INPUT_HANDLE); // STD in Handle
		unsigned int hConsole = _open_osfhandle((intptr_t)hStd, _O_TEXT); // Console Handle
		FILE* fIn = ::_fdopen(hConsole, "r");
		*stdin = *fIn;
		setvbuf(stdin, NULL, _IONBF, 0);
		//Output
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
	else if( Reason == DLL_PROCESS_DETACH )
	{
		return true;
	}
	else if( Reason == DLL_THREAD_ATTACH )
	{
		return true;
	}
	else if( Reason == DLL_THREAD_DETACH )
	{
		return true;
	}
	else
	{
		return true;
	}
	return false;
}