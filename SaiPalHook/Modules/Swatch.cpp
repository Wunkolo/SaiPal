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
	return
		"'swatch' shows info of operations you can execute onto your active swatch\n"
		"'swatch save [Scale] [Filename]' Saves your current swatch at the specified image scale\n"
		"\t(leave filename blank for unique generated name\n";
}

void Swatch::Tick(const std::chrono::duration<double>& Delta)
{
}

void Swatch::Run(const std::vector<std::string>& Args)
{
	std::cout << std::hex
		<< SaiPal::Instance().GetSession().GetSwatch().GetSwatchRGBA(0, 0)
		<< std::endl;
	SaiPal::Instance().GetSession().GetSwatch().SaveSwatch("Swatch1.png");
}