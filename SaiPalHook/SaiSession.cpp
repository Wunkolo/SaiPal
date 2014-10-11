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