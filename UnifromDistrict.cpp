#include "UnifromDistrict.h"

namespace elections
{
	UnifromDistrict::UnifromDistrict(const std::string&name, int rep_number)
		:District(name,rep_number)
	{

	}
	 void UnifromDistrict::print(std::ostream&out)const
	{
		 District::print(out);
		 out << " type:" << " Unifrom";
	}
	void UnifromDistrict::updatePartiesResults()
	{

		voting_system.updateResults();
		auto results = voting_system.getResults();
		bool first = true;
		for (auto&p_results:results)
		{
			if (first)
			{
				if(p_results->getTotalVotes()>0)
				p_results->getParty()->addRep(representative_number);
				first = false;
			}
			int p_votes = p_results->getTotalVotes();
			p_results->getParty()->addvotes(p_votes);
			

		}
	}
	void UnifromDistrict::showWinnersResults()
	{
		if (voting_system.getTotalVotes() > 0)
		{
			SmartPtr<Party> winner = voting_system.getResults()[0]->getParty();
			std::cout << "District owned by " << winner->getLeader()->getName()
				<< " from party " << winner->getName() << std::endl;
	    }
	}

}