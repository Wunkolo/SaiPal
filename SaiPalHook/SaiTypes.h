#pragma once

#pragma pack()
struct Color
{
	Color()
	{
		B = G = R = 128;
	}
	Color(unsigned short R, unsigned short G, unsigned short B)
	{
		this->R = 128 + (R << 8);
		this->G = 128 + (G << 8);
		this->B = 128 + (B << 8);
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
