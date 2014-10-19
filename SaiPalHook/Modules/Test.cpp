#include "Test.h"
#include "../SaiPal.h"
#include <Windows.h>
#include <iostream>

Test::Test()
{
}
Test::~Test()
{
}
std::string Test::Info() const
{
	return "Test module for testing out future modules.\n"
		"You probably should be using this...";
}

void ProcessLayer(Pointer Layer)
{
	printf("honk %x\n", Layer(0x5b).asUshort());
	*(short*)Layer(0x5b) = !Layer(0x5b).asUchar();

	if( Layer(0x5b).asUshort() == 1 )
	{
		*(short*)Layer(0x5b) = 0x90;
	}
	else
	{
		*(short*)Layer(0x5b) = 0;
	}
	if( Layer[0] )
	{
		ProcessLayer(Layer[0]);
	}
}

void Test::Tick(const std::chrono::duration<double>& Delta)
{
	static double Timer = 0.0;
	Timer += Delta.count();
	if( Timer >= 0.5 && Running )
	{
		//do something every half-second
		Timer = 0.0;
		if( SaiPal::Instance().GetSession().ActiveCanvas()[0] )
		{
			SaiCanvas Canvas(
				SaiPal::Instance().GetSession().ActiveCanvas()[0]);
			Pointer Layer = Canvas.GetFirstLayer();
			ProcessLayer(Layer);
		}
	}
}
void Test::Run(const std::vector<std::string>& Args)
{
	if( Args.size() >= 2 )
	{
		if( !Args[1].compare("go") )
		{
			Running = true;
		}
		else if( !Args[1].compare("stop") )
		{
			Running = false;
		}
	}
}