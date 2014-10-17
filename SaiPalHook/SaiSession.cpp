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
	if( !Session )
	{
		return;
	}
	this->Session = Session;
}

SaiSession::~SaiSession()
{
}

HWND SaiSession::GetWindow()
{
	if( !Session )
	{
		return HWND();
	}
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

SaiSwatch SaiSession::GetSwatches()
{
	return SaiSwatch(Session(0x886));
}

SaiColor SaiSession::GetPrimaryColor() const
{
	if( !Session )
	{
		return SaiColor();
	}
	return Session(0x874).as<SaiColor>();
}

void SaiSession::SetPrimaryColor(unsigned char R, unsigned char G, unsigned char B)
{
	if( !Session )
	{
		return;
	}
	SetPrimaryColor(SaiColor(R, G, B));
}

void SaiSession::SetPrimaryColor(const SaiColor& NewColor)
{
	if( !Session )
	{
		return;
	}
	*(SaiColor*)Session(0x874) = NewColor;
}

SaiColor SaiSession::GetSecondaryColor() const
{
	if( !Session )
	{
		return SaiColor();
	}
	return Session(0x87C).as<SaiColor>();
}

void SaiSession::SetSecondaryColor(unsigned char R, unsigned char G, unsigned char B)
{
	if( !Session )
	{
		return;
	}
	SetSecondaryColor(SaiColor(R, G, B));
}

void SaiSession::SetSecondaryColor(const SaiColor& NewColor)
{
	if( !Session )
	{
		return;
	}
	*(SaiColor*)Session(0x87C) = NewColor;
}