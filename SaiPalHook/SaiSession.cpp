#include "SaiSession.h"

#include "SaiPalConfig.h"

#include <iostream>

SaiSession::SaiSession()
{
	Pointer Base(GetBasePointer());
	Session = Base[SESSION];
}

SaiSession::SaiSession(Pointer Session)
{
	this->Session = Session;
}

SaiSession::~SaiSession()
{
}

HWND SaiSession::GetWindow()
{
	return Session(0xA0).as<HWND>();
}

Pointer SaiSession::ActiveCanvas()
{
	return Session(0x3C4);
}

Pointer SaiSession::NewCanvas(std::string CanvasName,
	unsigned int Width,
	unsigned int Height,
	unsigned short Args)
{
#if defined(SAI120)
	//void*(cdecl *CreateCanvas)(void* Session,int width, int height, short flags,const char* name) =
	//	(void*(*)(void*,int,int,short,const char*))((unsigned int)GetBasePointer() + 0x9F270);
	//Pointer Canv = CreateCanvas(Session(), Width, Height, Args, CanvasName.c_str());
	//std::cout << "Create Canvas: Offset: " << (void*)Canv << std::endl;
	//*(int*)Canv(0x14c) = 0x480000;
	//*(short*)Canv(0x150) = 0;
	//*(short*)Canv(0x152) = 0;

	//((int(*)(void*))((unsigned int)GetBasePointer() + 0xE1DE70))(Canv);
	//((int(*)(void*))((unsigned int)GetBasePointer() + 0xE1E190))(Canv);

	//((HWND(__stdcall *)(void*, unsigned int, char))((unsigned int)GetBasePointer() + 0xDa0C10))(Session(), 0x100, 0x100);

	return nullptr;
#endif
	return nullptr;
}

Color SaiSession::GetPrimaryColor()
{
	return Session(0x874).as<Color>();
}

void SaiSession::SetPrimaryColor(unsigned char R, unsigned char G, unsigned char B)
{
	SetPrimaryColor(Color(R, G, B));
}

void SaiSession::SetPrimaryColor(const Color& NewColor)
{
	*(Color*)Session(0x874) = NewColor;
}

Color SaiSession::GetSecondaryColor()
{
	return Session(0x87C).as<Color>();
}

void SaiSession::SetSecondaryColor(unsigned char R, unsigned char G, unsigned char B)
{
	SetSecondaryColor(Color(R, G, B));
}

void SaiSession::SetSecondaryColor(const Color& NewColor)
{
	*(Color*)Session(0x87C) = NewColor;
}