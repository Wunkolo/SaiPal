#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <conio.h>
#include <map>
#include <psapi.h> //GetModuleFileNameEx

#include "crc.h"

bool DLLInjectRemote(DWORD ProcessID, std::string DLLpath)
{
	if( !ProcessID )
	{
		return false;
	}

	if( GetFileAttributes(DLLpath.c_str()) == INVALID_FILE_ATTRIBUTES )
	{
		std::cout << "DLL file: " << DLLpath << " does not exists" << std::endl;
		return false;
	}

	HANDLE Process = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE |
		PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		false,
		ProcessID);
	if( !Process )
	{
		return false;
	}
	void* ProcLoadLibrary = (void*)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if( !ProcLoadLibrary )
	{
		return false;
	}
	void* Alloc = (void*)VirtualAllocEx(
		Process,
		NULL,
		DLLpath.length() + 1,
		MEM_RESERVE | MEM_COMMIT,
		PAGE_READWRITE);
	if( !Alloc )
	{
		return false;
	}
	WriteProcessMemory(Process, Alloc, DLLpath.c_str(), DLLpath.length() + 1, NULL);
	CreateRemoteThread(Process,
					   NULL,
					   NULL,
					   (LPTHREAD_START_ROUTINE)ProcLoadLibrary,
					   Alloc,
					   NULL,
					   NULL);
	CloseHandle(Process);
	return true;
}

int main()
{
	std::map<std::string, std::string> DLLFiles;
	DLLFiles["1.1.0"] = "SaiHook110.dll";
	DLLFiles["1.2.0"] = "SaiHook120.dll";

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleOutputCP(437);

	unsigned int ConsoleWidth = 80;
	CONSOLE_SCREEN_BUFFER_INFO ConsoleBuf;
	if( GetConsoleScreenBufferInfo(hStdout, &ConsoleBuf) )
	{
		ConsoleWidth = ConsoleBuf.dwSize.X;
	}

	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_GREEN |
							FOREGROUND_INTENSITY);
	std::cout << "SaiPalÄÄÂBuild date (" << __DATE__ << " : " << __TIME__ << ")" << std::endl;
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_BLUE |
							FOREGROUND_GREEN |
							FOREGROUND_INTENSITY);
	std::cout << "\tÀDEElekgolo (DEElekgolo@gmail.com)\n";
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_BLUE);
	std::cout << std::string(ConsoleWidth - 1, 'Ä') << std::endl;
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_GREEN |
							FOREGROUND_INTENSITY);
	std::cout << "Detecting sai..." << std::endl;
	if( GetFileAttributes("sai.exe") == -1 )
	{
		std::cout << "Local Sai.exe file not found" << std::endl;
	}
	else
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << "Local sai.exe found, running" << std::endl;
		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// start the program up
		CreateProcess("sai.exe",   // the path
					  NULL,        // Command line
					  NULL,           // Process handle not inheritable
					  NULL,           // Thread handle not inheritable
					  FALSE,          // Set handle inheritance to FALSE
					  0,              // No creation flags
					  NULL,           // Use parent's environment block
					  NULL,           // Use parent's starting directory
					  &si,            // Pointer to STARTUPINFO structure
					  &pi           // Pointer to PROCESS_INFORMATION structure
					  );
		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		Sleep(500);
	}

	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "Looking for open Sai process..." << std::endl;
	std::string CurrentDirectory(MAX_PATH, 0);
	CurrentDirectory.resize(GetCurrentDirectoryA(MAX_PATH, &CurrentDirectory[0]));
	DWORD Address = 0;
	DWORD ProcessID;
	HWND SaiProcess;
	SaiProcess = FindWindowA("sfl_window_class", nullptr);
	if( SaiProcess == NULL )
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "Sai process not found (Error code: " << std::dec << GetLastError() << ")" << std::endl;
		system("pause");
		return 1;
	}
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "Sai.exe process found" << std::endl;
	GetWindowThreadProcessId(SaiProcess, &ProcessID);

	unsigned int SaiCheckSum = 0;

	//Hash the .exe file to identify what version/build it is
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "Identifying Sai version...";
	HANDLE SaiHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessID);
	if( SaiHandle )
	{
		std::cout << "Process opened" << std::dec << std::endl;
		char filePath[MAX_PATH];
		if( GetModuleFileNameEx(SaiHandle, NULL, filePath, MAX_PATH) )
		{
			std::cout << "Getting process file location: " << __LINE__ << std::endl;
			std::cout << filePath << std::endl;
			CloseHandle(SaiHandle);
			HANDLE SaiExe = CreateFile(filePath,
									   GENERIC_READ,
									   FILE_SHARE_READ,
									   NULL,
									   OPEN_EXISTING,
									   FILE_ATTRIBUTE_NORMAL,
									   NULL);
			std::cout << "File Opened: " << __LINE__ << std::endl;
			if( SaiExe != INVALID_HANDLE_VALUE )
			{
				unsigned int Size = GetFileSize(SaiExe, nullptr);
				if( Size == 0xFFFFFFFF )
				{
					std::cout << "Error calulating file size: " << GetLastError() << std::endl;
				}
				char* Buffer = new char[Size];
				unsigned int BytesRead;
				if( ReadFile(SaiExe, Buffer, Size, (DWORD*)&BytesRead, nullptr) )
				{
					CloseHandle(SaiExe);
					std::cout << "Calculating checksum..." << std::endl;
					SaiCheckSum = crc32(0, Buffer, Size);
					delete[] Buffer;
				}
				else
				{
					CloseHandle(SaiExe);
					delete[] Buffer;
					SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
					std::cout << "Unable to read sai executable." << std::endl;
					system("pause");
					return 1;
				}
			}
			else
			{
				SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
				CloseHandle(SaiExe);
				std::cout << "Unable to open sai file \"" << filePath <<
					"\" for reading." << std::endl;
				system("pause");
				return 1;
			}
		}
		else
		{
			CloseHandle(SaiHandle);
			SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "Unable to resolve filename for process." << std::endl;
			system("pause");
			return 1;
		}
	}
	else
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "Failed to open process for reading. (Error code: " << std::dec << GetLastError() << ")" << std::endl;
		system("pause");
		return 1;
	}

	std::string DLLFile = "";

	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	switch( SaiCheckSum )
	{
	case 0xD398D354:
		std::cout << "1.2.0" << std::endl;
		DLLFile = DLLFiles["1.2.0"];
		break;
	case 0x622B4A8A:
	case 0xE1B6027D:
		std::cout << "1.1.0" << std::endl;
		DLLFile = DLLFiles["1.1.0"];
		break;
	default:
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "Unknown sai version: 0x" << std::hex << SaiCheckSum << std::endl;
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "Input the version of your Sai.exe" << std::endl;
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED);
		std::cout << "(or '";
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "quit";
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED);
		std::cout << "'to exit)" << std::endl;
		std::string InputVer;
		do
		{
			SetConsoleTextAttribute(hStdout,
									FOREGROUND_RED |
									FOREGROUND_GREEN |
									FOREGROUND_INTENSITY);
			std::cout << "Available versions:" << std::endl;
			for( std::pair<std::string, std::string> Version : DLLFiles )
			{
				SetConsoleTextAttribute(hStdout,
										FOREGROUND_RED |
										FOREGROUND_GREEN |
										FOREGROUND_INTENSITY);
				std::cout << "\tÄ\"";
				SetConsoleTextAttribute(hStdout,
										FOREGROUND_BLUE |
										FOREGROUND_GREEN |
										FOREGROUND_INTENSITY);
				std::cout << Version.first;
				SetConsoleTextAttribute(hStdout,
										FOREGROUND_RED |
										FOREGROUND_GREEN |
										FOREGROUND_INTENSITY);
				std::cout << "\"" << std::endl;
			}
			SetConsoleTextAttribute(hStdout,
									FOREGROUND_RED |
									FOREGROUND_GREEN |
									FOREGROUND_INTENSITY);
			std::cout << ">";
			SetConsoleTextAttribute(hStdout,
									FOREGROUND_BLUE |
									FOREGROUND_GREEN |
									FOREGROUND_INTENSITY);
			std::cin >> InputVer;
			if( InputVer.compare("quit") == 0 )
			{
				return 0;
			}
			if( DLLFiles.count(InputVer) == 0 )
			{
				SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << "Invalid version." << std::endl;
				continue;
			}
		} while( DLLFiles.count(InputVer) == 0 );
		DLLFile = DLLFiles[InputVer];
		break;
	}

	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "Injecting " << DLLFile << " into process ID " << ProcessID << ": ";
	if( DLLInjectRemote(ProcessID, CurrentDirectory + "\\" + DLLFile) )
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		//Beep(1046, 675);
		std::cout << "Success!" << std::endl;
		return 0;
	}
	else
	{
		SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "Failed" << std::endl;
	}
	system("pause");
	return 0;
}