#pragma once
#include "SaiModule.h"

class Test : public SaiModule
{
public:
	Test();
	~Test();
	std::string Info() const;
	void Tick(const std::chrono::duration<double>& Delta);
	void Run(const std::vector<std::string>& Args);
private:
	bool Running;
};
