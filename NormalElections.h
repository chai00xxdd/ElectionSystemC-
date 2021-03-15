#pragma once

#include"Elections.h"
#include<string>
namespace elections
{
	class NormalElections:public Elections
	{
	private:
		std::string type = "Normal";
	public:
		 NormalElections()=default;
		 NormalElections(const Date&date);
		 NormalElections(const NormalElections&other) = delete;
		 NormalElections(std::istream&in):Elections(in){}
		virtual const std::string&getType()const override { return type; }
		//operators
		void operator=(const NormalElections&other) = delete;
		
	};
}

