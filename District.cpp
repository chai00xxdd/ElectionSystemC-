#include "District.h"
#include"CitizensBook.h"
#include"Party.h"
#include<iostream>
#include"FILEIO.h"
#include<string>
#include"Exceptions.h"
#include"Templates.h"
#include<algorithm>
using namespace std;
//ctors dtors
namespace elections
{
	int District::ID_GENRATOR = 1;

	//ctors dtors
	District::District(const std::string&name, int rep)
		:Entity(ID_GENRATOR++,name), representative_number(rep), voting_system(rep)
		, supervise(true)

	{
		setRepresentativeNumber(rep);//for exception
	}
    District::District(std::istream&in)
		 :voting_system(0)
	{
		load(in);
	}
    District::District ()//default constructor for loading
		:voting_system(0),Entity(-1,"EMPTY"),representative_number(0)
	{

	}
   

	//not allowing full copy
	District::District(const District&other)
		:Entity(other.id, other.name),representative_number(other.representative_number)
		,voting_system(representative_number)
		,supervise(other.supervise)
	{
		
	}
	
	//getters setters

	float District::getVotingPrecent()const
	{
		float div = citizens.size() ? static_cast<float>(citizens.size()) : 1.0f;
		return voting_system.getTotalVotes() / div;


	}
	const VotingSystem& District::getVotingSystem()const
	{
		District&thisref = const_cast<District&>(*this);
		return thisref.getVotingSystem();
	}
	VotingSystem&District::getVotingSystem()
	{
		voting_system.updateResults();
		return voting_system;
	}
	
	void District::setSupervise(bool supervise)
	{
		this->supervise = supervise;
		;
	}
	
	void District::setRepresentativeNumber(int rep)
	{
		if (rep < 1)
			throw std::invalid_argument("rep_num must be atlist 1!!!!");
		this->representative_number = rep;
		

	}

	


	
	void District::addCitizen(Citizen*c)
	{
		if (c == nullptr)
			throw NullException("Citizen is nullptr in addDistrict");
		if (isSupervisor() &&c->getDistrict()==this)
		{
		throw std::logic_error(std::string("citizen with id:") + std::to_string(c->getId()) 
			+ " alredy in the district "+std::to_string(id));
		  
		}
		else if (isSupervisor()&& c->getDistrict())
		{
		throw std::logic_error(std::string("citizen with id:") + std::to_string(c->getId()) + " alredy belong to another district");
		}
		else
		{
			citizens.push_back(c);
			c->setDistrict(this);
			
		}
		
	}

	void District::vote(int citizenId, int partyId)
	{
		auto citizen = std::find_if(citizens.begin(), citizens.end(), findPtrById(citizenId));
		if (citizen==citizens.end())
		{
		throw NoFoundException(std::string("citizen ") + std::to_string(citizenId) 
			+ " doesnt exists in district " +std::to_string(id));
			
		}
		Citizen &c = **citizen;
		if (c.isvoted())
		{
			throw std::logic_error("citizen alredy voted");
		}
		voting_system.vote(partyId);
		c.setVoted();	
		
	}
	void District::showResults()
	{
		showResultsTitle();
		if (getVotingSystem().getTotalVotes() == 0)
		{
			return;
		}
		showWinnersResults();
		showPartiesResults();
	}

	void District::vote(Citizen&c, int partyId)
	{
			
		if (c.getDistrict()!=this)//theta(1) check 
			throw NoFoundException(std::string("citizen ") + 
				std::to_string(c.getId()) + " doesnt exists in district " + std::to_string(id));
		if (c.isvoted())
			throw std::logic_error(std::string("citizen ")+std::to_string(c.getId())+ "alredy voted");
				voting_system.vote(partyId);
				c.setVoted();
			
		
	}

	void District::removeCitizen(int citizenId)
	{
	
		auto citizen = std::find_if(citizens.begin(), citizens.end(), findPtrById(citizenId));
		if (citizen == citizens.end())
		{
			throw NoFoundException(std::string("citizen ") + std::to_string(citizenId)
				+ " doesnt exists in district " + std::to_string(id));

		}
		    Citizen&c = **citizen;
			c.setDistrict(nullptr);
			citizens.erase(citizen);
		
		
			
	}
	//operators

	std::ostream& operator<<(std::ostream&out, const District&d)
	{

		d.print(out);

		return out;
	}
	void District::print(std::ostream&out)const
	{
		out << "id:" << getId() << " name:" << getName() << " reprensatives number:"
			<< getRepresentativeNumber();;
	}

	

	 void District::showResultsTitle()
	{
		 const VotingSystem&d_results = getVotingSystem();
		 cout << "#" << getName() << std::endl;;
		 cout << "district name:" << getName() << endl;
		 cout << "district id:" << getId() << endl;
		 cout << "reprenstaive number:" << getRepresentativeNumber() << endl;
		 cout << "ppl voted:" << d_results.getTotalVotes() << "/"
			 << getCitizens().size() << endl;
		 cout << "vote precent:" << getVotingPrecent() * 100 << "%" << endl;
	}

	 void District::showPartiesResults()
	 {
		auto&results = getVotingSystem().getResults();
		 cout << "!!!PARTIES RESULSTS IN DISTRICT " << getId()
			 << "!!!" << endl;
		 bool first = true;
		 for (const auto& p_results_ptr : results)
		 {
			 const VotingSystem::partyResults& p_results = *p_results_ptr;
			 if (p_results.getParty() == nullptr)
			 {
				 std::cout << "party is null!!!! in district " << std::endl;
				 continue;
			 }
			 cout << "#" << getName() << "::" << p_results.getParty()->getName();
			 if (first)
				 cout << " (leading result)";
			 first = false;
			 cout << endl;
			 cout << "party name:" << p_results.getParty()->getName() << endl;
			 cout << "party id:" << p_results.getParty()->getId() << endl;
			 cout << "votes:" << p_results.getTotalVotes() << "/" << voting_system.getTotalVotes() << endl;
			 cout << "vote precent:" << p_results.getVotePrecent() * 100 << "%" << endl;
			 cout << "party leader:" << p_results.getParty()->getLeader()->getName();
			 cout << endl;
			 cout << "$Repenstaives allocated:" << p_results.getRepresentativeEarned()
				 << "/" << getRepresentativeNumber() << endl;
			 cout << "$Repenstaives List:";
			 try
			 {
			 cout << endl;
		     const std::vector<Citizen*>&earned_list = p_results.getParty()->getMembersAt(id);
			 for (size_t i = 0; i < earned_list.size() && i < static_cast<size_t>(p_results.getRepresentativeEarned()); i++)
			 {

				 cout << i + 1 << "." << *earned_list[i] << endl;
			 }
			 int diff = p_results.getRepresentativeEarned() - earned_list.size();
			 if (diff > 0)
				 cout << "PARTY MISSING " << diff << " REPRESENTATIVES FROM LIST!!!!!!!!" << endl;
		 }
				 catch (...)
				 {
				     cout << "none" << endl;
				 }
			 cout << endl;
		 }
	 }


	 
	 void District::save(std::ostream&out)
	 {
		 if (!out)
			 throw IOException("bad file");
		 try
		 {
			 
			 Entity::save(out);
			 FILEIO::save({ representative_number }, out);
			 voting_system.save(out);
		 }
		 catch (...)
		 {
			 throw IOException("District save Error");
		 }
		
	 }
	 void District::load(std::istream&in)
	 {
		 if (!in)
			 throw IOException("bad file");
		 try
		 {
			 
			 Entity::load(in);
			 FILEIO::load({ &representative_number }, in);
			 voting_system.load(in);
		 }
		 catch (...)
		 {
			 throw IOException("District Load Error");
		 }
		
	 }

	  void District::loadState(std::istream&in)
	 {
		  if (!in)
			  throw IOException("bad file");
		  int prev = ID_GENRATOR;
		  try
		  {
			  FILEIO::load({ &ID_GENRATOR }, in);
		  }
		  catch (...)
		  {
			  ID_GENRATOR = prev;
			  throw IOException("IO ERROR Distrcit LoadState");
		  }
		  if (ID_GENRATOR<1)
		  {
			  ID_GENRATOR = prev;
			  throw IOException("corrupted file Distrcit LoadState");
		  }
		  
	 }
	  void District::saveState(std::ostream&out)
	 {
		  if (!out)
			  throw IOException("bad file");
		  FILEIO::save({ ID_GENRATOR }, out);
	 }


	 

	
	

}





