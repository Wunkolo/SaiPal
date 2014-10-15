#pragma once
#include <string>

#define NOMINMAX
#include <Windows.h>
#include "Pointers.h"

#include "SaiTypes.h"
#include "SaiCanvas.h"

class SaiSession
{
public:
	SaiSession();
	SaiSession(Pointer Session);
	~SaiSession();

	HWND GetWindow();

	//May be null. Resolve([0]) to verify
	Pointer ActiveCanvas();
	Pointer NewCanvas(std::string CanvasName,
					  unsigned int Width = 500,
					  unsigned int Height = 500,
					  unsigned short Args = 1);

	SaiColor GetPrimaryColor() const;
	void SetPrimaryColor(unsigned char R, unsigned char G, unsigned char B);
	void SetPrimaryColor(const SaiColor& NewColor);
	SaiColor GetSecondaryColor() const;
	void SetSecondaryColor(unsigned char R, unsigned char G, unsigned char B);
	void SetSecondaryColor(const SaiColor& NewColor);
private:
	Pointer Session;
};