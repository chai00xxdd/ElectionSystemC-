#pragma once
#include"District.h"
#include<iostream>
namespace elections
{
	class DividedDistrict:public District
	{
	protected:
	 virtual void showWinnersResults()override;
	 virtual void print(std::ostream&out)const override;
	public:
		DividedDistrict(const std::string&name, int rep_number);
		DividedDistrict() = default;
		DividedDistrict(std::istream&in):District(in){}
		virtual void updatePartiesResults()override;
	
		
		
	};

}

