#include "DistrictLoader.h"
#include<typeinfo>
#include"DividedDistrict.h"
#include"UnifromDistrict.h"
#include"FILEIO.h"
#include"Exceptions.h"
namespace elections
{
	void DistrictLoader::save(District*district, std::ostream&out)
	{
		if (!out)
			throw IOException("bad file DistrictLoader");
		if (district == nullptr)
			throw NullException("district is nullptr!!! in districtLoader save");
		DistrictType type = DistrictType::Unifrom;
		if (typeid(*district) == typeid(UnifromDistrict))
		{
			type = DistrictType::Unifrom;
			FILEIO::save({ type }, out);
		}
		else//Divided
		{
			
			type = DistrictType::Divided;
			FILEIO::save({ type }, out);
		}
		
		district->save(out);
		
		
		
	}
	District* DistrictLoader::load(std::istream&in)
	{
		if (!in)
			throw IOException("bad file DistrictLoader");
		DistrictType type=DistrictType::Divided;
			FILEIO::load({ &type }, in);
			switch (type)
			{
			case DistrictType::Unifrom:
				return new UnifromDistrict(in);
				break;
			case DistrictType::Divided:
				return new DividedDistrict(in);
				break;
			default:
				throw IOException("corrupted file DistrictLoader");
				break;
			}
		
			return nullptr;
			
	}
}