#pragma once

#pragma pack()
struct Color
{
	Color()
	{
		B = G = R = 128;
	}
	signed short B, G, R;
};