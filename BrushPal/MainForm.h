#pragma once

#include <string>

namespace BrushPal {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			Log("SaiBrush");

			TimerLogFade->Start();
		}

		void Log(const std::string& Message);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::FlowLayoutPanel^  SaiBrushPanels;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  cStatusLabel;
	private: System::Windows::Forms::Timer^  TimerLogFade;
	private: System::ComponentModel::IContainer^  components;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->SaiBrushPanels = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->cStatusLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->TimerLogFade = (gcnew System::Windows::Forms::Timer(this->components));
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// SaiBrushPanels
			// 
			this->SaiBrushPanels->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->SaiBrushPanels->Location = System::Drawing::Point(80, 41);
			this->SaiBrushPanels->Name = L"SaiBrushPanels";
			this->SaiBrushPanels->Size = System::Drawing::Size(214, 380);
			this->SaiBrushPanels->TabIndex = 0;
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1)
			{
				this->cStatusLabel
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 445);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(491, 22);
			this->statusStrip1->SizingGrip = false;
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// cStatusLabel
			// 
			this->cStatusLabel->BackColor = System::Drawing::SystemColors::Control;
			this->cStatusLabel->Name = L"cStatusLabel";
			this->cStatusLabel->Size = System::Drawing::Size(53, 17);
			this->cStatusLabel->Text = L"BrushPal";
			// 
			// TimerLogFade
			// 
			this->TimerLogFade->Interval = 10;
			this->TimerLogFade->Tick += gcnew System::EventHandler(this, &MainForm::TimerLogFade_Tick);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(491, 467);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->SaiBrushPanels);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"MainForm";
			this->Text = L"BrushPal";
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: 
		template <typename T, typename F>
		T Lerp(const T Start,const T End, const F Phase)
		{
			return Start + (T)((F)(End - Start) * Phase);
		}
	private:		unsigned int Tick = 0;
	private: System::Void TimerLogFade_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		const unsigned int FadeTime = 2500;

		Tick += TimerLogFade->Interval;

		if( Tick < FadeTime ) // Within FadeTime
		{
			float Phase = (Tick / (float)FadeTime);
			Color FadeColor,FadeTo;
			FadeTo = cStatusLabel->BackColor;

			FadeColor = FadeColor.FromArgb(
				Lerp((unsigned char)0, FadeTo.R, Phase),
				Lerp((unsigned char)0, FadeTo.G, Phase),
				Lerp((unsigned char)0, FadeTo.B, Phase) );
			//Interpolate foreground color into background color

			cStatusLabel->ForeColor = FadeColor;
		}
		else
		{
			cStatusLabel->Text = nullptr;
			Tick = 0;
			TimerLogFade->Stop();
			cStatusLabel->ForeColor = cStatusLabel->BackColor;
		}
		cStatusLabel->Invalidate();
	}
};
}
