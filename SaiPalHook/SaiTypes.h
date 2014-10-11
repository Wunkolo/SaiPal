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
		this->R = 128+(R << 8);
		this->G = 128+(G << 8);
		this->B = 128+(B << 8);
	}
	unsigned short B, G, R;
};