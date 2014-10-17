#pragma once
#include "SaiModule.h"

class Swatch : public SaiModule
{
public:
	Swatch();
	~Swatch();
	std::string Info() const;
	void Tick(const std::chrono::duration<double>& Delta);
	void Run(const std::vector<std::string>& Args);
private:
};
