#pragma once
#include <set>
#include <vector>
#include <chrono>

#include "SaiPalConfig.h"

#include "Singleton.h"
#include "SaiSession.h"


class SaiPal : public Singleton < SaiPal >
{
public:
	SaiPal();
	~SaiPal();

	void Tick(const std::chrono::duration<double> Delta);

private:
	//Console
	void PrintConsole();
	std::set<std::string> Commands;
	std::vector<std::string> PrevCommands;
	std::vector<std::string>::iterator PrevCommand;
	std::string Command;
	std::string PrevSuggestion;

	//Sai Session
	SaiSession Session;
};