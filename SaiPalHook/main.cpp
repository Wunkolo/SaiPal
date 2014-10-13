#include <windows.h>
#include <fcntl.h> //_O_TEXT
#include <io.h> //_open_osfhandle

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "SaiPal.h"

int Thread()
{
	Clock::time_point PrevTime, CurTime;
	while( 1 )
	{
		PrevTime = CurTime;
		CurTime = Clock::now();
		SaiPal::Instance().Tick(CurTime-PrevTime);
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD Reason, LPVOID Misc)
{
	if( Reason==DLL_PROCESS_ATTACH )
	{
		::DisableThreadLibraryCalls(hDLL);
		//Set up new console window
		//Input
		::AllocConsole();
		::SetConsoleTitleA("SaiPal");
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		::EnableMenuItem(::GetSystemMenu(::GetConsoleWindow(), FALSE),
						 SC_CLOSE|SC_MINIMIZE,
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
	else if( Reason==DLL_PROCESS_DETACH )
	{
		return true;
	}
	else if( Reason==DLL_THREAD_ATTACH )
	{
		return true;
	}
	else if( Reason==DLL_THREAD_DETACH )
	{
		return true;
	}
	else
	{
		return true;
	}
	return false;
}