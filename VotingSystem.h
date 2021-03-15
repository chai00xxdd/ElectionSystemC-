#pragma once
#include"Party.h"
#include<string>
#include<vector>
#include<unordered_map>
#include"DynamicArray.h"
#include"SmartPointers.h"
namespace elections
{
	class VotingSystem
	{
	public:
		class partyResults
		{
		private:
			SmartPtr<Party> party=nullptr;
			int votes_counter;
			int representative_earned;
			float vote_precent;

		public:
			partyResults(const SmartPtr<Party> &party);
			partyResults(std::istream&in);
			partyResults(const partyResults&other) = default;
			partyResults() = default;
			//return the total votes part got
			int getTotalVotes()const { return votes_counter; }
			//return the amont uf represntives the party won
			int getRepresentativeEarned()const { return representative_earned; }
			//update the precent and represntisve sum
			void update(int total_votes, int rep_numb);
			//getters
			//return the votePrecent of party in the voting system from all votes
			float getVotePrecent()const { return vote_precent; }
			const SmartPtr<Party> &getParty()const { return party; }
			SmartPtr<Party> getParty() { return party; }
			int getId()const { return party->getId(); }
			//operators
			bool operator==(const partyResults&other)const;
			bool operator>(const partyResults&other)const;
			bool operator<(const partyResults&other)const;

			//IO
			void load(std::istream&in);
			void save(std::ostream&out);



			friend class VotingSystem;
		};
	private:

		int representative_number;
		int mode;
		int votes = 0;
		bool require_update;//if set to  true the current result need to be sorted
		DynamicArray<SmartPtr<partyResults>> results;//return result to user
		std::unordered_map<int, SmartPtr<partyResults>>results_map;
		//the function calculte each partyResult the 
		//values of the total votes,vote precent,represntives earned
		void setResults();

	public:

		//ctors dtors
		VotingSystem(const VotingSystem&other);
		VotingSystem(int representative_number, int mode = NORMAL_MODE);
		//geters setters
		const DynamicArray<SmartPtr<partyResults>>& getResults()const { return results; }
		DynamicArray<SmartPtr<partyResults>>& getResults() { return results; }
		int getTotalVotes()const { return votes; }
		int length()const { return results.size(); }
		//set the mode of the system
		//AUTO_UPDATE update the results on [] operator
		//LIVE_UPDATE update the votes in the parties pointers
		//NORMAL_MODE not one of the aboves
		void setMode(int mode) { this->mode = mode; }
		//actions
		//adding a party to the voting system
		void addParty(const SmartPtr<Party>& party);//through pointer
		//voting to the party with the id
		void vote(int party_id);
		//update the results in the system
		//so the first result is the of the winner 
		//and the last result is whom got the lowest votes
		void updateResults();
		//remove and clear
		void clear();
		
		//the functions set the parties pointers after load
		
		void setParties(const std::unordered_map<int, SmartPtr<Party>>&parties);
		
		
		//operators
		VotingSystem& operator=(const VotingSystem&other) = delete;
		 
		//constants
		static constexpr int AUTO_UPDATE = 4;//update the results order on get functions
		static constexpr int LIVE_UPDATE = 1;//update the parties on each vote
		static constexpr int NORMAL_MODE = 0;//neither of the above

		//IO
		void save(std::ostream&out);
		void load(std::istream&in);
		
		

	};
}

