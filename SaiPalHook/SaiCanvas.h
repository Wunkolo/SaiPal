#pragma once
#include <string>

#include "Pointers.h"
#include "SaiTypes.h"

class SaiCanvas
{
public:
	SaiCanvas();
	SaiCanvas(const Pointer Canvas);
	~SaiCanvas();

	std::string GetName();

	bool CaptureImage(const std::string Path);

	inline unsigned int Width() const;
	inline unsigned int Height() const;

private:
	Pointer Canvas;
};
