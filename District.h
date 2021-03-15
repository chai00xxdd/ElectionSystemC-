#pragma once
#include<string>
#include"VotingSystem.h"
#include"Entity.h"
#include<list>
#include<unordered_map>
namespace elections
{
	

	class District:public Entity
	{
	private:
	static int ID_GENRATOR;
	protected:	
	int representative_number=-1;
	std::list<Citizen*> citizens;
	mutable VotingSystem voting_system;
	bool supervise=true;//if set to true on insert operation valdity check will happen
		              //to ensure citizen isnt belong to district alredy
	
	protected:
		virtual void showResultsTitle();
	    virtual void showWinnersResults()=0;
		virtual void showPartiesResults();
		virtual void print(std::ostream&out)const;
	public:
		static void loadState(std::istream&in);
		static void saveState(std::ostream&out);
		static int getState() { return ID_GENRATOR; }
		static void setState(int id) { ID_GENRATOR = id; }
		
		//ctor dtors
		District(const District&other);
		District(const std::string&name, int rep);
		explicit District(std::istream&in);
		District();//for loading data
		virtual ~District() {};

		//getters 
		const VotingSystem& getVotingSystem()const;
		VotingSystem&getVotingSystem();
		
		
		const std::list<Citizen*>& getCitizens()const { return citizens; }
		std::list<Citizen*>& getCitizens() { return citizens;}
		int getRepresentativeNumber()const { return representative_number; }
		float  getVotingPrecent()const;
		bool isSupervisor()const { return supervise; }
		//setters
		void setRepresentativeNumber(int representative_number);
		void setSupervise(bool supervise);
		
		
		
		//adding a citizen by poitner
		void addCitizen(Citizen*c);
		void removeCitizen(int citizenId);
		//
		//bool removeCitizen(int citizen_id);
		//suppose to vote with a citizen that exists in district
		void vote(Citizen&c, int partyId);
		void vote(int citizenId, int partyId);
		//shows the results of the elections in the district
		virtual void showResults();
		//operators
		
		virtual bool operator!=(const District&other)const  { return !(*this==other); }
		friend std::ostream& operator<<(std::ostream&out, const District&d);
		void operator=(const District&other) = delete;
		//inherticne 
		virtual void updatePartiesResults() = 0;
		//IO
		
		virtual void save(std::ostream&out)override;
		virtual void load(std::istream&in)override;
	};
	
	
	std::ostream& operator<<(std::ostream&out, const District&d);

}