#pragma once
#include "SaiModule.h"

class Capture : public SaiModule
{
public:
	Capture();
	~Capture();
	std::string Info() const;
	void Tick(const std::chrono::duration<double>& Delta);
	void Run(const std::vector<std::string>& Args);
private:
	void CaptureCanvas();
	bool Running, Active;
	double Delay;
	double Timer;
};
