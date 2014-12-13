#include "SaiBrush.h"

#include "SaiPalConfig.h"

SaiBrush::SaiBrush() : Brush(nullptr)
{
}

SaiBrush::SaiBrush(const Pointer Brush) : Brush(Brush)
{
}

SaiBrush::~SaiBrush()
{
}

std::string SaiBrush::GetName() const
{
	if( !Brush )
	{
		return "";
	}
	return std::string((char*)Brush(0x4C));
}

std::string SaiBrush::GetDescription() const
{
	if( !Brush )
	{
		return "";
	}
	return std::string((char*)Brush(0x8C));
}

std::string SaiBrush::GetToolName() const
{
	if( !Brush )
	{
		return "";
	}
	return std::string((char*)Brush(0xCC));
}

SaiTool::Type SaiBrush::GetType() const
{
	if( !Brush )
	{
		return SaiTool::Type::Unknown;
	}
	return (SaiTool::Type)*(unsigned int*)Brush(0x28);
}

SaiTool::Stabilizer SaiBrush::GetStrokeStabilizser() const
{
	if( !Brush )
	{
		return SaiTool::Stabilizer::NotSpecified;
	}
	return (SaiTool::Stabilizer)*(unsigned int*)Brush(0x44);
}

SaiTool::Stabilizer SaiBrush::GetPressureStabilizer() const
{
	if( !Brush )
	{
		return SaiTool::Stabilizer::NotSpecified;
	}
	return (SaiTool::Stabilizer)*(unsigned int*)Brush(0x48);
}

unsigned char SaiBrush::GetShortcutKey() const
{
	if( !Brush )
	{
		return '\0';
	}
	return *(unsigned char*)Brush(0xEC);
}

SaiTool::EdgeShape SaiBrush::GetTipShape() const
{
	if( !Brush )
	{
		return SaiTool::EdgeShape::Gaussian;
	}
	return (SaiTool::EdgeShape)*(unsigned char*)Brush(0x44);
}

float SaiBrush::GetSize() const
{
	if( !Brush )
	{
		return 0.0f;
	}
	return (*(unsigned int*)Brush(0x128))*0.1f;
}

unsigned char SaiBrush::GetEdgeHardness() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x134);
}

unsigned char SaiBrush::GetDensity() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x138);
}

unsigned char SaiBrush::GetHardSoft() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x13C);
}

float SaiBrush::GetMinSize() const
{
	if( !Brush )
	{
		return 0.0f;
	}
	return (*(unsigned char*)Brush(0x140)) / 100.0f;
}

unsigned char SaiBrush::GetMinDensity() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x144);
}

float SaiBrush::GetMaxDensityPrs() const
{
	if( !Brush )
	{
		return 0.0f;
	}
	return (*(unsigned char*)Brush(0x148)) / 100.0f;
}

unsigned char SaiBrush::GetBlending() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x15C);
}

unsigned char SaiBrush::GetDilution() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x160);
}

unsigned char SaiBrush::GetPersistence() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x164);
}

unsigned char SaiBrush::GetPressureFlags() const
{
	if( !Brush )
	{
		return 0;
	}
	return *(unsigned char*)Brush(0x170);
}