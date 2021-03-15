#pragma once
#include"NormalElections.h"
#include"SimpleElections.h"

namespace elections
{
	class ElectionsLoader
	{
	private:
		enum class ElectionsType
		{
			Simple,
			Normal
		};
	public:
	static	void save(Elections*e, std::ostream&out);
		 static	Elections*load(std::istream&in);

	};
}

