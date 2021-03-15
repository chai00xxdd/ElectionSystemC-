#pragma once
#include"District.h"
#include<iostream>
#include"FILEIO.h"
#include"Templates.h"
#include<map>
namespace elections
{


	class DistrictLoader
	{
	private:
		enum class DistrictType
		{
			Unifrom,
			Divided
		};
	public:
		static void save(District*district,std::ostream&out);
		static District* load(std::istream&in);
		template<class T>
		static void saveDistricts(T&ds,std::ostream&out)
		{
			int size = ds.size();
			FILEIO::save({ size }, out);
			for (District* d:ds)
			{
				save(d, out);

			}
			
		}
		template<class T>
		static void LoadDistricts(T&ds, std::istream&in)
		{
			Templates::ClearPtrDS(ds);
			int size;
			FILEIO::load({ &size }, in);
			if(size>=0)
			ds.resize(size);
			for (District*&d : ds)
			{
				try
				{
					d = load(in);
				}
				catch (...)
				{
					Templates::ClearPtrDS(ds);
					throw;
				}
			}


		}
		

	};
}
