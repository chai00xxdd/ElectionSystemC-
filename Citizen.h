#pragma once
#include<string>
#include<iostream>
#include"Date.h"
#include"SmartPointers.h"
#include"Entity.h"
namespace elections
{
	class District;
	class Party;
	class Citizen:public Entity
	{//shallow copy is enough here so no need cctor and dtor
	private:
		//District _district;
		int birth;
		const District*district=nullptr;//optinal
		SmartPtr<Party> party=nullptr;//optinal
		bool voted;
		
	
	public:
		//ctors dtors
		
		Citizen(std::istream&in);
		Citizen(const Citizen&c)=default;
		Citizen(const std::string &name, int id, int birth);
		Citizen();//for reading from file
		
		
		//getters 
		int getBirth()const { return birth; }
		const District* getDistrict()const { return district; }
		const SmartPtr<Party>& getParty()const { return party; }
		bool isvoted()const { return voted; }
		
		//setters
		void setParty(const SmartPtr<Party>& p);
		void setBirth(int birth);
		void setDistrict(const District*district);
		void setVoted() { voted = true; }

		
		void joinParty(SmartPtr<Party> party,const District*district);
		void joinDistrict( District*district);

		void leaveDistrict();
		void leaveParty();


		void vote(int partyId);
		
		

		//operator
		//operator = default
		friend std::ostream& operator<<(std::ostream&, const Citizen&);
		bool operator==(const Entity&other)const override;
		void operator=(const Citizen&other) = delete;

		virtual void save(std::ostream&out)override;
		virtual void load(std::istream&in)override;


	};
	std::ostream& operator<<(std::ostream&, const Citizen&);
}

