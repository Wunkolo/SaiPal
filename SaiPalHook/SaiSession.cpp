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

Pointer SaiSession::ActiveCanvas()
{
	return Session(0x3C4);
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