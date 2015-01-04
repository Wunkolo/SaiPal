#include "GenericBrush.h"


GenericBrush::GenericBrush()
	:
	mType(BrushType::Brush),
	mHotKey(0)
{
}

GenericBrush::~GenericBrush()
{

}

BrushType GenericBrush::GetType() const
{
	return mType;
}

void GenericBrush::SetType(BrushType Type)
{
	mType = Type;
}

std::string GenericBrush::GetName() const
{
	return std::string(mName, 64);
}

void GenericBrush::SetName(std::string Name)
{
	mName = Name.substr(0, 64);
}

std::string GenericBrush::GetDescription() const
{
	return std::string(mDescription, 64);
}

void GenericBrush::SetDescription(std::string Description)
{
	mDescription = Description.substr(0, 64);
}

unsigned short GenericBrush::GetHotkey() const
{
	return mHotKey;
}

void GenericBrush::SetHotkey(unsigned short Key)
{
	mHotKey = Key;
}