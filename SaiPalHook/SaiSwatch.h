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
	unsigned int GetSwatchBGRA(unsigned char X,
							   unsigned char Y);
	//12 x 13
	void SetSwatchRGBA(unsigned char X,
					   unsigned char Y,
					   unsigned int Color);
	void SetSwatchBGRA(unsigned char X,
					   unsigned char Y,
					   unsigned int Color);

	//BGRA pixel array
	unsigned int* RawPtr();

	bool SaveSwatch(const std::string Path,
					unsigned int Scale = 1);
	bool LoadSwatch(const std::string Path);
private:
	Pointer Swatch;
};