#pragma once

#include "Pointers.h"

class SaiSwatch
{
public:
	SaiSwatch();
	SaiSwatch(const Pointer Swatch);
	~SaiSwatch();

	//12 x 13
	unsigned int GetSwatchRGBA(unsigned char X,
							   unsigned char Y);
	unsigned int GetSwatchARGB(unsigned char X,
							   unsigned char Y);

	//BGRA pixel array
	unsigned int* RawPtr();

private:
	Pointer Swatch;
};