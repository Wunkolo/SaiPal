#pragma once
#include "SaiCanvas.h"
#include "SaiSwatch.h"

enum SaiLayerType
{
	RootLayer = 0x00,	// Parent Canvas layer object
	Layer = 0x03,		// Regular Layer
	Unknown4 = 0x4,		// Unknown
	Linework = 0x05,	// Vector Linework Layer
	Mask = 0x06,		// Masks applied to any layer object
	Unknown7 = 0x07,	// Unknown
	Set = 0x08			// Layer Folder
};

enum SaiLayerBlend
{
	Pass = 0x00,		// Pass-through. Only available to Sets(folders)
	Normal = 0x01,
	Multiply = 0x02,
	Screen = 0x03,
	Overlay = 0x04,
	Luminosity = 0x05,
	Shade = 0x06,
	LumiShade = 0x07,	// Lumi & Shade
	Binary = 0x08		// Binary Color
};

enum SaiBrushType
{
	Pencil = 1,
	AirBrush = 2,
	Brush = 3,
	Water = 4,
	Marker = 5,
	Eraser = 16,
	MaskPen = 17,
	Masker = 18,
	// Ink Mode Brushes
	InkPen = 48,
	InkParam = 52,
	Inkers = 53,
	InkCurve = 54,
	InkLine = 55
};

#pragma pack()
struct SaiColor
{
	SaiColor()
	{
		B = G = R = 128;
	}
	SaiColor(unsigned short R, unsigned short G, unsigned short B)
	{
		this->R = 128 + (R << 8);
		this->G = 128 + (G << 8);
		this->B = 128 + (B << 8);
	}
	SaiColor(unsigned int RGBX)
	{
		this->R = 128 + (((RGBX >> 24) & 0xff) << 8);
		this->G = 128 + (((RGBX >> 16) & 0xff) << 8);
		this->B = 128 + (((RGBX >> 8) & 0xff) << 8);
	}
	unsigned int RGBX()
	{
		unsigned int RGBX = 0;
		RGBX |= Red() << 24;
		RGBX |= Green() << 16;
		RGBX |= Blue() << 8;
		return RGBX;
	}
	unsigned char Red()
	{
		return (R >> 8);
	}
	unsigned char Blue()
	{
		return (B >> 8);
	}
	unsigned char Green()
	{
		return (G >> 8);
	}
	float Redf()
	{
		return (R >> 8) / (float)0xff;
	}
	float Bluef()
	{
		return (B >> 8) / (float)0xff;
	}
	float Greenf()
	{
		return (G >> 8) / (float)0xff;
	}
	void SetF(float R, float G, float B)
	{
		this->R = 128 + (unsigned char(R * 0xff) << 8);
		this->G = 128 + (unsigned char(G * 0xff) << 8);
		this->B = 128 + (unsigned char(B * 0xff) << 8);
	}
	void Set(unsigned short R, unsigned short G, unsigned short B)
	{
		this->R = 128 + (R << 8);
		this->G = 128 + (G << 8);
		this->B = 128 + (B << 8);
	}
	void SetRGBX(unsigned int RGBX)
	{
		this->R = 128 + (((RGBX >> 24) & 0xff) << 8);
		this->G = 128 + (((RGBX >> 16) & 0xff) << 8);
		this->B = 128 + (((RGBX >> 8) & 0xff) << 8);
	}
	unsigned short B, G, R;
};

#pragma pack()
struct Rect
{
	Rect()
	{
		X = Y = Width = Height = 0;
	}
	Rect(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height)
	{
		this->X = X;
		this->Y = Y;
		this->Width = Width;
		this->Height = Height;
	}
	unsigned int X, Y, Width, Height;
};
