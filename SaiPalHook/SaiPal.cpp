#include "SaiPal.h"

#include <Windows.h>
#include <conio.h> // _getch()
#include <iomanip>
#include <cctype> //isprint

#define ps1 ">["

SaiPal::SaiPal()
{
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							  FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	std::cout<<"SaiPal thread created"<<std::endl;
	std::cout<<"SaiHook.dll - DEElekgolo(DEElekgolo@gmail.com)\n\tCompile date: "<<__DATE__<<" : "<<__TIME__<<std::endl;
	std::cout<<"This DLL is intended to be injected into version ";
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
	std::cout<<VersionStr;
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	std::cout<<" of PaintTool Sai"<<std::endl;

	unsigned int ConsoleWidth = 80;
	CONSOLE_SCREEN_BUFFER_INFO ConsoleBuf;
	if( GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleBuf) )
	{
		ConsoleWidth = ConsoleBuf.dwSize.X;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							FOREGROUND_RED|
							FOREGROUND_BLUE);
	std::cout<<std::string(ConsoleWidth-1, '=')<<std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							FOREGROUND_RED|
							FOREGROUND_GREEN|
							FOREGROUND_INTENSITY);

	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							  FOREGROUND_RED|
							  FOREGROUND_GREEN|
							  FOREGROUND_INTENSITY);
	std::cout<<ps1;
	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							  FOREGROUND_BLUE|
							  FOREGROUND_GREEN|
							  FOREGROUND_INTENSITY);

	::CreateDirectoryA("SaiPal", NULL);

	//place holder
	Commands.insert("history");
	Commands.insert("canvas");
	Commands.insert("capture");
	Commands.insert("color");
}

SaiPal::~SaiPal()
{
}

void SaiPal::Tick(const std::chrono::duration<double> Delta)
{
	PrintConsole();
	static double Timer = 0;
	Timer += Delta.count();
	if( Timer>=(1/24.0) )//24 Hz
	{
		//Random colors: 24 Hz
		//place holder
		//Session.SetPrimaryColor(rand(), rand(), rand());
		Session.SetSecondaryColor(rand(), rand(), rand());
		Timer = 0;
	}
}

void SaiPal::PrintConsole()
{
	if( _kbhit() )
	{
		int Key = _getch();
		//std::cout << std::endl << std::hex << Key << std::endl;
		if( std::isprint(Key) )
		{
			Command.push_back(Key);
			::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
									  FOREGROUND_BLUE|
									  FOREGROUND_GREEN|
									  FOREGROUND_INTENSITY);
			std::cout<<(char)Key;

			//Clear previous suggestion
			for( unsigned int i = Command.length();
				i<PrevSuggestion.length();
				i++ )
			{
				std::cout<<" ";
			}
			for( unsigned int i = Command.length();
				i<PrevSuggestion.length();
				i++ )
			{
				std::cout<<"\b";
			}
			PrevSuggestion.clear();

			std::vector<std::string> Suggest;
			for( std::set<std::string>::iterator it = Commands.begin(); it!=Commands.end(); ++it )
			{
				if( !Command.compare(0, Command.length(), (*it), 0, Command.length()) )//includes command
				{
					Suggest.push_back((*it));
				}
			}
			if( Suggest.size()==1 )
			{
				PrevSuggestion = Suggest[0];
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_BLUE|
										  FOREGROUND_GREEN);
				std::cout<<Suggest[0].substr(Command.length(), std::string::npos);

				//move back to end of input char
				for( unsigned int i = Command.length();
					i<Suggest[0].length();
					i++ )
				{
					std::cout<<"\b";
				}
			}
			Suggest.clear();
		}
		else if( Key=='\b' )
		{
			if( !Command.empty() )
			{
				Command.pop_back();
				std::cout<<"\b \b";
			}
			if( !PrevSuggestion.empty() )
			{
				//Clear previous suggestion
				for( unsigned int i = Command.length();
					i<PrevSuggestion.length();
					i++ )
				{
					std::cout<<" ";
				}
				for( unsigned int i = Command.length();
					i<PrevSuggestion.length();
					i++ )
				{
					std::cout<<"\b";
				}
				PrevSuggestion.clear();
			}
		}
		else if( Key=='\r' )
		{
			//enter
			if( !Command.empty() )
			{
				//Clear previous suggestion
				for( unsigned int i = Command.length();
					i<PrevSuggestion.length();
					i++ )
				{
					std::cout<<" ";
				}
				for( unsigned int i = Command.length();
					i<PrevSuggestion.length();
					i++ )
				{
					std::cout<<"\b";
				}
				PrevSuggestion.clear();

				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_RED|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				std::cout<<'\r'<<ps1;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_BLUE|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				std::cout<<Command;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_RED|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				std::cout<<']'<<std::endl;
				//process command and execute it
				//if (Commands.find(Command) != Commands.end())
				//{
				//	::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				//		FOREGROUND_GREEN |
				//		FOREGROUND_INTENSITY);
				//	std::cout << "Command executed" << std::endl;
				//}
				//else
				if( !Command.compare("canvas") )
				{
					if( Session.ActiveCanvas()[0] )
					{
						SaiCanvas Canvas(Session.ActiveCanvas()[0]);
						std::cout<<Canvas.GetName()<<" : "<<Canvas.Width()<<"x"<<Canvas.Height()<<std::endl;
					}
					else
					{
						std::cout<<"No active canvas"<<std::endl;
					}
				}
				else if( !Command.compare("capture") )
				{
					if( Session.ActiveCanvas()[0] )
					{
						SaiCanvas Canvas(Session.ActiveCanvas()[0]);
						std::string Path = Canvas.GetName();
						Path = Path.substr(
							Path.find_last_of('\\')==std::string::npos ? 0 : Path.find_last_of('\\')+1,
							std::string::npos);
						Path.erase(Path.begin()+Path.find_last_of('.'), Path.end());
						::CreateDirectory(std::string("SaiPal\\"+Path).c_str(), 0);
						Path = "SaiPal\\"+Path+"\\"+Path;

						LARGE_INTEGER time;
						QueryPerformanceCounter(&time);

						Path += " ["+std::to_string(time.QuadPart)+"]";
						Path += ".png";

						std::cout<<"Capturing canvas to: "<<Path<<" ["<<(Canvas.CaptureImage(Path) ? "Success" : "Failed")<<std::endl;
					}
					else
					{
						std::cout<<"No active canvas"<<std::endl;
					}
				}
				else if( !Command.compare("new") )
				{
					std::cout<<"New canvas: "<<(void*)Session.NewCanvas("Honk")<<std::endl;
				}
				else if( !Command.compare("color") )
				{
					Color curColor = Session.GetPrimaryColor();
					std::cout<<"Current Color: "<<
						(curColor.R>>8)<<", "<<
						(curColor.G>>8)<<", "<<
						(curColor.B>>8)<<std::endl;
				}
				else if( !Command.compare("history") )
				{
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED|
											  FOREGROUND_GREEN);
					std::cout<<"Previous commands:"<<std::endl;
					std::vector<std::string>::iterator it;
					for( it = PrevCommands.begin();
						it!=PrevCommands.end();
						++it )
					{
						std::cout<<" -"<<*it<<std::endl;
					}
				}
				PrevCommands.push_back(Command);
				PrevCommand = PrevCommands.end();
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_RED|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				std::cout<<ps1;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_BLUE|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				Command.clear();
			}
		}
		else if( Key=='\t' )
		{
			//auto complete
			if( !Command.empty() )
			{
				//Clear previous suggestion
				for( unsigned int i = Command.length();
					i<PrevSuggestion.length();
					i++ )
				{
					std::cout<<" ";
				}
				for( unsigned int i = Command.length();
					i<PrevSuggestion.length();
					i++ )
				{
					std::cout<<"\b";
				}
				PrevSuggestion.clear();

				//Populate possible matches
				std::vector<std::string> Matches;
				for( std::set<std::string>::iterator it = Commands.begin(); it!=Commands.end(); ++it )
				{
					if( !Command.compare(0, Command.length(), (*it), 0, Command.length()) )//includes command
					{
						Matches.push_back((*it));
					}
				}
				//Calculate common prefix if there are matches
				if( Matches.size()>1 )
				{
					std::cout<<std::endl;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED|
											  FOREGROUND_GREEN);
					//Print possible matches
					for( std::vector<std::string>::iterator it = Matches.begin();
						it!=Matches.end();
						it++ )
					{
						std::cout<<" -"<<(*it)<<std::endl;
					}
					std::string Prefix = Matches[0];
					for( std::vector<std::string>::iterator it = Matches.begin()+1;
						it!=Matches.end();
						it++ )
					{
						std::string Temp;
						for( unsigned int i = 0; i<Prefix.length(); i++ )
						{
							if( i>=(*it).length() )
							{
								break;
							}
							if( Prefix[i]!=(*it)[i] )
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
											  FOREGROUND_RED|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_BLUE|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<Command;
				}
				else if( Matches.size()==1&&Matches[0].compare(Command)!=0 )
				{
					Command = Matches[0];
					for( unsigned int i = 0;
						i<Command.length();
						i++ )
					{
						std::cout<<"\b \b";
					}
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<'\r'<<ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_BLUE|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<Command;
				}
				Matches.clear();
			}
			else
			{
				std::cout<<std::endl;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_RED|
										  FOREGROUND_GREEN);
				std::cout<<"Available commands:"<<std::endl;
				std::set<std::string>::iterator it;
				for( it = Commands.begin(); it!=Commands.end(); ++it )
				{
					std::cout<<" -"<<*it<<std::endl;
				}
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_RED|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				std::cout<<ps1;
				::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										  FOREGROUND_BLUE|
										  FOREGROUND_GREEN|
										  FOREGROUND_INTENSITY);
				std::cout<<Command;
			}
		}
		else if( Key==0||Key==0xe0 )//escape character
		{
			int Func = _getch();
			if( Func==0x48 )
			{
				//up
				if( !PrevCommands.empty() )
				{
					//Clear previous suggestion
					for( unsigned int i = Command.length();
						i<PrevSuggestion.length();
						i++ )
					{
						std::cout<<" ";
					}
					for( unsigned int i = Command.length();
						i<PrevSuggestion.length();
						i++ )
					{
						std::cout<<"\b";
					}
					PrevSuggestion.clear();
					//backspace
					for( unsigned int i = 0;
						i<Command.length();
						i++ )
					{
						std::cout<<"\b \b";
					}
					//use previous command
					if( PrevCommand!=PrevCommands.begin() )
					{
						PrevCommand--;
					}
					Command = *PrevCommand;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<'\r'<<ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_BLUE|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<Command;
				}
			}
			else if( Func==0x50 )
			{
				//down
				if( !PrevCommands.empty() )
				{
					//Clear previous suggestion
					for( unsigned int i = Command.length();
						i<PrevSuggestion.length();
						i++ )
					{
						std::cout<<" ";
					}
					for( unsigned int i = Command.length();
						i<PrevSuggestion.length();
						i++ )
					{
						std::cout<<"\b";
					}
					PrevSuggestion.clear();
					//backspace
					for( unsigned int i = 0;
						i<Command.length();
						i++ )
					{
						std::cout<<"\b \b";
					}
					PrevSuggestion.clear();
					//use previous command
					if( PrevCommand!=PrevCommands.end() )
					{
						PrevCommand++;
					}
					Command = *PrevCommand;

					//Clear previous suggestion
					for( unsigned int i = Command.length();
						i<PrevSuggestion.length();
						i++ )
					{
						std::cout<<" ";
					}
					for( unsigned int i = Command.length();
						i<PrevSuggestion.length();
						i++ )
					{
						std::cout<<"\b";
					}

					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_RED|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<'\r'<<ps1;
					::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
											  FOREGROUND_BLUE|
											  FOREGROUND_GREEN|
											  FOREGROUND_INTENSITY);
					std::cout<<Command;
				}
			}
			else if( Func = 0x4B )
			{
				//left
			}
			else if( Func==0x4D )
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