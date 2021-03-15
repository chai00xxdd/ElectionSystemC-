#pragma once
#include<string>
#include"Exceptions.h"
#include<iostream>
namespace elections
{
	

	template<class T>
	class SmartPtr
	{
	private:
		int *counter = nullptr;
		T* ptr=nullptr;
	public:
		SmartPtr() = default;//nullptr
		SmartPtr(const SmartPtr&other)
		{
			*this = other;
		}
		 SmartPtr(T*ptr)
			:ptr(ptr)
		{

			 if (ptr)
			 {
				 try
				 {
					 counter = new int(1);
				 }
				 catch (...)
				 {
					 delete counter;
					 delete ptr;
					 throw MemoryException("memory exception in SmartPtr ctor");
				 }
			 }
		}
		~SmartPtr()
		{
			release();
		}
		void release()
		{
			if (counter)
			{
				(*counter)--;
				if ((*counter) == 0)
				{
					delete counter;
					delete ptr;
				}
			}
			counter = nullptr;
			ptr = nullptr;
		}
		  SmartPtr& operator=(const SmartPtr&other)
		{
			if (this == &other)
				return *this;
			release();
			ptr = other.ptr;
			counter = other.counter;
			if (counter)
			{
				(*counter)++;
			}
			return *this;
		}

		const T&operator*()const 
		{ 
			if (*this == nullptr)
				throw NullException("smart ptr is nullptr!!!");
			return *ptr; 
		
		}
		T&operator*() 
		{ 
			const SmartPtr<T>&constThis = *this;
			return const_cast<T&>(*constThis);
		}
		const T*operator->()const
		{
			if (*this == nullptr)
				throw NullException("smart ptr is nullptr!!!");
			return ptr;
		}
		T*operator->() 
		{
			if (*this == nullptr)
				throw NullException("smart ptr is nullptr!!!");
			return ptr;;
		}

		bool operator!()const { return counter != nullptr; }
		bool operator==(const SmartPtr&other)const
		{
			return ptr == other.ptr;
		}
		bool operator!=(const SmartPtr&other)const
		{
			return ptr != other.ptr;
		}
		bool operator!=(const T*other_ptr)const
		{
			return ptr != other_ptr;
		}
		bool operator==(const T*other_ptr)const
		{
			return ptr == other_ptr;
		}
		operator bool()const { return counter != nullptr; }
		operator const T* ()const { return ptr; }
		operator T*() { return ptr; }
		

		 void save(std::ostream&out)
		{
			 try
			 {
				 if (ptr)
					 ptr->save(out);
			 }
			 catch (IOException&e)
			 {
				 throw IOException(std::string(e.what()) + " error in SmartPtr load");
			 }

		}
		 void load(std::istream&in)
		{
			release();
			try
			{
				ptr = new T(in);
				counter = new int(1);
			}
			catch (std::bad_alloc&e)
			{
				delete counter;
				ptr = nullptr;
				counter = nullptr;
				throw MemoryException(std::string(e.what()) + " error in SmartPtr load");
			}
			catch (IOException&e)
			{
				ptr = nullptr;
				counter = nullptr;
				throw IOException(std::string(e.what()) + " error in SmartPtr load");
			}
		}


	
	};






}
