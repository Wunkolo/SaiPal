#include "SaiPal.h"

#include <Windows.h>
#include <conio.h> // _getch()
#include <iomanip>
#include <cctype>


SaiPal::SaiPal()
{
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "SaiPal thread created" << std::endl;
	std::cout << "SaiHook.dll - DEElekgolo(DEElekgolo@gmail.com)\n\tCompile date: " << __DATE__ << " : " << __TIME__ << std::endl;
	std::cout << "This DLL is intended to be injected into version ";
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << VersionStr;
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << " of PaintTool Sai" << std::endl;
	::CreateDirectoryA("SaiPal", NULL);

	Commands.insert("history");
	Commands.insert("canvas");
	Commands.insert("capture");
	Commands.insert("clear");
}

SaiPal::~SaiPal()
{

}

void SaiPal::Tick(std::chrono::duration<double> Delta)
{
	PrintConsole();
}

void SaiPal::PrintConsole()
{
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);
	std::cout << std::setw(2) << ">[";
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_BLUE |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);
	std::cout << Command;
	if (_kbhit())
	{
		int Key = _getch();
		//std::cout << std::endl << std::hex << Key << std::endl;
		if (std::isprint(Key))
		{
			Command.append(1, Key);
		}
		else if (Key == '\b')
		{
			if (!Command.empty())
			{
				Command.pop_back();
			}
			std::cout << "\b \b";
		}
		else if (Key == '\r')
		{
			//enter
			if (!Command.empty())
			{
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << "]" << std::endl;
				//process command and execute it
				if (Commands.find(Command) != Commands.end() && false)
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << "command executed" << std::endl;
				}
				else if (!Command.compare("canvas"))
				{
					if (Session.ActiveCanvas()[0])
					{
						Pointer ThisCanvas = Session.ActiveCanvas()[0];
#if defined(SAI120)
							ThisCanvas = ThisCanvas(0x100);
#endif
							std::string CanvasName((char*)ThisCanvas(0x4d8));
#if defined(SAI120)
							ThisCanvas = ThisCanvas(-0x100);
#endif
							unsigned int Width = ThisCanvas(0x124).asUint();
							unsigned int Height = ThisCanvas(0x128).asUint();

							std::cout << CanvasName << " : " << Width << "x" << Height << std::endl;
					}
					else
					{
						std::cout << "No active canvas" << std::endl;
					}
				}
				else if (!Command.compare("capture"))
				{
					if (Session.ActiveCanvas()[0])
					{
						Pointer ThisCanvas = Session.ActiveCanvas()[0];
#if defined(SAI120)
						ThisCanvas = ThisCanvas(0x100);
#endif
						std::string CanvasName((char*)ThisCanvas(0x4d8));
						CanvasName = CanvasName.substr(
							CanvasName.find_last_of('\\') == std::string::npos ? 0 : CanvasName.find_last_of('\\') + 1,
							std::string::npos);
						CanvasName.erase(CanvasName.begin() + CanvasName.find_last_of('.'), CanvasName.end());
						CanvasName = "SaiPal\\" + CanvasName + "\\" + CanvasName;

						SYSTEMTIME st;
						GetSystemTime(&st);

						CanvasName += " (" + std::to_string(std::time(nullptr)) + "_" + std::to_string(st.wMilliseconds) + ")";
						CanvasName += ".png";
						std::cout << CanvasName << std::endl;
					}
					else
					{
						std::cout << "No active canvas" << std::endl;
					}
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					std::cout << "Unknown command: " << Command << std::endl;
				}
				PrevCommands.push_back(Command);
				PrevCommand = PrevCommands.end();
				Command.clear();
			}
		}
		else if (Key == '\t')
		{
			//auto complete
			if (!Command.empty())
			{
				std::cout << "*" << std::endl;
				std::string Match = Command;

				//Populate possible matches
				std::vector<std::string> Matches;
				for (std::set<std::string>::iterator it = Commands.begin(); it != Commands.end(); ++it)
				{
					if (!Command.compare(0, Command.length(), (*it), 0, Command.length()))//includes command
					{
						Matches.push_back((*it));
					}
				}
				//Calculate common prefix if there are matches
				if (Matches.size() > 1)
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_RED |
						FOREGROUND_GREEN);
					//Print possible matches
					for (std::vector<std::string>::iterator it = Matches.begin();
						it != Matches.end();
						it++)
					{
						std::cout << " -" << (*it) << std::endl;
					}
					std::string Prefix = Matches[0];
					for (std::vector<std::string>::iterator it = Matches.begin() + 1;
						it != Matches.end();
						it++)
					{
						std::string Temp;
						for (unsigned int i = 0; i < Prefix.length(); i++)
						{
							if (i >= (*it).length())
							{
								break;
							}
							if (Prefix[i] != (*it)[i])
							{
								break;
							}
							else
							{
								Temp.push_back(Prefix[i]);
							}
						}
						Prefix = Temp;
					}
					Match = Prefix;
				}
				else if (Matches.size() == 1)
				{
					Match = Matches[0];
				}
				Matches.clear();
				//backspace
				for (unsigned int i = 0;
					i < Command.length();
					i++)
				{
					std::cout << "\b \b";
				}
				Command = Match;
			}
			else
			{
				std::cout << std::endl;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED |
					FOREGROUND_GREEN);
				std::cout << "Available commands:" << std::endl;
				std::set<std::string>::iterator it;
				for (it = Commands.begin(); it != Commands.end(); ++it)
				{
					std::cout << " -" << *it << std::endl;
				}
			}
		}
		else if (Key == 0 || Key == 0xe0)//escape character
		{
			int Func = _getch();
			if (Func == 0x48)
			{
				//up
				if (!PrevCommands.empty())
				{
					//backspace
					for (unsigned int i = 0;
						i < Command.length();
						i++)
					{
						std::cout << "\b \b";
					}
					//use previous command
					if (PrevCommand != PrevCommands.begin())
					{
						PrevCommand--;
					}
					Command = *PrevCommand;
				}
			}
			else if (Func == 0x50)
			{
				//down
				if (!PrevCommands.empty())
				{
					//backspace
					for (unsigned int i = 0;
						i < Command.length();
						i++)
					{
						std::cout << "\b \b";
					}
					//use previous command
					if (PrevCommand != PrevCommands.end())
					{
						PrevCommand++;
					}
					Command = *PrevCommand;
				}
			}
			else if (Func = 0x4B)
			{
				//left
			}
			else if (Func == 0x4D)
			{
				//right
			}
			else
			{
				std::cout << "Unknown function Key" << std::endl;
			}
		}
	}
	std::cout << '\r';
}