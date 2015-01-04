#include "Test.h"
#include "../SaiPal.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>

Test::Test()
{
	Running = false;
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
#if defined(SAI120)
	*(short*)Layer(0x5b - 8) = !Layer(0x5b - 8).asUchar();

	if( Layer(0x5b - 8).asUshort() == 1 )
	{
		*(short*)Layer(0xB4 - 8) = 0x90;
	}
	else
	{
		*(short*)Layer(0xB4 - 8) = 0;
	}
#elif defined(SAI110)
	*(short*)Layer(0x5b) = !Layer(0x5b).asUchar();

	if( Layer(0x5b).asUshort() == 1 )
	{
		*(short*)Layer(0xB4) = 0x90;
	}
	else
	{
		*(short*)Layer(0xB4) = 0;
	}
#else
#endif

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
			printf("honk %x %x\n",
				   Canvas.GetWindow(),
				   SaiPal::Instance().GetSession().GetLayerListWindow());
			Pointer Layer = Canvas.GetFirstLayer();
			ProcessLayer(Layer);

			RedrawWindow(SaiPal::Instance().GetSession().GetLayerListWindow(),
						 0,
						 0,
						 RDW_INVALIDATE);

			//redraw preview
			RedrawWindow(SaiPal::Instance().GetSession().GetPreviewWindow(),
						 0,
						 0,
						 RDW_INVALIDATE);
			PostMessageA(SaiPal::Instance().GetSession().GetPreviewWindow(),
						 WM_USER + 3066, 0, 0);

			//redraw canvas
			RedrawWindow(Canvas.GetWindow(),
						 0,
						 0,
						 RDW_INVALIDATE);
			PostMessageA(Canvas.GetWindow(), WM_USER + 3066, 0, 0);
		}
	}
}
void Test::Run(const std::vector<std::string>& Args)
{
	std::cout << "Brush Test:" << std::endl;

	std::ofstream Dump("dump.txt");

	Dump << std::fixed << std::setprecision(2);

	for( unsigned int i = 0; i < 63; i++ )
	{
		Pointer BrushPtr = SaiPal::Instance().GetSession().GetBrush(i);
		std::cout << (void*)BrushPtr << std::endl;
		if( BrushPtr )
		{
			SaiBrush Brush(BrushPtr);
			Dump << std::setw(16) << "Brush Name: "
				<< std::setw(16) << Brush.GetName() << std::endl;
			Dump << std::setw(16) << "Description: "
				<< std::setw(16) << Brush.GetDescription() << std::endl;
			Dump << std::setw(16) << "Meta Name: "
				<< std::setw(16) << Brush.GetToolName() << std::endl;
			Dump << std::setw(16) << "Type: "
				<< std::setw(16) << Brush.GetType() << std::endl;
			Dump << std::setw(16) << "Shortcut Key: "
				<< std::setw(16) << Brush.GetShortcutKey() << std::endl;
			Dump << std::setw(16) << "Current Size: "
				<< std::setw(16) << Brush.GetSize() << std::endl;
			Dump << std::setw(16) << "Min Size: "
				<< std::setw(16) << Brush.GetMinSize() * 100 << '%' << std::endl;
			char out[9] = { 0 };
			_itoa(Brush.GetPressureFlags(), out, 2);
			Dump << std::setw(16) << "PressureFlags: "
				<< std::setw(16) << out << std::endl;

			Dump << std::string(48, 'Ä') << std::endl;
		}
	}

	Dump.close();

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