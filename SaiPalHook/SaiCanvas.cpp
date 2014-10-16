#include "SaiCanvas.h"

#include "SaiPalConfig.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <type_traits>

#include <stdlib.h> // _aligned_malloc
#include <emmintrin.h> // _mm_set_epi8 etc
#include <tmmintrin.h> // _mm_shuffle_epi8

SaiCanvas::SaiCanvas()
{
}

SaiCanvas::SaiCanvas(const Pointer Canvas)
{
	this->Canvas = Canvas;
}

SaiCanvas::~SaiCanvas()
{
}

std::string SaiCanvas::GetName() const
{
#if defined(SAI120)
	return std::string((char*)Canvas(0x5d8));
#elif defined(SAI110)
	return std::string((char*)Canvas(0x4d8));
#else
	return std::string((char*)Canvas(0x4d8));
#endif
}

bool SaiCanvas::CaptureImage(const std::string Path)
{
	if( !Canvas )
	{
		return false;
	}
	unsigned int LowerPadX = Canvas(0x114).asUint();
	unsigned int LowerPadY = Canvas(0x118).asUint();
	unsigned int HigherPadX = Canvas(0x11C).asUint();
	unsigned int HigherPadY = Canvas(0x120).asUint();
	Pointer PixelHeap = Canvas[0x30];

	unsigned int Stride = PixelHeap(1, sizeof(int)).asUint();
	//unsigned int StrideY = PixelHeap(2, sizeof(int)).asUint();
	PixelHeap = PixelHeap[0xC];
	//16 byte aligned buffer
	unsigned int * Buffer = new unsigned int[Width()*Height()];

	// BGRA to RGBA
	__m128i shuffle =
		_mm_set_epi8(
		15, 12, 13, 14,
		11, 8, 9, 10,
		7, 4, 5, 6,
		3, 0, 1, 2);
	//Process four pixels at a time

	for( unsigned int y = 0; y < Height(); y++ )
	{
		for( unsigned int x = 0; x < Width(); x += 4 )
		{
			__m128i QuadPixel =
				_mm_loadu_si128((__m128i*)
				PixelHeap((x + LowerPadX) + (y + LowerPadY) * Stride,
				sizeof(int)));
			QuadPixel = _mm_shuffle_epi8(QuadPixel, shuffle);
			_mm_store_si128(
				(__m128i*)&Buffer[Width() * y + x],
				QuadPixel);
		}
	}

	//for( unsigned int x = 0; x < Width(); x++ )
	//{
	//	for( unsigned int y = 0; y < Height(); y++ )
	//	{
	//		unsigned int Pixel =
	//			PixelHeap(
	//			(x + LowerPadX) + (y + LowerPadY) * StrideX,
	//			sizeof(int)).asUint();
	//		//BGRA to RGBA(Little endian)
	//		Pixel = (
	//			(Pixel & 0x00ff0000) >> 16) |      //Blue
	//			((Pixel & 0x000000ff) << 16) |     //Red
	//			(Pixel & 0xff00ff00);              //Green+Alpha
	//		Buffer[Width() * y + x] = Pixel;
	//	}
	//}

	if( stbi_write_png(Path.c_str(), Width(), Height(), 4, Buffer, 0) )
	{
		delete[] Buffer;
		return true;
	}
	delete[] Buffer;
	return false;
}

unsigned int SaiCanvas::Width() const
{
	return Canvas(0x124).asUint();
}

unsigned int SaiCanvas::Height() const
{
	return Canvas(0x128).asUint();
}