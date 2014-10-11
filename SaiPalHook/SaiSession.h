#pragma once
#include "Pointers.h"

#include "SaiTypes.h"

class SaiSession
{
public:
	SaiSession();
	SaiSession(Pointer Session);
	~SaiSession();

	//May be null. Resolve([0]) to verify
	Pointer ActiveCanvas();

	Color GetPrimaryColor();
	void SetPrimaryColor(unsigned char R, unsigned char G, unsigned char B);
	void SetPrimaryColor(const Color& NewColor);
private:
	Pointer Session;
};