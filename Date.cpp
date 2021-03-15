#include "Date.h"
#include"FILEIO.h"
#include"Exceptions.h"
//ctor

Date::Date()
:day(-1),month(-1),year(-1)
{

}
Date::Date(int day, int month, int year)
	
{
	set(day, month, year);
}

//setters
void Date::set(int day, int month, int year)
{
	setDay(day);
	setMonth(month);
	setYear(year);

}
void Date::setDay(int day)
{
	if (day >= 1 && day <= 31)
	{
		this->day = day;
		
	}
	else
	throw std::logic_error("day must be in range of [1,31]!!!");
	
}
void Date::setYear(int year)
{
	CheckYear(year);
	this->year = year;

}
void Date::setMonth(int month)
{
	if (month >= 1 && month <= 12)
	{
		this->month = month;
		
	}
	else
	throw std::logic_error("month must be in range of [1,12]!!!!");
}

//IO
std::istream& operator>>(std::istream&in, Date&d)
{
	int day, month, year;
	in >> day >> month >> year;
	d.set(day, month, year);
	return in;

}
std::ostream& operator<<(std::ostream&out, const Date&d)
{
	out << d.getDay() << "." << d.getMonth() << "." << d.getYear();
	return out;
}
//valid
bool Date::isValid()const
{
	return day >= 1 && day <= 31 && month >= 1 && month <= 12
		&& year >= MINIMUM_YEAR;
}

bool Date::operator==(const Date&other)const
{
	return day == other.day
		&&month == other.month
		&&year == other.year;
}

//IO

void Date::save(std::ostream&out)
{
	FILEIO::save({ day,month,year }, out);

}
void Date::load(std::istream&in)
{
	FILEIO::load({ &day,&month,&year },in);
}