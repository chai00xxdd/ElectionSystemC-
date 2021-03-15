#pragma once
#include<string>
#include "Citizen.h"
#include"Representatives.h"
#include"Entity.h"
#include<unordered_map>
namespace elections
{
	class Party:public Entity
	{
	private:
		const Citizen *leader=nullptr;
		static int ID_GENERATOR;
		int totalVotes = 0;//total votes in all districts
		int representativesSum = 0;//total reprenstaive sum earned
		std::unordered_map<int,Representatives> members;
		
	public:
		//ctors dtors
		Party() = default;//for loading data
		Party(const Party&other)=delete;
		Party(std::istream&in);
		explicit Party(const std::string& name,const Citizen *leader);//generate id
		//getters 
		int getTotalVotes()const { return totalVotes; }
		int getRepresentativesSum()const { return representativesSum; }
		const Citizen*getLeader()const { return leader; }
		const std::unordered_map<int,Representatives>&getAllMembers()const { return members; }
		std::unordered_map<int,Representatives>&getAllMembers() { return members; }
		//setters
		void setTotalVotes(int total);
		void setRepresentativesSum(int rep);
		void setLeader(const Citizen*leader);
		
		//voting functions
		void inscraseVotes() { totalVotes++; }
		void addRep(int rep);
		void addvotes(int votes);
		//adding removing members
		void addMember(const Citizen*c, const District*district);
		bool isExists(const Citizen&c)const;
		const std::vector<Citizen*>&getMembersAt(int district_id)const;
		std::vector<Citizen*>&getMembersAt(int distridct_id);
		void removeMember(int id);
		


		//operatoers
		friend std::ostream& operator<<(std::ostream& in, const Party&p);
		void operator=(const Party&other) = delete;
		bool operator==(const Entity&other)const override;
		//IO
		
		virtual void save(std::ostream&out)override;
		virtual void load(std::istream&in)override;
		

		//static IO 
		static void loadState(std::istream&in);
		static void saveState(std::ostream&out);
		static int getState() { return ID_GENERATOR; }
		static void setState(int id) { ID_GENERATOR = id; }

		
	
	};
	std::ostream& operator<<(std::ostream& in, const Party&p);
	

}