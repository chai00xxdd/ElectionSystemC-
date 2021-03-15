#pragma once
#include<string>
#include"Date.h"
#include"CitizensBook.h"
#include<vector>
#include<unordered_map>
namespace elections
{
	class Elections
	{
	    private:
		static constexpr int MINIMUM_AGE = 18;
		virtual bool partyCompare(const SmartPtr<Party>&p1, const SmartPtr<Party>&p2)const;
		//the funcions save the pointers connections
		//between citizens parties and districts
		void saveConnections(std::ostream&out);
		//the funcions load and create the pointers connections
		//between citizens parties and districts
		void loadConnections(std::istream&in);
		bool require_update;
		Date date;
		int total_votes = 0;
	    protected:
		std::unordered_map<int, SmartPtr<Party>>partiesMap;//for fast search
		std::vector<SmartPtr<Party>> parties;//fast iteration

		std::unordered_map<int,District*>districtsMap;//fast search
		std::vector<District*>districts;//fast iteration

		CitizensBook citizensDB;//voters book
		//the functions sort the parties by reprenstiveSum
		//and party id
		void updateResults();
		//the function is used in show results
		//and can be override by the derived classes
		virtual void printResults();
		
	public:
		//ctors dtors
		Elections():Elections(Date()){}
		Elections(const Date &date);
		Elections(std::istream&in);
		Elections(const Elections&other) = delete;
		virtual ~Elections();
		

	
		//the function get a citizen c and districtId
		//and if c doesnt exists add him to the citizensList
		//in a district that match to districtId if no such district
		//exists return false
		virtual void addCitizen(const Citizen&c, int districtId=-1);
		//the function get a paryName and leaderId 
		//and if a citizen exists with the same id
		//create and add a new party to the parties
		void addParty(const std::string&partyName, int leaderId);
		
		//the function find the citizen with citizenId and insert
		//him as a member of district with district_id in the party
		//if  possible else reutrn false
	    //this should be given an empty district
		virtual void addDistrict(District*district);
		virtual void addPartyMember(int citizenId, int partyId, int district_id);
		//the function get a citizen and a partyId
		//and if the citzen and the paryexists and the citzen didnt voted yet
		//allow the citizen to vote to the party in his district
		 void vote(int citizenId, int partyId);

		//remove Citizen
		void removeCitizen(int id);
		//bool remove party
		void removeParty(int partyid);
		
		

		//getters
		int getTotalVotes()const { return total_votes; }
		float getVotesPrecent()const;
		CitizensBook&getCitizensBook() { return citizensDB; }
		
		std::vector<District*>getDistricts() { return districts; }
		const std::vector<District*>&getDistricts()const { return districts; }
		std::vector<SmartPtr<Party>>&getParties() { return parties; }
		const CitizensBook&getCitizensBook()const { return citizensDB; }
		
		
		const std::vector<SmartPtr<Party>>&getParties()const { return parties; }
		const Date&getDate()const { return date; }
		Date&getDate() { return date; }
		virtual const std::string&getType()const=0;
		

		//setters
		void setDate(const Date&date) { this->date = date; }


		//show functions

		//prints all the citizens
		void showCitizens()const;
		//prints all the districts
		void showDistricts()const;
		//prints all the parties
		void showParties()const;
		//the functions show the current results of the elections 
		void showResults();
		//operatros
		void operator=(Elections&other) = delete;

		//IO
		virtual void save(std::ostream&out);
		virtual void load(std::istream&in);

		
		
	


	};
}
