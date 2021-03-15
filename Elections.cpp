#include "Elections.h"
#include"FILEIO.h"
#include"DistrictLoader.h"
#include<string>
#include"Exceptions.h"
#include"Templates.h"
#include<algorithm>
using namespace VALIDATION;
using namespace std;
namespace elections
{
	Elections::Elections(const Date& date)
		:date(date), require_update(false)
	{

	}

	void Elections::addCitizen(const Citizen&c, int distrcitId)
	{
	    AgeValidator(date.getYear(),MINIMUM_AGE)(c.getBirth());
		IdValidation()(c.getId());
		auto district = districtsMap.find(distrcitId);
		if (district==districtsMap.end())
		{
			throw NoFoundException(std::string("district with id ") + std::to_string(distrcitId) + " doesnt exists");
		
		}
		citizensDB.addCitizen(c);
		citizensDB.getCitizens().back()->joinDistrict(district->second);
		
	}
	void Elections::addParty(const std::string &partyName, int leaderId)
	{
		
		Citizen&leader = *citizensDB.find(leaderId);
	
		 if (leader.getParty()!=nullptr)
		{
			
			throw std::logic_error(std::string("citizen ") + std::to_string(leaderId) + " alredy belong to party");
			
		}
		
		try
		{
			parties.push_back(SmartPtr<Party>(new Party(partyName, &leader)));
			partiesMap.insert({ parties.back()->getId(),parties.back() });
			leader.setParty(parties.back());
		}
		catch (std::bad_alloc&e)
		{
			throw MemoryException(std::string(e.what()) + " error in Elections addParty!!!");
		}
		for (District*d:districts)
		{
			d->getVotingSystem().addParty(parties.back());
		}
		

		
	}
	

	void Elections::showCitizens()const
	{
		std::cout << citizensDB;
		if (citizensDB.getCitizens().size() == 0)
			std::cout << "NO CITIZENS IN THE SYSTEM!!!" << std::endl;
	}
	void Elections::showDistricts()const
	{
		if (districts.size() == 0)
			std::cout << "NO DISTRICTS IN SYSTSM!!!!" << std::endl;
		size_t i = 1;
		for ( District*d:districts)
			std::cout << i++ << "." <<"[ "<< *d <<" ]"<< std::endl;
	}
	void Elections::showParties()const
	{

		if (parties.size() == 0)
		{
			std::cout << "NO PARTIES IN SYSTEM!!!" << std::endl;
		}
		for (const SmartPtr<Party>&p:parties)
		{

			
				std::cout << "#" << p->getName() << std::endl;
				std::cout << *p << std::endl;
			
		}
	}
	void Elections::removeCitizen(int id)
	{
		
		Citizen&c = *citizensDB.find(id);
		
		
			if(c.getDistrict())
			c.leaveDistrict();
			if (c.getParty() != nullptr)
				c.leaveParty();
			citizensDB.removeById(id);
			
	}
	void Elections::removeParty(int partyid)
	{
	
		auto p = std::find_if(parties.begin(), parties.end(), findPtrById(partyid));
		if (p == parties.end())
		{
			throw std::invalid_argument(std::string("party with id:") +
				std::to_string(partyid) + " doesnt exists");
		}
		
		SmartPtr<Party> party = *p;
		
		for (District*d : districts)
		{
			
			
			for ( auto& pair: party->getAllMembers())
			{
				Representatives&r = pair.second;
				std::vector<Citizen*>&memebers = r.getMembers();
				for(Citizen* member: memebers)
				{
					member->setParty(SmartPtr<Party>());
				}
			}
		}
			
		
		
	}
 	void Elections::addPartyMember(int citizenId, int partyId, int district_id)
	{
		
		Citizen&c = *citizensDB.find(citizenId);
		
		if (c.getParty() != nullptr)
		{
		throw std::logic_error(std::string("citizen with id ") + std::to_string(citizenId)+" alredy in party");
			
		}
		
		auto party = partiesMap.find(partyId);
		if(party==partiesMap.end())
		throw NoFoundException(std::string("party with id:") + std::to_string(partyId) + " doesnt exists");
		
		auto district = districtsMap.find(district_id);
		if (district == districtsMap.end())
		{
		throw std::logic_error(std::string("district with id:") + std::to_string(district_id) + " doesnt exists");
			
		}
		 c.joinParty(party->second,district->second);
		
	}
	void Elections::vote(int citizenId, int partyId)
	{

		Citizen&citizen = *citizensDB.find(citizenId);
		
		if (citizen.isvoted())
		{
	    throw std::logic_error(std::string("citizen ") + std::to_string(citizenId) + " alredy voted!!!");
			
		}
		
		citizen.vote(partyId);
		require_update = true;
		total_votes++;
		
	}
	void Elections::showResults()
	{
		updateResults();
		printResults();
		

	}
	



	//this should be given an empty district
	//and this is elections responsiblty to delete it!!!
	void Elections::addDistrict(District*district)
	{
		if (district == nullptr)
		{
			throw NullException( "district given is nullptr");
			
		}
		district->setSupervise(false);//alowing elections manage validaty check on add citizen
		districtsMap.insert({ district->getId(), district });
		districts.push_back(district);
		for (const SmartPtr<Party>& p:parties)
		{
			district->getVotingSystem().addParty(p);

		}
		
	}

	void Elections::save(std::ostream&out)
	{
		if (!out)
			throw IOException("Elections IO error");
		date.save(out);
		FILEIO::save({ total_votes }, out);
		FILEIO::save({ require_update }, out);
		int i = 0;
		for (District*d : districts)
		{
			d->setIndex(i++);
		}
	
		for (size_t i = 0; i < citizensDB.getCitizens().size(); i++)
			citizensDB[i].setIndex(i);
		Templates::saveDS(parties, out);
		citizensDB.save(out);
		DistrictLoader::saveDistricts(districts, out);
		saveConnections(out);
		

		
	}
	void Elections::loadConnections(std::istream&in)
	{
		
		for (SmartPtr<Party> p : parties)
		{
			partiesMap.insert({ p->getId(),p });
		}
		for (District*d : districts)
		{
			districtsMap.insert({ d->getId(),d });
			d->getVotingSystem().setParties(partiesMap);
		}
		for(SmartPtr<Citizen> c:citizensDB.getCitizens())
		{
			int districtIndex;
			FILEIO::load({ &districtIndex }, in);
			c->joinDistrict(districts.at(districtIndex));
		}

		
		for (SmartPtr<Party> p:parties)
		{
			
		//load leader
			int leader_index;
			FILEIO::load({ &leader_index }, in);
			p->setLeader(&citizensDB[leader_index]);
			citizensDB[leader_index].setParty(p);
			//load members at district
			int howManyLists;
			FILEIO::load({ &howManyLists }, in);
			for (int j = 0; j < howManyLists; j++)
			{
				int dis_id;
				FILEIO::load({ &dis_id }, in);
				const District*d = districts.at(dis_id);
				int members;
				FILEIO::load({ &members }, in);
				for (int k = 0; k < members; k++)
				{
					int c_index;
					FILEIO::load({ &c_index }, in);
					citizensDB[c_index].joinParty(p, d);

				}


			}
		}
		
	}
	void Elections::saveConnections(std::ostream&out)
	{
		
		for(SmartPtr<Citizen> c:citizensDB.getCitizens())
		{
			
			FILEIO::save({ c->getDistrict()->getIndex()}, out);
		}
		
		for (const SmartPtr<Party>& p:parties)
		{
			
			//save leader
			int leader_index =p->getLeader()->getIndex();
			//save members at district
			auto Allmembers =p->getAllMembers();
			FILEIO::save({ leader_index,static_cast<int>(Allmembers.size()) }, out);
			for (const auto&pair: Allmembers)
			{
				const Representatives&r = pair.second;
				size_t dis_ind = static_cast<size_t>(r.getDistrcit().getIndex());
				const std::vector<Citizen*> membersList = r.getMembers();
				
				FILEIO::save({ dis_ind,membersList.size() }, out);
				
				for(Citizen *c: membersList)
				{
					FILEIO::save({ c->getIndex() }, out);
				}


			}
		}
		
	}
	void Elections::load(std::istream&in)
	{
		
		if (!in)
			throw IOException("Elections IO error");
		date.load(in);
		FILEIO::load({ &total_votes }, in);
		FILEIO::load({ &require_update }, in);
		Templates::loadDS(parties, in);
		citizensDB.load(in);
		DistrictLoader::LoadDistricts(districts,in);
	    loadConnections(in);
		
		
	}

	 Elections::~Elections()
	{
		 Templates::ClearPtrDS(districts);
	}
	
	Elections::Elections(std::istream&in)
	{
		
		try
		{
			load(in);
		}
		catch (...)
		{
			Templates::ClearPtrDS(districts);
			throw;
		}
	}

	void Elections::updateResults()
	{
		if (require_update)
		{
			
			for (SmartPtr<Party> p:parties)
			{
				p->setTotalVotes(0);
				p->setRepresentativesSum(0);
			}
			for (District*d:districts)
			{

				d->updatePartiesResults();
				
			}
			
			auto partySortCompare = [this](const SmartPtr<Party>&p1, const SmartPtr<Party>&p2)
			{
				return partyCompare(p1, p2);
			};
			std::sort(parties.begin(), parties.end(), partySortCompare);
			require_update = false;
			
		}
	}
	

	void Elections::printResults()
	{
		if (districts.size() == 0)
		{
			std::cout << "THE SYSTEM IS EMPTY!!!" << std::endl;
			return;
		}
		
		std::cout << "**********Districts current resesults*************" << std::endl;
		size_t i = 0;
		for(District*d:districts)
		{
			
			d->showResults();
			if (i++ != districts.size() - 1)
				cout << "__________________________________________" << endl;
		}

		std::cout << "**********Parties summery results*************" << std::endl;
		if (parties.size() == 0)
		{
			std::cout << "NO PARTIES IN THE SYSTEM!!!" << std::endl;
			return;
		}
		i = 0;
		
		for (const SmartPtr<Party>& p:parties)
		{
			cout << "#" << p->getName() << endl;
			cout << "party id:" << p->getId() << endl;
			cout << "party leader:" << p->getLeader()->getName();
			if (i == 0)
				cout << " (Prime Minister)";
			cout << endl;

			cout << "Reprenstaitves sum:" << p->getRepresentativesSum() << endl;
			cout << "votes earned:" << p->getTotalVotes() << endl;
			if (i++ != parties.size() - 1)
				cout << "_______________________________" << endl;
			

		}

	}

	float Elections::getVotesPrecent()const
	{
		if (citizensDB.getCitizens().size() == 0)
			return 0;
		return (static_cast<float>(total_votes) / citizensDB.getCitizens().size())*100;
	}
	
	

	 bool Elections::partyCompare(const SmartPtr<Party>&p1, const SmartPtr<Party>&p2)const
	 {
		bool less= !((p2->getRepresentativesSum()
		> p1->getRepresentativesSum())
			 || (p1->getRepresentativesSum()
				 == p2->getRepresentativesSum() && p2->getId()
				 < p1->getId()));
		return less;
		
		 
		 
	 }
}