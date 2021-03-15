#include "CitizensBook.h"
#include"Exceptions.h"
#include<algorithm>
#include"Templates.h"
#include"Party.h"
#include<string>
namespace elections
{
	

	std::ostream& operator<<(std::ostream&out, const CitizensBook&b)
	{
		size_t i = 1;
		for (const Citizen*c : b.citizens)
		{
			out << (i++) << "." << *c << std::endl;
		}
		return out;
	}

	const std::vector<SmartPtr<Citizen>>& CitizensBook::findByName(const std::string&name)const
	{
		search_results.clear();
		for (SmartPtr<Citizen> c : citizens)
			if (c->getName() == name)
				search_results.push_back(c);
		return search_results;
	}

	std::vector<SmartPtr<Citizen>>&CitizensBook::findByName(const std::string&name)
	{
		const CitizensBook&thisConst = *this;
		return const_cast<std::vector<SmartPtr<Citizen>>&>(thisConst.findByName(name));
	}
	SmartPtr<Citizen> CitizensBook::find(int id)
	{
		const CitizensBook&constThis = *this;
		return const_cast<SmartPtr<Citizen>&>(constThis.find(id));
	}
	const SmartPtr<Citizen>& CitizensBook::find(int id)const
	{
		auto citizen = citizens_map.find(id);
		if (citizen == citizens_map.end())
			throw std::invalid_argument("citizen with id "+std::to_string(id)+" doesnt exists");
		return  citizen->second;

	}
	const std::vector<SmartPtr<Citizen>>& CitizensBook::findByBirthYear(int birth)const
	{
		search_results.clear();
		for (SmartPtr<Citizen> c : citizens)
			if (c->getBirth() == birth)
				search_results.push_back(c);
		return search_results;
	}
	std::vector<SmartPtr<Citizen>>& CitizensBook::findByDate(int birth)
	{
		const CitizensBook&thisConst = *this;
		return const_cast<std::vector<SmartPtr<Citizen>>&>(thisConst.findByBirthYear(birth));
	}

	void CitizensBook::removeById(int id)
	{
		citizens_map.erase(id);
		auto citizen = std::find_if(citizens.begin(), citizens.end(), findPtrById(id));
		if(citizen!=citizens.end())
		citizens.erase(citizen);
	}

	void CitizensBook::addCitizen(const Citizen&c)
	{
		
		if (isExists(c.getId()))
		{
			throw std::logic_error(std::string("citizen with id: ")+
				std::to_string(c.getId())+" alredy exists");
		}
		try
		{
			citizens.push_back(new Citizen(c));
			citizens_map.insert({ c.getId(),citizens.back() });
		}
		catch (std::bad_alloc&e)
		{
			throw MemoryException("memory error CitizensBook :" + std::string(e.what()));
		}

		
	}
	void CitizensBook::clear()
	{
		citizens_map.clear();
		citizens.clear();
	}

	 void CitizensBook::save(std::ostream&out)
	{
		 Templates::saveDS(citizens, out);
	}
	 void CitizensBook::load(std::istream&in)
	 {
		 clear();
		 Templates::loadDS(citizens, in);
		 for (const SmartPtr<Citizen>&c : citizens)
		 {
			 citizens_map.insert({ c->getId(),c });
		 }
	}

	bool CitizensBook::isExists(int citizenId)const
	{
		return citizens_map.find(citizenId) != citizens_map.end();
	}

	

	
}
