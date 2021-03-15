#pragma once
#include"District.h"
namespace elections
{
	class UnifromDistrict:public District
	{
	protected:
	virtual void showWinnersResults()override;
	virtual void print(std::ostream&out)const override;
	public:
		UnifromDistrict(const std::string&name, int rep_number);
		UnifromDistrict(std::istream&in):District(in){}
		UnifromDistrict()=default;
		virtual void updatePartiesResults()override;
		//operators
		void operator=(const UnifromDistrict&other) = delete;
		
	};
}
