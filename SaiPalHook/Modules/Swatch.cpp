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
		"'swatch' shows available swatch operations and available swatches\n"
		"'swatch save [Swatch Name] (Scale)' saves your current swatch with the\n"
		"\tspecified name. Scale value optional(defaults to 1)\n"
		"'swatch load [Swatch Name]' loads the specified swatch\n"
		"\tex: 'swatch save CoolSwatch' will save to \"CoolSwatch.png\"\n"
		"\tex: 'swatch save CoolSwatch 2' will save an x2 larger image of it.\n"
		"\tex: 'swatch load CoolerSwatch' will load \"CoolerSwatch.png\"\n";
}

void Swatch::Tick(const std::chrono::duration<double>& Delta)
{
}

void Swatch::Run(const std::vector<std::string>& Args)
{
	if( Args.size() == 1 )
	{
		std::cout << Info();
		std::cout << "Available Swatches: " << std::endl;
		WIN32_FIND_DATA Finder;
		HANDLE hFind = FindFirstFile(
			(SaiPal::Instance().GetDirectory() + "Swatches\\*.png").c_str(),
			&Finder);
		if( hFind == INVALID_HANDLE_VALUE )
		{
			std::cout << "No swatch files found" << std::endl;
			return;
		}
		do
		{
			if( !(Finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				std::string SwatchName(Finder.cFileName);
				//remove extension
				SwatchName = SwatchName.substr(
					0,
					SwatchName.find_last_of('.'));
				std::cout << "-" << SwatchName << std::endl;
			}
		} while( FindNextFile(hFind, &Finder) != 0 );
	}
	if( Args.size() >= 2 )
	{
		if( !Args[1].compare("save") )
		{
			std::string Path(SaiPal::Instance().GetDirectory() + "Swatches\\");
			if( Args.size() >= 3 )
			{
				Path += Args[2];
				Path += ".png";
				int Scale = 1;
				if( Args.size() >= 4 )
				{
					Scale = std::atoi(Args[3].c_str());
					if( Scale <= 0 )
					{
						std::cout << "Invalid Scale: " << Args[3] << std::endl;
						return;
					}
					if( Scale > 100 )
					{
						// should be a good enough cut-off.
						std::cout << "Scale too large!" << std::endl;
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
				std::cout << "No swatch name specified" << std::endl;
				//LARGE_INTEGER time;
				//QueryPerformanceCounter(&time);
				//Path += "Swatch [" + std::to_string(time.QuadPart) + "].png";
				//if( SaiPal::Instance().GetSession().
				//   GetSwatch()
				//   .SaveSwatch(Path) )
				//{
				//	std::cout << "Swatch saved to: " << Path << std::endl;
				//	return;
				//}
				//else
				//{
				//	std::cout << "Failed to save swatch image" << std::endl;
				//	return;
				//}
			}
		}
		else if( !Args[1].compare("load") )
		{
			if( Args.size() >= 3 )
			{
				unsigned int Attrib = GetFileAttributes(
					(SaiPal::Instance().GetDirectory()
					+ "Swatches\\"
					+ Args[2] + ".png").c_str());
				//Check if file exists.
				if( Attrib == INVALID_FILE_ATTRIBUTES ||
				   Attrib == FILE_ATTRIBUTE_DIRECTORY )
				{
					std::cout
						<< "Swatch " << Args[2] << " not found"
						<< std::endl;
					std::cout << "Available Swatches: " << std::endl;
					WIN32_FIND_DATA Finder;
					HANDLE hFind = FindFirstFile(
						(SaiPal::Instance().GetDirectory() + "Swatches\\*.png").c_str(),
						&Finder);
					if( hFind == INVALID_HANDLE_VALUE )
					{
						std::cout << "No swatch files found" << std::endl;
						return;
					}
					do
					{
						if( !(Finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
						{
							std::string SwatchName(Finder.cFileName);
							//remove extension
							SwatchName = SwatchName.substr(
								0,
								SwatchName.find_last_of('.'));
							std::cout << "-" << SwatchName << std::endl;
						}
					} while( FindNextFile(hFind, &Finder) != 0 );
					return;
				}
				if( SaiPal::Instance().GetSession()
				   .GetSwatch().LoadSwatch(SaiPal::Instance().GetDirectory()
				   + "Swatches\\"
				   + Args[2] + ".png") )
				{
					std::cout << "Swatch \"" << Args[2] << "\" loaded" << std::endl;
				}
				else
				{
					std::cout << "Failed to load swatch: " << Args[2] << std::endl;
				}
			}
			else
			{
				std::cout << "No swatch name specified" << std::endl;
				std::cout << "Available Swatches: " << std::endl;
				WIN32_FIND_DATA Finder;
				HANDLE hFind = FindFirstFile(
					(SaiPal::Instance().GetDirectory() + "Swatches\\*.png").c_str(),
					&Finder);
				if( hFind == INVALID_HANDLE_VALUE )
				{
					std::cout << "No swatch files found" << std::endl;
					return;
				}
				do
				{
					if( !(Finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
					{
						std::string SwatchName(Finder.cFileName);
						//remove extension
						SwatchName = SwatchName.substr(
							0,
							SwatchName.find_last_of('.'));
						std::cout << "-" << SwatchName << std::endl;
					}
				} while( FindNextFile(hFind, &Finder) != 0 );
				return;
			}
		}
	}
}