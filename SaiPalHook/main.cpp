#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <fcntl.h> //_O_TEXT
#include <io.h> //_open_osfhandle
#include <conio.h> // getch

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "SaiPal.h"

void SaveCanvasImage(Pointer Canvas)
{
	if (!Canvas)
	{
		return;
	}

#if defined(SAI120)
	Canvas = Canvas(0x100);
#endif

	//std::cout << "Saving canvas: " << (void*)Canvas << std::endl;
	std::string CanvasName((char*)Canvas(0x4d8));
	CanvasName = CanvasName.substr(
		CanvasName.find_last_of('\\') == std::string::npos ? 0 : CanvasName.find_last_of('\\') + 1,
		std::string::npos);
	CanvasName.erase(CanvasName.begin() + CanvasName.find_last_of('.'), CanvasName.end());
	::CreateDirectoryA(std::string("SaiPal\\" + CanvasName).c_str(), NULL);

	CanvasName = "SaiPal\\" + CanvasName + "\\" + CanvasName;

	SYSTEMTIME st;
	GetSystemTime(&st);

	CanvasName += " (" + std::to_string(std::time(nullptr)) + "_" + std::to_string(st.wMilliseconds) + ")";
	CanvasName += ".png";

#if defined(SAI120)
	Canvas = Canvas(-0x100);
#endif
	unsigned int LowerPadX = Canvas(0x114).asUint();
	unsigned int LowerPadY = Canvas(0x118).asUint();
	unsigned int HigherPadX = Canvas(0x11C).asUint();
	unsigned int HigherPadY = Canvas(0x120).asUint();
	unsigned int Width = Canvas(0x124).asUint();
	unsigned int Height = Canvas(0x128).asUint();
	//std::cout << std::dec
	//	<< Width << " x "
	//	<< Height << std::endl;

	unsigned int * Buffer = new unsigned int[Width*Height];

	Pointer PixelHeap = Canvas[0x30];
	//std::cout << std::hex << "PixelAloc " << (void*)PixelHeap << std::endl;
	unsigned int StrideX = PixelHeap(1, sizeof(int)).asUint();
	//unsigned int StrideY = PixelHeap(2,sizeof(int)).asUint();
	PixelHeap = PixelHeap[0xC];
	//std::cout << std::hex << "Stride: " << StrideX << ":" << StrideY << std::endl;
	//std::cout << std::hex << "PixelHeap " << (void*)PixelHeap << std::endl;
	//system("pause");

	for (unsigned int x = LowerPadX; x < Width; x++)
	{
		for (unsigned int y = LowerPadX; y < Height; y++)
		{
			//std::cout << std::dec << x << ":" << y << "= " << std::hex << PixelHeap(x + y * StrideX,sizeof(int)).asUint() << std::endl;
			unsigned int Pixel =
				PixelHeap(x + y * StrideX, sizeof(int)).asUint();
			//BGRA to RGBA(Little endian)
			Pixel = ((Pixel & 0x00ff0000) >> 16) | //Blue
				((Pixel & 0x000000ff) << 16) |     //Red
				(Pixel & 0xff00ff00);              //Green+Alpha
			Buffer[Width * y + x] = Pixel;
		}
	}
	stbi_write_png(CanvasName.c_str(), Width, Height, 4, Buffer, 0);
	delete[] Buffer;
	std::cout << "Saved Canvas to: " << CanvasName << std::endl;
}

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

	while (1)
	{
		//if (Canvas[0])
		//{
		//	//void* Layer = (void*)*(unsigned int*)(*(unsigned int*)Canvas + 0x168);//first layer
		//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		//	//std::cout << "Active Canvas: " << (void*)*(unsigned int*)Canvas
		//	//	<< " First Layer: " << Layer << std::endl;
		//	//ProcessLayerList(Layer);//first layer
		//	//win_redraw(SaiWindow, 0);
		//	//RenderLayers(Canvas,Layer);
		//	//std::cout << (void*)*(unsigned int*)Canvas << std::endl;
		//	SaveCanvasImage(Canvas[0]);
		//	//std::cout << SaveCanvasToFile << " Result " << SaveCanvasToFile((int)Session, (int)Canvas, "asdf.png", true, false) << std::endl;
		//}

		SaiPal::Instance().Tick(std::chrono::duration<double>(1.0));
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD Reason, LPVOID Misc)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hDLL);

		//Set up new console window
		AllocConsole();
		::SetConsoleTitleA("SaiPal");
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		::EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE | SC_MINIMIZE, MF_GRAYED);
		::DrawMenuBar(GetConsoleWindow());
		//set CIN
		::GetConsoleScreenBufferInfo(GetStdHandle(STD_INPUT_HANDLE), &consoleInfo);
		consoleInfo.dwSize.Y = 1024;
		SetConsoleScreenBufferSize(GetStdHandle(STD_INPUT_HANDLE), consoleInfo.dwSize);
		HANDLE hStd = ::GetStdHandle(STD_INPUT_HANDLE); // STD in Handle
		unsigned int hConsole = _open_osfhandle((intptr_t)hStd, _O_TEXT); // Console Handle
		FILE* fIn = ::_fdopen(hConsole, "r");
		*stdin = *fIn;
		setvbuf(stdin, NULL, _IONBF, 0);
		//setup COUT
		::GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
		consoleInfo.dwSize.Y = 1024;
		::SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), consoleInfo.dwSize);
		hStd = ::GetStdHandle(STD_OUTPUT_HANDLE); // STD out Handle
		hConsole = _open_osfhandle((intptr_t)hStd, _O_TEXT); // Console Handle
		FILE* fOut = _fdopen(hConsole, "w");
		*stdout = *fOut;
		setvbuf(stdout, NULL, _IONBF, 0);
		std::ios::sync_with_stdio();

		CreateThread(
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