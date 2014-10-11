#include "SaiPal.h"

#include <Windows.h>
#include <conio.h> // _getch()
#include <iomanip>
#include <cctype> //isprint

#define ps1 ">["

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

	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);
	std::cout << ps1;
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_BLUE |
		FOREGROUND_GREEN |
		FOREGROUND_INTENSITY);


	::CreateDirectoryA("SaiPal", NULL);

	//place holder
	Commands.insert("history");
	Commands.insert("canvas");
	Commands.insert("capture");
	Commands.insert("color");
	Commands.insert("clean");
	Commands.insert("call");
	Commands.insert("cook");
	Commands.insert("install");
	Commands.insert("obama");
	Commands.insert("shellshock");
	Commands.insert("apple");

}

SaiPal::~SaiPal()
{

}

void SaiPal::Tick(std::chrono::duration<double> Delta)
{
	PrintConsole();
	static double Timer = 0;
	Timer += Delta.count();
	if (Timer >= (1 / 24.0))//24 Hz
	{
		Session.SetPrimaryColor(rand(), rand(), rand());
		Timer = 0;
	}
}

void SaiPal::PrintConsole()
{
	if (_kbhit())
	{
		int Key = _getch();
		//std::cout << std::endl << std::hex << Key << std::endl;
		if (std::isprint(Key))
		{
			Command.push_back(Key);
			::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_BLUE |
				FOREGROUND_GREEN |
				FOREGROUND_INTENSITY);
			std::cout << (char)Key;

			//Clear previous suggestion
			for (unsigned int i = Command.length();
				i < PrevSuggestion.length();
				i++)
			{
				std::cout << " ";
			}
			for (unsigned int i = Command.length();
				i < PrevSuggestion.length();
				i++)
			{
				std::cout << "\b";
			}
			PrevSuggestion.clear();

			std::vector<std::string> Suggest;
			for (std::set<std::string>::iterator it = Commands.begin(); it != Commands.end(); ++it)
			{
				if (!Command.compare(0, Command.length(), (*it), 0, Command.length()))//includes command
				{
					Suggest.push_back((*it));
				}
			}
			if (Suggest.size() == 1)
			{
				PrevSuggestion = Suggest[0];
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_BLUE |
					FOREGROUND_GREEN);
				std::cout << Suggest[0].substr(Command.length(),std::string::npos);

				//move back to end of input char
				for (unsigned int i = Command.length();
					i < Suggest[0].length();
					i++)
				{
					std::cout << "\b";
				}
			}
			Suggest.clear();

		}
		else if (Key == '\b')
		{
			if (!Command.empty())
			{
				Command.pop_back();
				std::cout << "\b \b";
			}
			if (!PrevSuggestion.empty())
			{
				//Clear previous suggestion
				for (unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++)
				{
					std::cout << " ";
				}
				for (unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++)
				{
					std::cout << "\b";
				}
				PrevSuggestion.clear();
			}
		}
		else if (Key == '\r')
		{
			//enter
			if (!Command.empty())
			{
				//Clear previous suggestion
				for (unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++)
				{
					std::cout << " ";
				}
				for (unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++)
				{
					std::cout << "\b";
				}
				PrevSuggestion.clear();

				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << '\r' << ps1;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << Command;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << ']' << std::endl;
				//process command and execute it
				//if (Commands.find(Command) != Commands.end())
				//{
				//	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				//		FOREGROUND_GREEN |
				//		FOREGROUND_INTENSITY);
				//	std::cout << "Command executed" << std::endl;
				//}
				//else 
				if (!Command.compare("canvas"))
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
				else if (!Command.compare("color"))
				{
					Color curColor = Session.GetPrimaryColor();
					std::cout << "Current Color: " <<
						(curColor.R >> 8) << ", " <<
						(curColor.G >> 8) << ", " <<
						(curColor.B >> 8) << std::endl;
				}
				else if (!Command.compare("history"))
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_RED |
						FOREGROUND_GREEN);
					std::cout << "Previous commands:" << std::endl;
					std::vector<std::string>::iterator it;
					for (it = PrevCommands.begin();
						it != PrevCommands.end();
						++it)
					{
						std::cout << " -" << *it << std::endl;
					}
				}
				PrevCommands.push_back(Command);
				PrevCommand = PrevCommands.end();
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << ps1;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				Command.clear();
			}
		}
		else if (Key == '\t')
		{
			//auto complete
			if (!Command.empty())
			{
				//Clear previous suggestion
				for (unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++)
				{
					std::cout << " ";
				}
				for (unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++)
				{
					std::cout << "\b";
				}
				PrevSuggestion.clear();

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
					std::cout << std::endl;
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
					Command = Prefix;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_RED |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_BLUE |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << Command;
				}
				else if (Matches.size() == 1 && Matches[0].compare(Command) != 0)
				{
					Command = Matches[0];
					for (unsigned int i = 0;
						i < Command.length();
						i++)
					{
						std::cout << "\b \b";
					}
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_RED |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << '\r' << ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_BLUE |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << Command;
				}
				Matches.clear();
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
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_RED |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << ps1;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_INTENSITY);
				std::cout << Command;
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
					//Clear previous suggestion
					for (unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++)
					{
						std::cout << " ";
					}
					for (unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++)
					{
						std::cout << "\b";
					}
					PrevSuggestion.clear();
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
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_RED |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << '\r' << ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_BLUE |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << Command;
				}
			}
			else if (Func == 0x50)
			{
				//down
				if (!PrevCommands.empty())
				{
					//Clear previous suggestion
					for (unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++)
					{
						std::cout << " ";
					}
					for (unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++)
					{
						std::cout << "\b";
					}
					PrevSuggestion.clear();
					//backspace
					for (unsigned int i = 0;
						i < Command.length();
						i++)
					{
						std::cout << "\b \b";
					}
					PrevSuggestion.clear();
					//use previous command
					if (PrevCommand != PrevCommands.end())
					{
						PrevCommand++;
					}
					Command = *PrevCommand;

					//Clear previous suggestion
					for (unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++)
					{
						std::cout << " ";
					}
					for (unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++)
					{
						std::cout << "\b";
					}


					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_RED |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << '\r' << ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_BLUE |
						FOREGROUND_GREEN |
						FOREGROUND_INTENSITY);
					std::cout << Command;
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
		std::cout.flush();
	}
	//std::cout << '\r';
}