#pragma once
#include <map>
#include <vector>
#include <chrono>

#include "SaiPalConfig.h"
#include "Modules\SaiModule.h"

#include "Singleton.h"
#include "SaiSession.h"

class SaiPal : public Singleton < SaiPal >
{
public:
	SaiPal();
	~SaiPal();

	//Saipal directory along-side the dll file. Use for reading/writing.
	std::string GetDirectory();

	void Tick(const std::chrono::duration<double> Delta);

	SaiSession GetSession()
	{
		return this->Session;
	}

private:
	//Console
	void PrintConsole();
	std::map<std::string, SaiModule*> Commands;
	std::vector<std::string> PrevCommands;
	std::vector<std::string>::iterator PrevCommand;
	std::string Command;
	std::string PrevSuggestion;

	//Sai Session
	SaiSession Session;
};