#pragma once

#include <string>
#include "BrushTypes.h"

class GenericBrush
{
public:
	GenericBrush();
	~GenericBrush();

	BrushType GetType() const;

	void SetType(BrushType Type);

	std::string GetName() const;

	void SetName(std::string Name);

	std::string GetDescription() const;

	void SetDescription(std::string Description);

	unsigned short GetHotkey() const;

	void SetHotkey(unsigned short Key);

private:
	BrushType mType;
	std::string mName;
	std::string mDescription;
	unsigned short mHotKey;
};