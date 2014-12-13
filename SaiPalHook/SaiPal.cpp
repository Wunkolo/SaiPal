#include "SaiPal.h"

#include <iomanip>
#include <sstream>
#include <vector>

#include <Windows.h>
#include <conio.h> // _getch()
#include <cctype> //isprint

#include "SaiModules.h"

#define ps1 "¯["

SaiPal::SaiPal()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleOutputCP(437);

	unsigned int ConsoleWidth = 80;
	CONSOLE_SCREEN_BUFFER_INFO ConsoleBuf;
	if( GetConsoleScreenBufferInfo(hStdout, &ConsoleBuf) )
	{
		ConsoleWidth = ConsoleBuf.dwSize.X;
	}
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_GREEN |
							FOREGROUND_INTENSITY);
	std::cout << "SaiPalÄÄÂBuild date (" << __DATE__ << " : " << __TIME__ << ")" << std::endl;
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_BLUE |
							FOREGROUND_GREEN |
							FOREGROUND_INTENSITY);
	std::cout << "\tÀDEElekgolo (DEElekgolo@gmail.com)\n";
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_BLUE);
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "This DLL is intended to be injected into version ";
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << VersionStr;
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << " of PaintTool Sai" << std::endl;
	std::cout << "Enter \"";
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_BLUE |
							  FOREGROUND_GREEN |
							  FOREGROUND_INTENSITY);
	std::cout << "help";
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << "\" to get started!" << std::endl;
	std::cout << "Current directory: ";
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_BLUE |
							  FOREGROUND_GREEN |
							  FOREGROUND_INTENSITY);
	std::cout << GetDirectory() << std::endl;
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_BLUE);
	std::cout << std::string(ConsoleWidth - 1, 'Ä') << std::endl;
	SetConsoleTextAttribute(hStdout,
							FOREGROUND_RED |
							FOREGROUND_GREEN |
							FOREGROUND_INTENSITY);

	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_RED |
							  FOREGROUND_GREEN |
							  FOREGROUND_INTENSITY);
	std::cout << ps1;
	::SetConsoleTextAttribute(hStdout,
							  FOREGROUND_BLUE |
							  FOREGROUND_GREEN |
							  FOREGROUND_INTENSITY);

	::CreateDirectoryA(GetDirectory().c_str(), NULL);

	//Command list
	Commands["help"] = nullptr;
	Commands["history"] = nullptr;
	Commands["capture"] = new Capture();
	Commands["color"] = new Color();
	Commands["swatch"] = new Swatch();
	Commands["test"] = new Test();

	Session.SetStatusText("SaiPal loaded!");
}

SaiPal::~SaiPal()
{
	for( std::map<std::string, SaiModule*>::iterator it = Commands.begin(); it != Commands.end(); ++it )
	{
		if( it->second )
		{
			delete it->second;
		}
	}
}

namespace
{
	static void HandleFinder()
	{
	};
}
std::string SaiPal::GetDirectory()
{
	char Path[MAX_PATH];
	HMODULE hMod;
	if( !GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&::HandleFinder,
		&hMod) )
	{
		int Error = GetLastError();
		std::cout << "Unable to resolve current directory" << std::endl;
	}
	GetModuleFileNameA(hMod, Path, sizeof(Path));
	std::string SaiPalDir(Path);
	SaiPalDir = SaiPalDir.substr(0, std::string(SaiPalDir).find_last_of('\\') + 1);
	SaiPalDir += "SaiPal\\";
	return SaiPalDir;;
}

void SaiPal::Tick(const std::chrono::duration<double> Delta)
{
	static double Timer(0);
	static std::string Strings[] = {
		"Cool!",
		"Nice Drawing!",
		"Wow!",
		"Hey you're getting better!",
		"Nice.",
		"Woah dude!",
		"Awesome!",
		"Keep at it!",
		"Amazing!!!",
		"I should commission you!",
		"Dang!!" };
	Timer += Delta.count();
	if( Timer >= 0.25 )
	{
		Timer = 0;
		Session.SetStatusText(Strings[rand() % 11]);
	}
	PrintConsole();
	for( std::map<std::string, SaiModule*>::iterator it = Commands.begin(); it != Commands.end(); ++it )
	{
		if( it->second )
		{
			it->second->Tick(Delta);
		}
	}
}

void SaiPal::PrintConsole()
{
	if( _kbhit() )
	{
		int Key = _getch();
		if( std::isprint(Key) )
		{
			Command.push_back(Key);
			::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
									  FOREGROUND_BLUE |
									  FOREGROUND_GREEN |
									  FOREGROUND_INTENSITY);
			std::cout << (char)Key;

			//Clear previous suggestion
			for( unsigned int i = Command.length();
				i < PrevSuggestion.length();
				i++ )
			{
				std::cout << " ";
			}
			for( unsigned int i = Command.length();
				i < PrevSuggestion.length();
				i++ )
			{
				std::cout << "\b";
			}
			PrevSuggestion.clear();

			std::vector<std::string> Suggest;
			for( std::map<std::string, SaiModule*> ::iterator it = Commands.begin(); it != Commands.end(); ++it )
			{
				if( !Command.compare(0, Command.length(), (it->first), 0, Command.length()) )//includes command
				{
					Suggest.push_back((it->first));
				}
			}
			if( Suggest.size() == 1 )
			{
				PrevSuggestion = Suggest[0];
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_BLUE |
										  FOREGROUND_GREEN);
				std::cout << Suggest[0].substr(Command.length(), std::string::npos);

				//move back to end of input char
				for( unsigned int i = Command.length();
					i < Suggest[0].length();
					i++ )
				{
					std::cout << "\b";
				}
			}
			Suggest.clear();
		}
		else if( Key == '\b' )
		{
			if( !Command.empty() )
			{
				Command.pop_back();
				std::cout << "\b \b";
			}
			if( !PrevSuggestion.empty() )
			{
				//Clear previous suggestion
				for( unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++ )
				{
					std::cout << " ";
				}
				for( unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++ )
				{
					std::cout << "\b";
				}
				PrevSuggestion.clear();
			}
		}
		else if( Key == '\r' )
		{
			//enter
			if( !Command.empty() )
			{
				//Split up string into arguments
				std::stringstream ss(Command);
				std::istream_iterator<std::string> begin(ss);
				std::istream_iterator<std::string> end;
				std::vector<std::string> Args(begin, end);
				//Clear previous suggestion
				for( unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++ )
				{
					std::cout << " ";
				}
				for( unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++ )
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
				std::cout << "]®" << std::endl;
				//process command and execute it
				if( Commands.count(Args[0]) == 1 &&
				   Commands[Args[0]] != nullptr )
				{
					Commands[Args[0]]->Run(Args);
				}
				else if( !Args[0].compare("help") )
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED |
											  FOREGROUND_GREEN);
					if( Args.size() == 2 )
					{
						if( Commands.count(Args[1]) == 1 &&
						   Commands[Args[1]] != nullptr )
						{
							std::cout << std::setfill('Ä');
							std::cout.width(48);
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED |
													  FOREGROUND_GREEN |
													  FOREGROUND_INTENSITY);
							std::cout << std::left
								<< ((char)std::toupper(Args[1][0]) + Args[1].substr(1))
								<< std::endl;
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED |
													  FOREGROUND_GREEN);
							std::cout << Commands[Args[1]]->Info() << std::endl;
						}
						else if( !Args[1].compare("help") )
						{
							std::cout << std::setfill('Ä');
							std::cout.width(48);
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED |
													  FOREGROUND_GREEN |
													  FOREGROUND_INTENSITY);
							std::cout << std::left
								<< "Help"
								<< std::endl;
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED |
													  FOREGROUND_GREEN);
							std::cout <<
								"Displays help for commands\n"
								"help [command] to get information related to a command\n"
								<< std::endl;
						}
						else if( !Args[1].compare("history") )
						{
							std::cout << std::setfill('Ä');
							std::cout.width(48);
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED |
													  FOREGROUND_GREEN |
													  FOREGROUND_INTENSITY);
							std::cout << std::left
								<< "history"
								<< std::endl;
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED |
													  FOREGROUND_GREEN);
							std::cout <<
								"Displays previously ran commands\n"
								"Press up or down in the console to cycle through previously ran commands\n"
								<< std::endl;
						}
						else
						{
							::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
													  FOREGROUND_RED);
							std::cout << "Unknown Command" << Args[1] << std::endl;
						}
					}
					else
					{
						std::cout << std::setfill('Ä');
						std::map<std::string, SaiModule*>::iterator it;
						for( it = Commands.begin(); it != Commands.end(); ++it )
						{
							if( it->second != nullptr )
							{
								std::cout.width(48);
								::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
														  FOREGROUND_RED |
														  FOREGROUND_GREEN |
														  FOREGROUND_INTENSITY);
								std::cout << std::left
									<< ((char)std::toupper(it->first[0]) + it->first.substr(1))
									<< std::endl;
								::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
														  FOREGROUND_RED |
														  FOREGROUND_GREEN);
								std::cout << it->second->Info() << std::endl;
							}
						}
					}
				}
				else if( !Args[0].compare("history") )
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED |
											  FOREGROUND_GREEN);
					std::cout << "Previous commands:" << std::endl;
					std::vector<std::string>::iterator it;
					for( it = PrevCommands.begin();
						it != PrevCommands.end();
						++it )
					{
						std::cout << " -" << *it << std::endl;
					}
				}
				else
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED);
					std::cout << "Unknown Command" << std::endl;
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
		else if( Key == '\t' )
		{
			//auto complete
			if( !Command.empty() )
			{
				//Clear previous suggestion
				for( unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++ )
				{
					std::cout << " ";
				}
				for( unsigned int i = Command.length();
					i < PrevSuggestion.length();
					i++ )
				{
					std::cout << "\b";
				}
				PrevSuggestion.clear();

				//Populate possible matches
				std::vector<std::string> Matches;
				for( std::map<std::string, SaiModule*>::iterator it = Commands.begin(); it != Commands.end(); ++it )
				{
					if( !Command.compare(0, Command.length(), (it->first), 0, Command.length()) )//includes command
					{
						Matches.push_back((it->first));
					}
				}
				//Calculate common prefix if there are matches
				if( Matches.size() > 1 )
				{
					std::cout << std::endl;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED |
											  FOREGROUND_GREEN);
					//Print possible matches
					for( std::vector<std::string>::iterator it = Matches.begin();
						it != Matches.end();
						it++ )
					{
						std::cout << " Ä" << (*it) << std::endl;
					}
					std::string Prefix = Matches[0];
					for( std::vector<std::string>::iterator it = Matches.begin() + 1;
						it != Matches.end();
						it++ )
					{
						std::string Temp;
						for( unsigned int i = 0; i < Prefix.length(); i++ )
						{
							if( i >= (*it).length() )
							{
								break;
							}
							if( Prefix[i] != (*it)[i] )
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
				else if( Matches.size() == 1 && Matches[0].compare(Command) != 0 )
				{
					Command = Matches[0];
					for( unsigned int i = 0;
						i < Command.length();
						i++ )
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
				std::cout << "Available modules:" << std::endl;
				std::map<std::string, SaiModule*>::iterator it;
				for( it = Commands.begin(); it != Commands.end(); ++it )
				{
					std::cout << " Ä" << it->first << std::endl;
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
		else if( Key == 0 || Key == 0xe0 )//escape character
		{
			int Func = _getch();
			if( Func == 0x48 )
			{
				//up
				if( !PrevCommands.empty() )
				{
					//Clear previous suggestion
					for( unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++ )
					{
						std::cout << " ";
					}
					for( unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++ )
					{
						std::cout << "\b";
					}
					PrevSuggestion.clear();
					//backspace
					for( unsigned int i = 0;
						i < Command.length();
						i++ )
					{
						std::cout << "\b \b";
					}
					//use previous command
					if( PrevCommand != PrevCommands.begin() )
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
			else if( Func == 0x50 )
			{
				//down
				if( !PrevCommands.empty() )
				{
					//Clear previous suggestion
					for( unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++ )
					{
						std::cout << " ";
					}
					for( unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++ )
					{
						std::cout << "\b";
					}
					PrevSuggestion.clear();
					//backspace
					for( unsigned int i = 0;
						i < Command.length();
						i++ )
					{
						std::cout << "\b \b";
					}
					PrevSuggestion.clear();
					//use previous command
					if( PrevCommand != PrevCommands.end() )
					{
						PrevCommand++;
					}
					Command = *PrevCommand;

					//Clear previous suggestion
					for( unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++ )
					{
						std::cout << " ";
					}
					for( unsigned int i = Command.length();
						i < PrevSuggestion.length();
						i++ )
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
			else if( Func = 0x4B )
			{
				//left
			}
			else if( Func == 0x4D )
			{
				//right
			}
			else
			{
				//Unknown function key
			}
		}
		std::cout.flush();
	}
}