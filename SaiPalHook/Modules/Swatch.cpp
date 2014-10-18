#include "Swatch.h"
#include "../SaiPal.h"

Swatch::Swatch()
{
	//Create Swatches Directory
	::CreateDirectoryA((SaiPal::Instance().GetDirectory() + "Swatches").c_str(), NULL);
}

Swatch::~Swatch()
{
}

std::string Swatch::Info() const
{
	return
		"Swatch Directory: " + (SaiPal::Instance().GetDirectory() + "Swatches\n") +
		"'swatch' shows info of operations you can execute onto your active swatch\n"
		"'swatch save' Saves your current swatch\n"
		"'swatch save [Swatchname] (Scale)' Saves your current swatch with the specified name(as png). Scale value optional\n"
		"'swatch load [Filename]' Loads the specified image into your swatches";
}

void Swatch::Tick(const std::chrono::duration<double>& Delta)
{
}

void Swatch::Run(const std::vector<std::string>& Args)
{
	if( Args.size() == 1 )
	{
		std::cout << Info() << std::endl;
	}
	if( Args.size() >= 2 )
	{
		if( !Args[1].compare("save") )
		{
			std::string Path(SaiPal::Instance().GetDirectory() + "Swatches\\");
			if( Args.size() >= 3 )
			{
				Path += Args[2];
				int Scale = 1;
				if( Args.size() >= 4 )
				{
					Scale = std::atoi(Args[3].c_str());
					if( Scale <= 0 )
					{
						std::cout << "Invalid Scale: " << Scale << std::endl;
						return;
					}
				}
				if( SaiPal::Instance().GetSession().
				   GetSwatch().SaveSwatch(Path, Scale) )
				{
					std::cout << "Swatch saved to: " << Path << std::endl;
					return;
				}
				else
				{
					std::cout << "Error saving swatch to:" << Path << std::endl;
					return;
				}
			}
			else
			{
				LARGE_INTEGER time;
				QueryPerformanceCounter(&time);
				Path += "Swatch [" + std::to_string(time.QuadPart) + "].png";
				std::cout << Path << std::endl;
				if( SaiPal::Instance().GetSession().
				   GetSwatch()
				   .SaveSwatch(Path) )
				{
					std::cout << "Swatch saved to: " << Path << std::endl;
					return;
				}
				else
				{
					std::cout << "Failed to save swatch image" << std::endl;
					return;
				}
			}
		}
		else if( !Args[1].compare("load") )
		{
			if( Args.size() >= 3 )
			{
			}
			else
			{
				std::cout << "No swatch file specified" << std::endl;
				return;
			}
		}
	}
}