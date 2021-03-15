#pragma once
#include<string>
#include<exception>
#include<iostream>
#include"Exceptions.h"
#include<vector>
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>
 
namespace FILEIO
{
	//the functions get basic types of elemnts int,float,char...
	//and a file out and save all of them to the file
	template<class T>
	static void save( std::initializer_list<T> list, std::ostream&out)
	{
		if (!out)
			throw IOException("bad file");
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{
			 T data = *itr;
			if (!out.write(rcastc(&data), sizeof(T)).good())
				throw IOException("FILEIO error");
		}
	}
	//the functions get basic types of pointers to int,float,char...
	//and a file in and load all of them the file
	template<class T>
	static void load( std::initializer_list<T*>list, std::istream&in)
	{
		if (!in)

			throw IOException("bad file");
		for (auto itr = list.begin(); itr != list.end(); ++itr)
		{

			if (!in.read(rcastc(*itr), sizeof(**itr)).good())
				throw IOException("FILEIO error");
		}

	}
	//class for load and save strings
	class StringLoader
	{
	private:
		std::string &str;
	public:
		StringLoader(std::string&str)
			:str(str)
		{

		}
		void save(std::ostream&out)
		{
			if (!out)
		  throw IOException("bad file");
			FILEIO::save({ str.length() }, out);
			if (str.length())
			{
				const char*c_str = str.c_str();
				if(!out.write(c_str,sizeof(char)*str.length()).good())
					throw IOException("std::string write error");
			}
			

			
		}
		void load(std::istream&in)
		{
			if (!in)
				throw IOException("bad file");
			str = "";
			int size;
			FILEIO::load({ &size }, in);
			if (size>0)
			{
				for (int i = 0; i < size; i++)
				{
					char c;
					FILEIO::load({ &c }, in);
					str += c;
				}
			}


		}
	};

	
}
