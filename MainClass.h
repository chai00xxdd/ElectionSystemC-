#pragma once
#include"Elections.h"
#include<string>
#include<vector>
using namespace elections;
class MainClass
{
private:
	static Elections *election_manager;
	static bool once;
	static bool alive;
	struct SHORCUT
	{
		std::string name;
		void(*f)(void);
		void execute() { f(); }
	};
	static std::vector<SHORCUT> shorcuts;
	static void init();
	static void initMain();
	static void printMenu();
public:
	static void run();
	static void addCitizen();
	static void addParty();
	static void addDistrict();
	static void addPartyMember();
	static void showResults();
	static void showParties();
	static void showDistricts();
	static void vote();
	static void showCitizens();
	static void startMenu();
	static void saveElectionsRound();
	static void loadElectionsRound();
};

