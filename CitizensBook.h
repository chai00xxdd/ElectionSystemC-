#pragma once
#include"Citizen.h"
#include<vector>
#include"SmartPointers.h"
#include<unordered_map>
namespace elections
{
	class District;
	class CitizensBook
	{
	private:
		std::unordered_map<int, SmartPtr<Citizen>>citizens_map;//fast search
		std::vector<SmartPtr<Citizen>> citizens;//fast iter
		mutable std::vector<SmartPtr<Citizen>> search_results;
		
	public:
		//the function add a new citizen to the DB
		void addCitizen(const Citizen&other);
		std::vector<SmartPtr<Citizen>>&getCitizens() { return citizens; }
		const std::vector<SmartPtr<Citizen>>&getCitizens()const { return citizens; }

		 //book find functions 
		//find the index of the citizen with citzenId 
		//if not found  thorw exeptoin
		SmartPtr<Citizen> find(int id);
		const SmartPtr<Citizen>& find(int id)const;
		
		//return a citziensArray of pointers to citizens to all citizens
		//in the book that have the name searched
		const std::vector<SmartPtr<Citizen>>&findByName(const std::string&name)const;
		std::vector<SmartPtr<Citizen>>&findByName(const std::string&name);

		//return a citziensArray of pointers to citizens to all citizens
		//in the book that have the date searched
		const std::vector<SmartPtr<Citizen>>&findByBirthYear(int birth)const;
		std::vector<SmartPtr<Citizen>>&findByDate(int birth);

		//return true if a citizen with citizenId
		//exists in the book otherwise reutrn false
		bool isExists(int citizenId)const;
		//remove a citizen that have the id
		void removeById(int id);
		//clear all the data
		void clear();

		 //operators
		 const Citizen&operator[](int index)const { return *citizens[index]; }
		 Citizen&operator[](int index) { return *citizens[index]; }
		 friend std::ostream& operator<<(std::ostream&, const CitizensBook&);

		 void save(std::ostream&out);
		 void load(std::istream&in);

		 


	};
	std::ostream& operator<<(std::ostream&, const CitizensBook&);
	
	

}