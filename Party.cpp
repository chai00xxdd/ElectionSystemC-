#include "Party.h"
#include"District.h"
#include"Exceptions.h"
#include"FILEIO.h"
#include<algorithm>
#include"Templates.h"
namespace elections
{
	int Party::ID_GENERATOR = 1;

	

	Party::Party(const std::string& name, const Citizen*leader)
		:Entity(ID_GENERATOR++, name), leader(leader), totalVotes(0), representativesSum(0)
	{

	}
	Party::Party(std::istream&in)
		:Party()
	{
		load(in);
	}
	void Party::setRepresentativesSum(int rep)
	{
		this->representativesSum = rep;
		
	}

	std::ostream& operator<<(std::ostream& out, const Party&p)
	{
		out << "party name:" << p.getName() << std::endl;
		out << "party id:" << p.getId() << std::endl;
		out << "party leader:";
		
		if(p.getLeader()!=nullptr)
		out<< p.getLeader()->getName() << " [id:" << p.getLeader()->getId() << "]" << std::endl;
		else out << "NONE!!!"<<std::endl;
		for (const auto &pair: p.getAllMembers())
		{
			const Representatives&r = pair.second;
			out << r << std::endl;
			
		}
		if (p.getAllMembers().size() == 0)
			out << "NO MEMBERS IN PARTY!!!" << std::endl;

		return out;
	}

	void Party::addRep(int rep)
	{

		this->representativesSum += rep;
	}
	

	void Party::addvotes(int votes)
	{
		this->totalVotes += votes;

	}

	void Party::setTotalVotes(int total)
	{
		this->totalVotes = total;
		
	}


	void Party::setLeader(const Citizen*leader)
	{
		this->leader = leader;
		
	}

	
	void Party::save(std::ostream&out)
	{
		
		try
		{
			Entity::save(out);
			FILEIO::save({ totalVotes,representativesSum }, out);
		}
		catch (...)
		{
			throw IOException("Party Save Error");
		}
	}
	void Party::load(std::istream&in)
	{
		try
		{
			Entity::load(in);
			FILEIO::load({ &totalVotes,&representativesSum }, in);
		}
		catch (...)
		{
			throw IOException("Party Load Error");
		}
		
	}
	

	void Party::loadState(std::istream&in)
	{
		if (!in)
			throw IOException("bad file");
		int prev = ID_GENERATOR;
		FILEIO::load({ &ID_GENERATOR }, in);
		if (ID_GENERATOR < 1)
		{
				ID_GENERATOR = prev;
				throw IOException("party LoadState file Corrupted");
		}
		
		
	}
	void Party::saveState(std::ostream&out)
	{
		if (!out)
			throw IOException("bad file");
	
		FILEIO::save({ ID_GENERATOR }, out);
	}

	bool Party::operator==(const Entity&other)const
	{
		return typeid(other) == typeid(Party)
			&& Entity::operator==(other);
	}

	void Party::addMember(const Citizen*c, const District*district)
	{
		if (c == nullptr || district == nullptr)
			throw NullException("nullpointer addMember party");
		if (c->getParty())
			throw std::logic_error("Citizen alredy belong to a party!!!");
		try
		{
			auto&membersAtDistrict = getMembersAt(district->getId());
			membersAtDistrict.push_back(const_cast<Citizen*>(c));
		}
		catch (NoFoundException&)
		{
			members[district->getId()] = Representatives{ district };
			members[district->getId()].getMembers().push_back(const_cast<Citizen*>(c));
		}
		

	}
	bool Party::isExists(const Citizen&c)const
	{
		for (auto const & pair : members)
		{
			const Representatives&r = pair.second;
			if (r.isExists(c))
				return true;
		}
		return false;

	}

	const std::vector<Citizen*>&Party::getMembersAt(int district_id)const
	{
		
		auto represntivesInDistrict = members.find(district_id);
		if (represntivesInDistrict == members.end())
			throw NoFoundException(std::string("members at district ") 
				+ std::to_string(district_id) + " doesnt exists in party!!!");
		return represntivesInDistrict->second.getMembers();
	}
	std::vector<Citizen*>& Party::getMembersAt(int district_id)
	{
		const Party&thisconst = *this;
		return const_cast<std::vector<Citizen*>&>(thisconst.getMembersAt(district_id));
	}

	void Party::removeMember(int id)
	{
		for (auto &pair : members)
		{
			Representatives &r = pair.second;
			auto memberToRemove = Templates::Find(r.getMembers(), findPtrById(id));
			if (memberToRemove != r.getMembers().end())
			{
				r.getMembers().erase(memberToRemove);
				break;
			}
			
		}
	}
	

}


