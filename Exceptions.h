#pragma once
#include<exception>
#include<string>
#include<iostream>
typedef std::runtime_error NullException, IOException;
typedef std::logic_error NoFoundException;

//same as bad_alloc but with
//string ctor
class MemoryException : public std::bad_alloc
{
private:
	std::string msg;
public:
	MemoryException(const std::string&msg) :msg(msg) {}
	virtual const char* what() const noexcept override
	{
		return msg.c_str();

	}
};

namespace VALIDATION
{
	//the class check if given id 
	//has the length of howManyDigits
	//otherwise throws exceptoin
	class IdValidation
	{
	private:
		int howManyDigits;
	public:
		IdValidation(int howManyDigits=9):howManyDigits(howManyDigits){}
		void operator()(int id)
		{
			std::string str = std::to_string(id);
			if (str.length() != howManyDigits)
				throw std::invalid_argument("bad id!!!! need to have "+std::to_string(howManyDigits)+" digits!!!");

		}

	};
	//the class check the given pointer is not class
	//if null throws exception
	class PointerValidator
	{
	public:
		template<class T>
		const T* operator()(const T*ptr)
		{
			if (ptr == nullptr)
				throw NullException("ptr is null!!");
			return ptr;
		}

		template<class T>
		 T* operator()( T*ptr)
		{
			 const PointerValidator&thisconst = *this;
			 return const_cast<T*>(thisconst(ptr));
		}
	};
	//the class check if given age
	//is in range of[min_age,max_age] if not throws excpetion
	class AgeValidator
	{
	private:
		int year;
		int min_age;
		int max_age;
	public:
		AgeValidator(int year,int min_age,int max_age=120):year(year), min_age(min_age),max_age(max_age){}
		void operator()(int born)
		{
			if ((year - born) < min_age|| year-born>max_age)
			throw std::invalid_argument(std::string("age need to between ") + 
				std::to_string(min_age)+"-"+std::to_string(max_age) +" age given is "
				+std::to_string(year-born)+" when  the current year is:"
			   +std::to_string(year));
		}
	public:
	};
}