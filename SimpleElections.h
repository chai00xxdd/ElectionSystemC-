#pragma once
#include"Elections.h"
#include"DividedDistrict.h"
#include<string>
namespace elections
{
	class SimpleElections:public Elections
	{
	private:
		std::string type = "Simple";
	public:
		//ctors dtors
		SimpleElections() = default;
		SimpleElections(const Date&date,int rep_number);
		SimpleElections(const SimpleElections&other) = delete;
		SimpleElections(std::istream&in):Elections(in){}

		
		virtual void addCitizen(const Citizen&c, int districtId = -1)override;
		virtual void addPartyMember(int citizenId, int partyId, int districtId = -1)override;
		virtual void addDistrict( District*district)override;
		virtual void printResults()override;
		virtual const std::string&getType()const { return type; }
		


	
		//operators
		void operator=(const SimpleElections&other) = delete;
		
 	};
}

