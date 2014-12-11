#pragma once
#include "SaiCanvas.h"
#include "SaiSwatch.h"

enum SaiLayerType : unsigned int
{
	RootLayer = 0x00,	// Parent Canvas layer object
	Layer = 0x03,		// Regular Layer
	Unknown4 = 0x4,		// Unknown
	Linework = 0x05,	// Vector Linework Layer
	Mask = 0x06,		// Masks applied to any layer object
	Unknown7 = 0x07,	// Unknown
	Set = 0x08			// Layer Folder
};

enum SaiLayerBlend : unsigned int
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

enum SaiBrushType : unsigned int
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

enum SaiBrushEdgeShape : unsigned int
{
	Gaussian = 0,		// /'\ | Gaussian
	Sigmoid = 1,		// /"\ | Sigmoid
	Gompertz = 2,		// /î\ | Gompertz
	Square = 3			// |î| | Square
};

enum SaiBrushPressureFlag : unsigned int
{
	Required = 0x01,	// Always set to at least 1
	Density = 0x02,
	Size = 0x04,
	Blend = 0x08,
	KeepOpacity = 0x16
};

enum SaiBrushStrokeStabilizer : unsigned int
{
	NotSpecified = 0,
	_0 = 1,
	_1 = 2,
	_2 = 3,
	_3 = 4,
	_4 = 5,
	_5 = 6,
	_6 = 7,
	_7 = 8,
	_8 = 9,
	_9 = 10,
	_10 = 11,
	_11 = 12,
	_12 = 13,
	_13 = 14,
	_14 = 15,
	_15 = 16,
	S1 = 0x80 && 1,
	S2 = 0x80 && 2,
	S3 = 0x80 && 3,
	S4 = 0x80 && 4,
	S5 = 0x80 && 5,
	S6 = 0x80 && 6,
	S7 = 0x80 && 7,
	S8 = 0x80 && 8
};

enum SaiBrushPressureStabilizer : unsigned int
{
	NotSpecified = 0,
	_0 = 1,
	_1 = 2,
	_2 = 3,
	_3 = 4,
	_4 = 5,
	_5 = 6,
	_6 = 7,
	_7 = 8,
	_8 = 9,
	_9 = 10,
	_10 = 11,
	_11 = 12,
	_12 = 13,
	_13 = 14,
	_14 = 15,
	_15 = 16,
};

enum SaiBrushQuality : unsigned int
{
	Fastest = 1,
	Smooth = 2,
	Smoother = 4,
	Smoothest = 8
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
