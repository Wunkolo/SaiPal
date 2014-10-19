#pragma once
#include <string>

#include "Pointers.h"
#include "SaiTypes.h"

#define NOMINMAX
#include <Windows.h>

class SaiCanvas
{
public:
	SaiCanvas();
	SaiCanvas(const Pointer Canvas);
	~SaiCanvas();

	std::string GetName() const;

	HWND GetWindow();

	Pointer GetFirstLayer();

	bool CaptureImage(const std::string Path);

	inline unsigned int Width() const;
	inline unsigned int Height() const;

private:
	Pointer Canvas;
};
