#include "Entity.h"
#include"FILEIO.h"
#include"Exceptions.h"
 void Entity::save(std::ostream&out)
{
	 FILEIO::save({ id }, out);
	 FILEIO::save({ index }, out);
	 FILEIO::StringLoader(name).save(out);
	 
}
 void Entity::load(std::istream&in)
{
	 int*idPointer = const_cast<int*>(&id);
	 FILEIO::load({ idPointer,&index }, in);
	 FILEIO::StringLoader(name).load(in);
	
	 
}