#include "VotingSystem.h"
#include"Party.h"
#include"FILEIO.h"
#include"Exceptions.h"
#include<string>
#include"Exceptions.h"
#include<algorithm>
#include<map>
#include"Templates.h"
namespace elections
{
	//ctors dtors
	VotingSystem::VotingSystem(const VotingSystem&other)
		: require_update(other.require_update), votes(other.votes)
		, representative_number(other.representative_number)
		,mode(other.mode),results(other.results),results_map(other.results_map)
		
	{
		
		

	}
	
	VotingSystem::VotingSystem(int representative_number,int mode)
		:require_update(false)
		, representative_number(representative_number), votes(0)
		, mode(mode)

	{
		
	}
	

	//elections functions
	void VotingSystem::vote(int party_id)
	{
		
		auto result = results_map.find(party_id);
		 if(result==results_map.end())
			 throw std::invalid_argument(std::string("party with id:") + std::to_string(party_id) +
				 " doesnt exists!!!");
		     partyResults& p_result = *result->second;
		      p_result.votes_counter++;
			if (mode&LIVE_UPDATE&&p_result.getParty())
			    {
					p_result.getParty()->inscraseVotes();
				}
				require_update = true;
				votes++;
				
	}
	void VotingSystem::updateResults()
	{
		
		if (require_update&&results.size() > 0 && votes > 0)
		{
			
			require_update = false;
			setResults();
		}

	}
	void VotingSystem::setResults()
	{
	
		Templates::Sort(results, [](const auto&p1, const auto&p2)
		{return (*p1)<(*p2); }, false);
			
		int total = 0;
		for (SmartPtr<partyResults> result:results)//fix results
		{
			result->update(votes,representative_number);
			total += result->representative_earned;
		}
		
		if (total > representative_number)
			throw std::logic_error("failed to calculate Results in votingSystem");
		int i = 0;
		while (total < representative_number)//fix results to int value
		{
			while (results[i%length()]->votes_counter == 0)
				i++;
			results[i++%length()]->representative_earned += 1;
			total++;
		}

	}

	

	void VotingSystem::addParty(const SmartPtr<Party> &party)
	{
		if (party == nullptr)
			throw NullException("VotingSystem::addParty: party given is null ptr ");
		if (results_map.find(party->getId()) != results_map.end())
		{
			return;
		}
		try
		{
			results.push_back(new partyResults(party));
			results_map.insert({ party->getId(),results.back() });
		}
		catch (std::bad_alloc&e)
		{
			throw MemoryException(std::string("VotingSystem::addParty error ") + e.what());
		}

	}

	VotingSystem::partyResults::partyResults(const SmartPtr<Party>& party)
		:party(party), vote_precent(0), representative_earned(0), votes_counter(0)
		
	{
		if (party == nullptr)
			throw NullException("VotingSystem::partyResults ctor party given is nullptr");
	}
	VotingSystem::partyResults::partyResults(std::istream&in)
		:party(SmartPtr<Party>())
	{
		load(in);
	}
	bool VotingSystem::partyResults::operator==(const partyResults&other)const
	{
		return votes_counter == other.votes_counter;
	}
	bool VotingSystem::partyResults::operator>(const partyResults&other)const
	{
		return votes_counter > other.votes_counter
			|| (votes_counter == other.votes_counter)
			&&(getId()<other.getId())
			;
	}
	bool VotingSystem::partyResults::operator<(const partyResults&other)const
	{
		return votes_counter < other.votes_counter
			|| (votes_counter == other.votes_counter)
			&& (other.getId()<getId())
			;
	}
	void VotingSystem::partyResults::update(int votes,int representative_number)
	{
		float div = votes == 0 ? 1.0f : static_cast<float>(votes);
        vote_precent = (votes_counter / div);
		representative_earned
			= static_cast<int>(representative_number * vote_precent);
	}

	void VotingSystem ::clear()
	{
	
		results.clear();
		//clear map
		votes = 0;
		require_update = false;

	}
	
	
	
	void VotingSystem::save(std::ostream&out)
	{
		if (!out)
			throw IOException("bad file");
		
			FILEIO::save({ representative_number,mode,votes }, out);
			FILEIO::save({ require_update }, out);
			Templates::saveDS(results, out);
		
	
	}
	void VotingSystem::load(std::istream&in)
	{
		if (!in)
			throw IOException("bad file");
		clear();
		FILEIO::load({ &representative_number,&mode,&votes }, in);
		FILEIO::load({ &require_update }, in);
		Templates::loadDS(results, in);
		
		
	
	}


	void VotingSystem::partyResults::load(std::istream&in)
	{
		if (!in)
			throw IOException("bad file");
		party.load(in);
		FILEIO::load({&votes_counter,&representative_earned }, in);
		FILEIO::load({ &vote_precent }, in);
	}
	void VotingSystem::partyResults::save(std::ostream&out)
	{
		if (!out)
			throw IOException("bad file");
		party.save(out);
		FILEIO::save({votes_counter,representative_earned }, out);
		FILEIO::save({ vote_precent }, out);
		
	}
	void VotingSystem::setParties(const std::unordered_map<int,SmartPtr<Party>>&parties)
	{
		
		for ( SmartPtr<partyResults> r : results)
		{
			
			auto p = parties.find(r->getId());
			if (p != parties.end())
			{
				r->party = p->second;
				results_map.insert({ r->getId(),r});
			}

		}
	}

	
	
}