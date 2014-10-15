#pragma once
#include <chrono>
#include <vector>
#include <string>

class SaiModule
{
public:
	SaiModule()
	{
	}
	~SaiModule()
	{
	}
	virtual std::string Info() const = 0;
	virtual void Tick(const std::chrono::duration<double>& Delta) = 0;
	virtual void Run(const std::vector<std::string>& Args) = 0;
private:
};