#pragma once
#include<vector>
#include<map>
#include<unordered_map>
#include"FILEIO.h"
#include<algorithm>
namespace Templates
{
	//save a data struct vector,list...
	//of pointers to the file out
	template<class T> 
	void savePtrDS(T&ds,std::ostream&out)
	{
		int log = ds.size();
		FILEIO::save({ log }, out);
		for (auto itr = ds.begin(); itr != ds.end(); ++itr)
		{
		
		  (*itr)->save(out);
		}
	}
	//load a data struct vector, list...
		//of pointers to ds
	template<class PtrOf,class T>
	void loadPtrDS(T&ds,std::istream&in)
	{
		ClearPtrDS(ds);
		int log;
		FILEIO::load({ &log }, in);
	
		if (log > 0)
		{
			int i = 0;
			try
			{
				ds.resize(log);
				applyAll(ds, [](PtrOf*&p) {p = nullptr; });
				for (auto itr = ds.begin(); itr != ds.end(); ++itr)
				{
					*itr = new PtrOf(in);
				
				}
			}
			catch (...)//memory error or io error
			{
				ClearPtrDS(ds);//free pointers
				throw;
			}
			
		}

	}
	/*
	the function get a data struct of pointers
	and frees all the pointers and clear the struct
	*/
	template<class T>
	void ClearPtrDS(T&ds)
	{
		for (auto itr = ds.begin(); itr != ds.end(); ++itr)

			delete *itr;
		ds.clear();
	}
	template< class T,class S>
	void ClearPtrDS(std::map<T,S>&ds)
	{
		for (const auto&pair:ds)
			if (pair.second)
				delete pair.second;
		ds.clear();
	}
	template<class T,class FUNC>
	//the functoin get a data struct ds
	//and loop through all elements and use the object function fp
	//on them
	void applyAll( T&ds,const FUNC&fp)
	{
		for (auto itr = ds.begin(); itr != ds.end(); ++itr)
			fp(*itr);
	}
	template<class T> 
	//the function get a data struct(vector,list...)
	//when all the elements have save(std::istream) function
	//and the functoin save all the elemntion to file out
	 void saveDS(T&ds, std::ostream&out)
	{
		 int log = ds.size();
		 FILEIO::save({ log }, out);
		 for (auto itr = ds.begin(); itr != ds.end(); itr++)
			 (*itr).save(out);
	}
	 template< class D>
	 void saveDS(std::map<int, D>&map,std::ostream&out)
	 {
		 FILEIO::save({ map.size() }, out);
		 for (auto & pair : map)
		 {
			 FILEIO::save({ pair->first }, in);
			 pair->second.save(out);
		 }

	 }
	 template< class D>
	 //the function get a data struct(vector,list...)
	//when all the elements have load(std::istream) function
	//and the functoin load all the elemntion to ds
	 void loadDS(std::map<int, D>&map, std::istream&in)
	 {
		 int size;
		 FILEIO::load({ &size },in);
		for(int i=0; i<size; i++)
		{
			 int key;
			 FILEIO::load({ &key }, in);
			 D d(in);
			 pair->second.save(out);
			 map.insert({ key,d });
		 }

	 }
	 template<class T>
	 void loadDS(T&ds, std::istream&in)
	 {
		 ds.clear();
		 int size;
		 FILEIO::load({ &size },in);
		 ds.resize(size);
		 for (auto itr = ds.begin(); itr != ds.end(); itr++)
		 {
			 try
			 {
				 (*itr).load(in);
			 }
			 catch (...)
			 {
				 
				 ds.clear();
				 throw;
			 }
		 }
	 }

	 //the function get a data struct(vector,list...)
	 //and return and find the first element that findBy return true on him
	 //else return ds.end()
	 template<class T,class findMethod>
	 auto Find(const T&ds,const findMethod&findBy)
	 {
		 return std::find_if(ds.begin(), ds.end(), findBy);
	 }



	
	 template<class T,class FP>
	 T Transform(const T&object, const FP&fp)
	 {
		 T t=object;
		 std::transform(object.begin(), object.end(), t.begin(), fp);
		 return t;

	 }

	 template<class T>
	 void Sort(T&ds, bool ascending = true)
	 {
		 std::sort(ds.begin(),ds.end());
		 if (!ascending)
			 std::reverse(ds.begin(), ds.end());
	 }
	 template<class T,class CMP>
	 void Sort(T&ds,const CMP&compare,bool ascending = true)
	 {
		 std::sort(ds.begin(), ds.end(),compare);
		 if (!ascending)
			 std::reverse(ds.begin(), ds.end());
	 }

	

	
	 
	 
}

	class findById
	{
	private:int id;
	public:
		findById(int id) :id(id) {}
		template<class T>
		bool operator()(const T&obj) { return obj.getId() == id; }
	};
	class findPtrById
	{
	private:
		int id;
	public:
		findPtrById(int id) :id(id) {}
		template<class T>
		bool operator()(const T& obj) { return findById(id)(*obj); }
	};
