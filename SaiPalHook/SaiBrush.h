#pragma once
#include <string>

#include "Pointers.h"
#include "SaiTypes.h"

class SaiBrush
{
public:
	SaiBrush();
	SaiBrush(const Pointer Brush);
	~SaiBrush();

	std::string GetName() const;
	void SetName(const std::string& Name);

	std::string GetDescription() const;
	void SetDescription(const std::string& Description);

	std::string GetToolName() const;
	void SetToolName(const std::string& ToolName);

	SaiTool::Type GetType() const;
	void SetType(const SaiTool::Type Type);

	SaiTool::Stabilizer GetStrokeStabilizser() const;
	SaiTool::Stabilizer GetPressureStabilizer() const;

	unsigned char GetShortcutKey() const;

	SaiTool::EdgeShape GetTipShape() const;
	float GetSize() const;
	unsigned char GetEdgeHardness() const;
	unsigned char GetDensity() const;
	unsigned char GetHardSoft() const;
	float GetMinSize() const;
	unsigned char GetMinDensity() const;
	float GetMaxDensityPrs() const;
	unsigned char GetBlending() const;
	unsigned char GetDilution() const;
	unsigned char GetPersistence() const;
	unsigned char GetPressureFlags() const;

private:
	Pointer Brush;
};