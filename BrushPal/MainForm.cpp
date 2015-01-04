#include "MainForm.h"

void BrushPal::MainForm::Log(const std::string& Message)
{
	cStatusLabel->Text = gcnew System::String(Message.c_str());
	TimerLogFade->Start();
	Tick = 0;
}