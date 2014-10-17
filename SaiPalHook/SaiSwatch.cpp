#include "SaiSwatch.h"

#include "SaiPalConfig.h"

#define SwatchWidth 12
#define SwatchHeight 13

#include <stdlib.h> // _aligned_malloc
#include <emmintrin.h> // _mm_set_epi8 etc
#include <tmmintrin.h> // _mm_shuffle_epi8

#include "stb_image_write.h"
#include "stb_image.h"
#include "stb_image_resize.h"

SaiSwatch::SaiSwatch() : Swatch(nullptr)
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
	if( !Swatch )
	{
		return 0;
	}
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
	if( !Swatch )
	{
		return 0;
	}
	if( X > 12 || Y > 13 )
	{
		return 0;
	}
	return Swatch(X + Y * 13).asUint();
}

unsigned int* SaiSwatch::RawPtr()
{
	if( !Swatch )
	{
		return nullptr;
	}
	return (unsigned int*)Swatch();
}

bool SaiSwatch::SaveSwatch(const std::string Path)
{
	if( !Swatch )
	{
		return false;
	}
	//16 byte aligned buffer
	unsigned int * Buffer =
		(unsigned int*)_aligned_malloc(
		SwatchWidth*SwatchHeight * 4
		, 16);

	// BGRA to RGBA
	__m128i shuffle =
		_mm_set_epi8(
		15, 12, 13, 14,
		11, 8, 9, 10,
		7, 4, 5, 6,
		3, 0, 1, 2);
	//Four pixels at a time
	for( unsigned int i = 0;
		i < (SwatchWidth*SwatchHeight);
		i += 4 )
	{
		__m128i QuadPixel =
			_mm_loadu_si128((__m128i*)
			Swatch(i, sizeof(unsigned int)));
		QuadPixel = _mm_shuffle_epi8(QuadPixel, shuffle);
		_mm_store_si128((__m128i*)&Buffer[i], QuadPixel);
	}

	if( stbi_write_png(Path.c_str(),
		SwatchWidth, SwatchHeight,
		4, Buffer, 0) )
	{
		_aligned_free(Buffer);
		return true;
	}
	_aligned_free(Buffer);
	return false;
}

bool SaiSwatch::ReadSwatch(const std::string Path)
{
	int Width, Height, Channels;
	unsigned int* Image
		= (unsigned int*)stbi_load(
		Path.c_str(),
		&Width,
		&Height,
		&Channels, 4);
	if( !Image )
	{
		stbi_image_free(Image);
		return false;
	}
	//if( Width < SwatchWidth ||
	//   Height < SwatchHeight )
	//{
	//	//Image size is less than swatch size
	//	//Todo: Take bilinear samples regardless of the image's size
	//	stbi_image_free(Image);
	//	return false;
	//}

	//16 byte aligned buffer
	unsigned int * Buffer =
		(unsigned int*)_aligned_malloc(
		SwatchWidth*SwatchHeight * 4
		, 16);
	printf("resize\n");
	//resize image to 12x13
	stbir_resize_uint8(
		(const unsigned char*)Image, Width, Height, 0,
		(unsigned char*)Buffer, SwatchWidth, SwatchHeight,
		0, 4);

	printf("rgba to bgra\n");
	//convert RGBA to BGRA
	__m128i shuffle =
		_mm_set_epi8(
		15, 12, 13, 14,
		11, 8, 9, 10,
		7, 4, 5, 6,
		3, 0, 1, 2);
	//Four pixels at a time
	printf("Buffer ptr: %x\n", Buffer);
	for( unsigned int i = 0;
		i < (SwatchWidth*SwatchHeight);
		i += 4 )
	{
		printf("Buffer ptr: %x\n", &Buffer[i]);
		__m128i QuadPixel =
			_mm_load_si128((__m128i*)&Buffer[i]);
		QuadPixel = _mm_shuffle_epi8(QuadPixel, shuffle);
		_mm_store_si128((__m128i*)&Buffer[i], QuadPixel);
	}

	memcpy(Swatch, Buffer, SwatchHeight*SwatchWidth * 4);
	_aligned_free(Buffer);
	stbi_image_free(Image);
	return true;
}