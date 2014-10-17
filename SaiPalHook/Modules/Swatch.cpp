#include "Swatch.h"
#include "../SaiPal.h"

Swatch::Swatch()
{
}

Swatch::~Swatch()
{
}

std::string Swatch::Info() const
{
	return "";
}

void Swatch::Tick(const std::chrono::duration<double>& Delta)
{
}

void Swatch::Run(const std::vector<std::string>& Args)
{
	std::cout << std::hex
		<< SaiPal::Instance().GetSession().GetSwatches().GetSwatchRGBA(0, 0)
		<< std::endl;
	SaiPal::Instance().GetSession().GetSwatches().ReadSwatch("Swatch.png");
}