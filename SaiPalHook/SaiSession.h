#pragma once
#include <string>

#define NOMINMAX
#include <Windows.h>
#include "Pointers.h"

#include "SaiTypes.h"

class SaiSession
{
public:
	SaiSession();
	SaiSession(Pointer Session);
	~SaiSession();

	HWND GetWindow();

	//Status text on bottom of Sai Window
	void SetStatusText(const std::string Text);

	//May be null. Resolve([0]) to verify
	Pointer ActiveCanvas();
	Pointer NewCanvas(std::string CanvasName,
					  unsigned int Width = 500,
					  unsigned int Height = 500,
					  unsigned short Args = 1);

	HWND GetSwatchWindow();
	HWND GetLayerListWindow();
	HWND GetPreviewWindow();
	SaiSwatch GetSwatch();
	SaiColor GetPrimaryColor() const;
	void SetPrimaryColor(unsigned char R, unsigned char G, unsigned char B);
	void SetPrimaryColor(const SaiColor& NewColor);
	SaiColor GetSecondaryColor() const;
	void SetSecondaryColor(unsigned char R, unsigned char G, unsigned char B);
	void SetSecondaryColor(const SaiColor& NewColor);
private:
	Pointer Session;
};