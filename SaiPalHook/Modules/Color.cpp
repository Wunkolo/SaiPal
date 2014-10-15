#include "Color.h"
#include "../SaiPal.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

Color::Color() : Running(false), Active(false), CurMutator(nullptr)
{
	//Random
	Mutators[{ "random", "rand" }] =
		[](SaiColor& color)
	{
		color.Set(rand(),
				  rand(),
				  rand());
	};

	//Inverse
	Mutators[{ "inverse", "invert", "inv" }] =
		[](SaiColor& color)
	{
		color.SetRGBX(color.RGBX() ^ 0xFFFFFF00);
	};

	//Complement
	Mutators[{ "complement", "comp", "harm" }] =
		[](SaiColor& color)
	{
		float h, s, v, r, g, b;
		RGBtoHSV(
			color.Redf(),
			color.Greenf(),
			color.Bluef(),
			h, s, v);
		h += 0.5;// Shift hue to opposite side
		HSVtoRGB(
			h, s, v,
			r, g, b);
		color.SetF(r, g, b);
	};
}

Color::~Color()
{
}

std::string Color::Info() const
{
	//generate mutator list
	std::stringstream MutatorList;
	std::map <
		std::vector<std::string>,
		Mutator
	>::const_iterator it;
	for( it = Mutators.begin(); it != Mutators.end(); it++ )
	{
		const std::vector<std::string>& names = (it->first);
		std::vector<std::string>::const_iterator name;
		MutatorList << "\t\t[ ";
		for( name = names.begin(); name != names.end(); name++ )
		{
			MutatorList << *name
				<< (name == (--names.end()) ? "" : " | ");
		}
		MutatorList << " ]" << std::endl;
	}

	return
		"'color' shows hex values of the current primary and secondary color.\n"
		"'color [inverse|invert|inv]' inverts your current primary color.\n"
		"'color [complement|comp|harm]' sets your primary color to its complement hue.\n"
		"'color [random|rand]' sets your primary color to a random color.\n"
		"'color start [mutator]' starts a color mutator.\n"
		"\tAvailable mutators:\n"
		+
		MutatorList.str()
		+
		"'color stop' stops a previously started mutator.\n"
		"'color [pause|resume]' pauses or resumes a previously started mutator.\n"
		"\tex: 'color start complement' makes your secondary color equal\n"
		"\t\tto the compliment of your primary color.\n"
		"\tex: 'color start random' sets your secondary color to\n"
		"\t\ta random color\n"
		"\t\t(hold 'X' while drawing to get random colors every stroke).\n";
}

void Color::Tick(const std::chrono::duration<double>& Delta)
{
	if( Running && Active )
	{
		if( CurMutator )
		{
			SaiColor curColor = SaiPal::Instance().GetSession().GetPrimaryColor();
			CurMutator(curColor);
			SaiPal::Instance().GetSession().SetSecondaryColor(curColor);
		}
	}
}

void Color::Run(const std::vector<std::string>& Args)
{
	if( Args.size() == 1 )
	{
		std::cout << "Current Color:\n\tPrimary:   #"
			<< std::hex << std::setw(6) << std::setfill('0') << std::uppercase
			<< (SaiPal::Instance().GetSession().GetPrimaryColor().RGBX() >> 8)
			<< std::endl
			<< "\tSecondary: #"
			<< std::hex << std::setw(6) << std::setfill('0') << std::uppercase
			<< (SaiPal::Instance().GetSession().GetSecondaryColor().RGBX() >> 8)
			<< std::endl;
	}
	if( Args.size() >= 2 )
	{
		if( !Args[1].compare("start") )
		{
			if( Args.size() == 3 )//check for mutator name
			{
				//search for mutator
				CurMutator = nullptr;
				//loop through mutators
				std::map <
					std::vector<std::string>,
					Mutator
				>::iterator it;
				for( it = Mutators.begin(); it != Mutators.end(); it++ )
				{
					//loop through names
					const std::vector<std::string>& names = (it->first);
					std::vector<std::string>::const_iterator name;
					for( name = names.begin(); name != names.end(); name++ )
					{
						if( !Args[2].compare(*name) )
						{
							CurMutator = it->second;
							std::cout << "Starting color mutator: "
								<< names[0]
								<< std::endl;
							Running = true;
							Active = true;
							break;
						}
					}
				}
				if( CurMutator == nullptr )
				{
					std::cout << "Unknown mutator name" << std::endl;
				}
			}
			else
			{
				std::stringstream MutatorList;
				std::map <
					std::vector<std::string>,
					Mutator
				>::const_iterator it;
				for( it = Mutators.begin(); it != Mutators.end(); it++ )
				{
					const std::vector<std::string>& names = (it->first);
					std::vector<std::string>::const_iterator name;
					MutatorList << "\t[ ";
					for( name = names.begin(); name != names.end(); name++ )
					{
						MutatorList << *name
							<< (name == (--names.end()) ? "" : " | ");
					}
					MutatorList << " ]" << std::endl;
				}
				std::cout << "Must input a mutator. 'color start [mutator]'\n\tAvailable mutators:"
					<< std::endl
					<< MutatorList.str();
			}
		}
		else if( !Args[1].compare("stop") )
		{
			if( Running )
			{
				Running = false;
				Active = false;
				CurMutator = nullptr;
			}
			else
			{
				std::cout << "No color mutator in progress." << std::endl;
			}
		}
		else if( !Args[1].compare("pause") )
		{
			if( Running && Active )
			{
				Active = false;
				std::cout << "Color mutator paused(type 'color resume' to continue)"
					<< std::endl;
			}
			else
			{
				std::cout << "No color mutator in progress" << std::endl;
			}
		}
		else if( !Args[1].compare("resume") )
		{
			if( Running && !Active )
			{
				Active = true;
				std::cout << "Color mutator resumed" << std::endl;
			}
			else
			{
				std::cout << "No color mutator in progress" << std::endl;
			}
		}
		else if( !Args[1].compare("inverse") ||
				!Args[1].compare("invert") ||
				!Args[1].compare("inv") )
		{
			SaiPal::Instance().GetSession().
				SetPrimaryColor(
				SaiPal::Instance().GetSession().GetPrimaryColor().RGBX() ^ 0xFFFFFF00);
		}
		else if( !Args[1].compare("complement") ||
				!Args[1].compare("comp") ||
				!Args[1].compare("harm") )
		{
			SaiColor CurColor = SaiPal::Instance().GetSession().GetPrimaryColor();
			float h, s, v, r, g, b;
			RGBtoHSV(
				CurColor.Redf(),
				CurColor.Greenf(),
				CurColor.Bluef(),
				h, s, v);
			h += 0.5;
			HSVtoRGB(
				h, s, v,
				r, g, b);
			CurColor.SetF(r, g, b);
			SaiPal::Instance().GetSession().
				SetPrimaryColor(CurColor);
		}
		else if( !Args[1].compare("random") ||
				!Args[1].compare("rand") )
		{
			SaiPal::Instance().GetSession().
				SetPrimaryColor(
				rand(),
				rand(),
				rand());
		}
		else
		{
			std::cout << "Unknown parameter: " << Args[1] << std::endl;
		}
	}
}

void Color::RGBtoHSV(
	float r, float g, float b,
	float& h, float& s, float&v)
{
	float K = 0.f;
	if( g < b )
	{
		std::swap(g, b);
		K = -1.f;
	}
	float min_gb = b;
	if( r < g )
	{
		std::swap(r, g);
		K = -2.f / 6.f - K;
		min_gb = std::min(g, b);
	}
	float chroma = r - min_gb;
	h = std::fabs(K + (g - b) / (6.f * chroma + 1e-20f));
	s = chroma / (r + 1e-20f);
	v = r;
}

void Color::HSVtoRGB(
	float h, float s, float v,
	float &r, float &g, float &b)
{
	int i;
	float f, p, q, t;

	if( s == 0 )//grayscale
	{
		r = g = b = v;
		return;
	}
	i = (int)floor(h * 6);
	f = h * 6 - i;
	p = v * (1 - s);
	q = v * (1 - f * s);
	t = v * (1 - (1 - f) * s);
	switch( i % 6 )
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:
		r = v;
		g = p;
		b = q;
		break;
	}
}