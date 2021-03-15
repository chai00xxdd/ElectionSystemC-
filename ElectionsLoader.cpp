#include "ElectionsLoader.h"
#include<typeinfo>
#include"FILEIO.h"
#include"Exceptions.h"
namespace elections
{
		void ElectionsLoader::save(Elections*e, std::ostream&out)
	{
			if (e == nullptr)
				throw NullException("ElectionLoader recived nullptr object");
			if (!out)
				throw IOException("bad file");
			ElectionsType type = ElectionsType::Normal;
			if (typeid(*e) == typeid(SimpleElections))
			{
				type = ElectionsType::Simple;
			}
			if (!out.write(rcastcc(&type), sizeof(type)).good())
				throw IOException("IO error ElectionLoader");
			 e->save(out);

	}
		Elections*ElectionsLoader::load(std::istream&in)
	{

			if (!in)
				throw IOException("bad File ElectionsLoader");
			ElectionsType type = ElectionsType::Normal;
			FILEIO::load({ &type }, in);
			try
			{
				switch (type)
				{
				case ElectionsType::Simple:
					return new SimpleElections(in);
					break;
				case ElectionsType::Normal:
					return new NormalElections(in);
					break;
				default:
					throw IOException("failed to load Elections");
					break;
				}
			}
			catch (std::bad_alloc&)
			{
				throw MemoryException("ElectionLoader::load memory exception");
			}
			
			
		
	}
}
