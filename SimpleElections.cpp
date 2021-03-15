#include "SimpleElections.h"
#include"DividedDistrict.h"
#include"Citizen.h"
#include"Exceptions.h"
#include"Templates.h"
using namespace std;
namespace elections
{
	void SimpleElections::addCitizen(const Citizen&c, int districtId)
	{
		districtId = (districts)[0]->getId();
		Elections::addCitizen(c, districtId);
	}

	SimpleElections::SimpleElections(const Date&date,int rep_number)
		:Elections(date)
	{
		try
		{
			Elections::addDistrict(new DividedDistrict("GENERAL", rep_number));
		}
		catch (std::bad_alloc&e)
		{
			throw MemoryException(std::string(e.what())+" error in SimpleElections");
		}
		
	}

	void SimpleElections::addPartyMember(int citizenId, int partyId, int districtId)
	{
		districtId= (districts)[0]->getId();
		 Elections::addPartyMember(citizenId, partyId, districtId);
	}

	void SimpleElections::addDistrict( District*district)
	{
		throw std::logic_error("addDistrict un athroized action");
		
	}
	
	void SimpleElections::printResults()
	{
		std::cout << "***********Elections RESULTS***************" << std::endl;
		if (getParties().size() == 0)
			std::cout << "NO PARTIES IN THE SYSTEM" << std::endl;
		else
		{
			
			cout<< "votes precent:" << getVotesPrecent() << "%" << endl;
			cout << "total votes:" << getTotalVotes() << "/" << citizensDB.getCitizens().size() << endl;
			std::cout << "*******PARTY RESULTS*********" << std::endl;
			for (const SmartPtr<Party>&p:parties)
			{
				std::cout << "#" << p->getName() << std::endl;
				std::cout << "party name:" << p->getName() << endl;
				std::cout << "party id:" << p->getId() << endl;
				cout << "party leader:" << p->getLeader()->getName() << endl;
				cout << "total votes:" << p->getTotalVotes() << endl;
				float div = getTotalVotes() ? static_cast<float>(getTotalVotes()) : 1;
				cout << "votes precent:" << (p->getTotalVotes() / div) * 100 << "%" << endl;
				cout << "Reprenstive Earned:" << p->getRepresentativesSum() << endl;
				cout << "Represntive List:";
				bool notEmpty = p->getAllMembers().size() > 0;

				if (notEmpty)
				{
					cout << endl;
					const std::vector<Citizen*>&earned_list = p->getAllMembers().find(districts[0]->getId())->second
						.getMembers();
					for (size_t i = 0; i < earned_list.size() && i < static_cast<size_t>(p->getRepresentativesSum()); i++)
					{

						cout << i + 1 << "." << *earned_list[i] << endl;
					}
					int diff = p->getRepresentativesSum() - earned_list.size();
					if (diff > 0)
						cout << "PARTY MISSING " << diff << " REPRESENTATIVES FROM LIST!!!!!!!!" << endl;
				}
				else cout << "none"<<endl;
				if (p != parties.back())
					cout << "__________________________________";
				cout << endl;
				
			}
			
		}
	}
	
	
}