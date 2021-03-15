#pragma once
#include<iostream>
#include"Exceptions.h"
class Date
{
private:
	int day, month, year;
	
public:
	Date();
	Date(int day, int month, int year);
	//setters
	void set(int day, int month, int year);
	void setDay(int day);
	void setYear(int year);
	void setMonth(int month);
	//getters
	int getDay()const { return day; }
	int getMonth()const { return month; }
	int getYear()const { return year; }

	//validation check
	bool isValid()const;
	//operators
	bool operator==(const Date&other)const;
	friend std::ostream& operator<<(std::ostream&, const Date&d);
	friend std::istream& operator>>(std::istream&, Date&);

	//IO
	void save(std::ostream&out);
	void load(std::istream&in);
	//const
	static constexpr int MINIMUM_YEAR =0;
	//
	static void CheckYear(int year)
	{
		if (year <= MINIMUM_YEAR)
			throw std::invalid_argument(std::string("year must be atlist ")
				+ std::to_string(MINIMUM_YEAR+1)+"!!!");
	}
};
std::ostream& operator<<(std::ostream&, const Date&d);
std::istream& operator>>(std::istream&, Date&);
