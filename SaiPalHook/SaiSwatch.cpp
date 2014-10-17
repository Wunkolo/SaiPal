#include "SaiSwatch.h"

#include "SaiPalConfig.h"

#define SwatchWidth 12
#define SwatchHeight 13

inline unsigned int endian_swap(unsigned const int& x)
{
	return (((x & 0x000000FF) << 24) |
			((x & 0x0000FF00) << 8) |
			((x & 0x00FF0000) >> 8) |
			((x & 0xFF000000) >> 24));
}

SaiSwatch::SaiSwatch()
{
}
SaiSwatch::SaiSwatch(const Pointer Swatch)
{
	this->Swatch = Swatch;
}
SaiSwatch::~SaiSwatch()
{
}

unsigned int SaiSwatch::GetSwatchRGBA(unsigned char X,
									  unsigned char Y)
{
	if( X > 12 || Y > 13 )
	{
		return 0;
	}
	unsigned int Pixel = GetSwatchARGB(X, Y);
	//BGRA -> RGBA
	Pixel = (((Pixel & 0x000000FF) << 24) |
			 ((Pixel & 0x0000FF00) << 8) |
			 ((Pixel & 0x00FF0000) >> 8) |
			 ((Pixel & 0xFF000000) >> 24));
	return Pixel;
}
unsigned int SaiSwatch::GetSwatchARGB(unsigned char X,
									  unsigned char Y)
{
	if( X > 12 || Y > 13 )
	{
		return 0;
	}
	return Swatch(X + Y * 13).asUint();
}

unsigned int* RawPtr()
{
}