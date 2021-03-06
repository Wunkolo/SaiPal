#include "Capture.h"
#include "../SaiPal.h"
#include <Windows.h>
#include <iostream>

Capture::Capture() : Running(false), Timer(0.0), Delay(FLT_MAX)
{
	//Create Captures Directory
	::CreateDirectoryA((SaiPal::Instance().GetDirectory() + "Captures").c_str(), NULL);
}

Capture::~Capture()
{
}

std::string Capture::Info() const
{
	return
		"Capture Directory: " + (SaiPal::Instance().GetDirectory() + "Captures\n") +
		"'capture' captures a single frame of the currently open document.\n"
		"'capture start [interval]' captures a frame every [interval] seconds.\n"
		"'capture stop' stops the current capture.\n"
		"'capture [pause|resume]' pause or resume a currently running capture.\n"
		"\tex: 'capture start 1.5' captures a frame every 1.5 seconds.\n"
		"\tex: 'capture start 0.5' captures a frame every half-second\n";
}

void Capture::Tick(const std::chrono::duration<double>& Delta)
{
	if( Running && Active )
	{
		//Capture every time delta adds up to the delay time
		Timer += Delta.count();
		if( Timer >= Delay )
		{
			Timer = 0.0;
			CaptureCanvas();
		}
	}
}

void Capture::Run(const std::vector<std::string>& Args)
{
	if( SaiPal::Instance().GetSession().ActiveCanvas()[0] )
	{
		if( Args.size() == 1 )
		{
			CaptureCanvas();
			std::cout << "Canvas Captured " << std::endl;
		}
		if( Args.size() >= 2 )
		{
			if( !Args[1].compare("stop") )
			{
				if( Running )
				{
					std::cout << "Stopping capture" << std::endl;
					Timer = 0.0;
					Delay = FLT_MAX;
					Running = false;
					Active = false;
				}
				else
				{
					std::cout << "No capture in progress" << std::endl;
				}
			}
			else if( !Args[1].compare("start") )
			{
				if( Args.size() >= 3 && std::atof(Args[2].c_str()) > 0.0 )
				{
					std::cout << "Starting capture every "
						<< std::atof(Args[2].c_str()) << " second(s)." << std::endl;
					Running = true;
					Active = true;
					Timer = 0.0;
					Delay = std::atof(Args[2].c_str());
				}
				else
				{
					do
					{
						std::cout << "Enter capture delay in seconds: ";
						std::cin >> Delay;
						if( Delay <= 0 || std::cin.fail() )
						{
							std::cout << "Invalid delay time" << std::endl;
							std::cin.clear();
							std::cin.ignore();
						}
					} while( Delay <= 0 || std::cin.fail() );
					std::cout << "Starting capture every "
						<< Delay << " seconds(s)" << std::endl;
					Running = true;
					Timer = 0.0;
				}
			}
			else if( !Args[1].compare("pause") )
			{
				if( Running && Active )
				{
					Active = false;
					std::cout << "Capture paused(type 'capture resume' to continue)"
						<< std::endl;
				}
				else
				{
					std::cout << "No capture in progress" << std::endl;
				}
			}
			else if( !Args[1].compare("resume") )
			{
				if( Running && !Active )
				{
					Active = true;
					std::cout << "Capture resumed" << std::endl;
				}
				else
				{
					std::cout << "No capture in progress" << std::endl;
				}
			}
			else
			{
				std::cout << "Unknown parameter: " << Args[1] << std::endl;
			}
		}
	}
	else
	{
		std::cout << "No active canvas" << std::endl;
	}
}

void Capture::CaptureCanvas()
{
	if( SaiPal::Instance().GetSession().ActiveCanvas()[0] )
	{
		//Capture single canvas
		SaiCanvas Canvas(
			SaiPal::Instance().GetSession().ActiveCanvas()[0]);
		std::string Path = Canvas.GetName();
		//Remove .sai extension
		Path = Path.substr(
			Path.find_last_of('\\') == std::string::npos ? 0 : Path.find_last_of('\\') + 1,
			std::string::npos);
		Path.erase(Path.begin() + Path.find_last_of('.'), Path.end());

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		Path += " [" + std::to_string(time.QuadPart) + "]";
		Path += ".png";
		Path = SaiPal::Instance().GetDirectory() + "Captures\\" + Path;
		Canvas.CaptureImage(Path);
	}
	else
	{
		std::cout << "No active canvas" << std::endl;
	}
}