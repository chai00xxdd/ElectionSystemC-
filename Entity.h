#pragma once
#include<string>
#include<iostream>
#include<string>
class Entity
{
protected:
	std::string name;
	const int id;
	int index = -1;//help to save to files 


public:
	Entity(int id, const std::string&name):id(id),name(name){}
	Entity() :name(""), id(-1) {}
	
	virtual ~Entity() {}
	const std::string&getName()const { return name; }
	int getId()const { return id; }
	int getIndex()const { return index; }
	void setName(const std::string&name) { this->name = name;  }
	 void setIndex(int index) { this->index = index; }
	virtual bool operator==(const Entity&other)const { return id == other.id; }
	virtual void save(std::ostream&out);
	virtual void load(std::istream&out);
};

