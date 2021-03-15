#include"Representatives.h"
#include"District.h"
#include<algorithm>
#include"Templates.h"
namespace elections
{
	int Representatives::getId()const
	{
		return district->getId();
	}
   Representatives::Representatives (const District*district)
	   :district(district)
	{
	   if (district == nullptr)
		   throw NullException("Representatives ctor district given is nullptr");
	}

   std::ostream& operator<<(std::ostream&out, const Representatives&r)
   {
	   out << "@Members List in district "
		   << r.getId() << std::endl;
	   size_t i = 0;

	   for (Citizen*m : r.members)
	   {
		   out << (i++) + 1 << "." << *m<<std::endl;
	   }
	   return out;
   }
   bool Representatives::isExists(const Citizen&c)const
   {
	   auto search = std::find_if(members.begin(), members.end(), findPtrById(c.getId()));
	   return search != members.end();
   }
}