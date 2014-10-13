#pragma once
#include <string>

#include "Pointers.h"
#include "SaiTypes.h"

class SaiCanvas
{
public:
	SaiCanvas();
	SaiCanvas(Pointer Canvas);
	~SaiCanvas();

	std::string GetName();

	bool CaptureImage(const std::string Path);

	unsigned int Width();
	unsigned int Height();

private:
	Pointer Canvas;
};
