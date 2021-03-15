#include "DividedDistrict.h"
using namespace std;

namespace elections
{
	DividedDistrict::DividedDistrict(const std::string&name, int rep_number)
		:District(name,rep_number)
	{

	}

	 void DividedDistrict::updatePartiesResults()
	{
		
		 voting_system.updateResults();
		 auto results = voting_system.getResults();
		 for (SmartPtr<VotingSystem::partyResults>&p_results:results)
		 {
			 int p_votes = p_results->getTotalVotes();
			 int p_repSum = p_results->getRepresentativeEarned();
			 p_results->getParty()->addRep(p_repSum);
			 p_results->getParty()->addvotes(p_votes);

		 }
	}

	 void DividedDistrict::showWinnersResults()
	 {
		 if (voting_system.getTotalVotes() > 0)
		 {
			 std::cout << "District Splited To:" << std::endl;
			 auto results = voting_system.getResults();
			 int i = 1;
			 for (const SmartPtr<VotingSystem::partyResults>&p_results:results)
			 {
				
				 if (p_results->getRepresentativeEarned() > 0)
				 {
					 const SmartPtr<Party> p = p_results->getParty();
					 cout << i++ << "." << p->getLeader()->getName()
						 << " from party " << p->getName() << " earned "
						 << p_results->getRepresentativeEarned()
						 << " reprenstives" << endl;
				 }
				 else break;
			 }
		 }
	 }
	 void DividedDistrict::print(std::ostream&out)const
	 {
		 District::print(out);
		 out << " type:" << "Divided";
	 }

}