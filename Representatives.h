#pragma once

#include"Citizen.h"
#include"SmartPointers.h"
#include<vector>
#include<iostream>
#include"Exceptions.h"
using namespace VALIDATION;
namespace elections
{
	class District;
	class Representatives
	{
	private:
		
		const District*district=nullptr;
		std::vector<Citizen*> members;
	public:
		Representatives(const District*district);
		Representatives() = default;
		Representatives(const Representatives&other) = default;
		const std::vector<Citizen*>&getMembers()const { return members; }
		std::vector<Citizen*>&getMembers() { return members; }
		const District&getDistrcit()const { return *PointerValidator()(district); }
		bool isExists(const Citizen&c)const;
		int getId()const;
		friend std::ostream& operator<<(std::ostream&out, const Representatives&other);
	};	
}

