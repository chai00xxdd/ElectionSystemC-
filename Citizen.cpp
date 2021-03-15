#include "Citizen.h"
#include"District.h"
#include"Party.h"
#include"FILEIO.h"
#include"Exceptions.h"

namespace elections
{
	//IdValidation()(id) 
	Citizen::Citizen(const std::string &name, int id, int birth)
		:Entity(id,name), birth(birth), district(nullptr)
		, party(nullptr), voted(false)
	{
		Date::CheckYear(birth);
	}
	Citizen::Citizen()
		: Citizen("?", -1, -1)	
	{

	}
   
	void Citizen::leaveDistrict()
	{
		bool success=false;
		if (district)
		{
			const_cast<District*>(district)->removeCitizen(id);
			setDistrict(nullptr);
		}
			
	
	}
	void Citizen::setDistrict(const District*district)
	{
		
		(this->district) = district;
		
	}

	
	void Citizen::setBirth(int birth)
	{
		Date::CheckYear(birth);
		this->birth = birth;

	}
	void Citizen::joinParty( SmartPtr<Party> party,const District*district)
	{
		if (party == nullptr || district == nullptr || this->party != nullptr)
			throw NullException("Citizen::joinParty Recvied null argument");
		
		party->addMember(this, district);
		setParty(party);
	
	}
	void Citizen::setParty(const SmartPtr<Party>& party)
	{
		this->party = party;
		
	}
	void Citizen::joinDistrict(District*district)
	{
		if (district == nullptr)
			throw NullException("Citizen::joinDistrict nullpointer argument");
		if (this->district)//alredy in district
		{
			return;
		}
		 district->addCitizen(this);
		
	}

	std::ostream& operator<<(std::ostream&out, const Citizen&c)
	{
		
		out << "[name:" << c.getName() << " ,id:" << c.getId() << " ,born:" << c.getBirth();

		if (c.getDistrict() != nullptr)
		{
			out << " ,district id:" << c.getDistrict()->getId();
			out << ",district name:" << c.getDistrict()->getName();
		}
		else out << ",NO DISTRICT!!!";
		out << "]";
		return out;

	}

	void Citizen::vote(int partyId)
	{
		if (district == nullptr)
			return;
		 const_cast<District*&>(district)->vote(*this, partyId);

	}
	
	void Citizen::leaveParty()
	{
		if (party != nullptr)
			(party)->removeMember(id);
		if (party->getLeader() && *party->getLeader() == *this)
		{
			party->setLeader(nullptr);
		}
		party = SmartPtr<Party>();//nullptr
		
	}

	void Citizen::save(std::ostream&out)
	{
		try
		{
			Entity::save(out);
			FILEIO::save({ birth }, out);
			FILEIO::save({ voted }, out);
		}
		catch (...)
		{
			throw IOException("IO Exception Citizen");
		}
	}
	void Citizen::load(std::istream&in)
	{
		try
		{
			Entity::load(in);
			FILEIO::load({ &birth }, in);
			FILEIO::load({ &voted }, in);
		}
		catch (...)
		{
			
			throw IOException("IO Exception Citizen");
		}
		

	}

	Citizen::Citizen(std::istream&in)
	{
		load(in);
	}

	bool Citizen::operator==(const Entity&other)const 
	{
		return typeid(other) == typeid(Citizen)
			&& Entity::operator==(other);
	}

	


	
	

	

}


