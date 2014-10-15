#pragma once
#include "SaiModule.h"
#include "../SaiTypes.h"

#include <vector>
#include <map>

typedef void(*Mutator)(SaiColor&);

class Color : public SaiModule
{
public:
	Color();
	~Color();
	std::string Info() const;
	void Tick(const std::chrono::duration<double>& Delta);
	void Run(const std::vector<std::string>& Args);
private:
	// Values are from 0..1
	static void RGBtoHSV(
		float r, float g, float b,
		float &h, float&s, float&v);
	static void HSVtoRGB(
		float h, float s, float v,
		float &r, float &g, float &b);

	std::map <
		std::vector<std::string>, // List of names to go by
		Mutator      // Mutator function
	> Mutators;

	void(*CurMutator)(SaiColor& color);
	bool Running, Active;
};