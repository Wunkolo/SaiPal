#pragma once
#include "Pointers.h"

class SaiSession
{
public:
	SaiSession();
	SaiSession(Pointer Session);
	~SaiSession();

	//May be null. Resolve([0]) to verify
	Pointer ActiveCanvas();

private:
	Pointer Session;
};