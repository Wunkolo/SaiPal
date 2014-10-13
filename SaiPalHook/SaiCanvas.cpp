#include "SaiCanvas.h"

#include "SaiPalConfig.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
	if (!Canvas)
	{
		return false;
	}
	unsigned int LowerPadX = Canvas(0x114).asUint();
	unsigned int LowerPadY = Canvas(0x118).asUint();
	//unsigned int HigherPadX = Canvas(0x11C).asUint();
	//unsigned int HigherPadY = Canvas(0x120).asUint();
	unsigned int * Buffer = new unsigned int[Width()*Height()];

	Pointer PixelHeap = Canvas[0x30];

	unsigned int StrideX = PixelHeap(1, sizeof(int)).asUint();
	//unsigned int StrideY = PixelHeap(2, sizeof(int)).asUint();
	PixelHeap = PixelHeap[0xC];

	for (unsigned int x = 0; x < Width(); x++)
	{
		for (unsigned int y = 0; y < Height(); y++)
		{
			unsigned int Pixel =
				PixelHeap((x + LowerPadX) + (y + LowerPadY) * StrideX, sizeof(int)).asUint();
			//BGRA to RGBA(Little endian)
			Pixel = ((Pixel & 0x00ff0000) >> 16) | //Blue
				((Pixel & 0x000000ff) << 16) |     //Red
				(Pixel & 0xff00ff00);              //Green+Alpha
			Buffer[Width() * y + x] = Pixel;
		}
	}
	if (stbi_write_png(Path.c_str(), Width(), Height(), 4, Buffer, 0))
	{
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