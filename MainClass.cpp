#include "MainClass.h"
#include<iostream>
#include"DividedDistrict.h"
#include"UnifromDistrict.h"
#include"NormalElections.h"
#include"SimpleElections.h"
#include"DistrictLoader.h"
#include"ElectionsLoader.h"
#include"Templates.h"
#include<algorithm>
#include<fstream>
using namespace std;

Elections* MainClass::election_manager = nullptr;
std::vector<MainClass::SHORCUT> MainClass::shorcuts;
bool MainClass::once = false;
bool MainClass::alive = true;
 void MainClass::init()
{
	 election_manager = new NormalElections(Date(30,12,2020));
	 int d = 1995;
	 election_manager->addDistrict(new UnifromDistrict("A",15));
	 election_manager->addDistrict(new UnifromDistrict("B",10));
	 election_manager->addDistrict(new DividedDistrict("C",7));
	 for(int j=0; j<3; j++)                        

	 for (int i = 0; i < 100; i++)
	 {
		 std::string names[] = { "a","b","c" };
		 election_manager->addCitizen(Citizen(names[j]+to_string(i),j*100+i, d),j+1);
		
	 }
	 
	
	 
	 election_manager->addCitizen({ "Ayli",1000,d,},1);
	 election_manager->addCitizen({ "Doni",1001,d,},1);
	 election_manager->addParty("Ayor",1001);
	 election_manager->addParty("Dambo",1000);
	 
	
	 for (int i = 0; i < 50; i++)
	 {
		 election_manager->addPartyMember(i + 1, 1, i % 3 + 1);
		 election_manager->addPartyMember(51+i, 2, i % 3 + 1);
	 }
	 
	 int dambo = 2;
	 int ayor = 1;
	
	 //election_manager->showDistricts();
	
	 for (int i = 0; i < 100; i++)
	 {
		 int p=(i<60)?dambo:ayor;
		 election_manager->vote(i, p);
		

		
	 }
	 
	 for (int i = 100; i <150; i++)
	 {
		 int p = (i <125) ? dambo : ayor;
		 election_manager->vote(i, p);
	 }
	 for (int i = 200; i < 290; i++)
	 {
		 int p = (i <240) ? dambo :ayor;
		 election_manager->vote(i, p);
	 }
	
  
}
 void MainClass::printMenu()
 {
	 cout << "*******************MENU:*********************" << endl;
	 for (size_t i = 0; i < shorcuts.size(); i++)
		 cout << i + 1 << "." << shorcuts[i].name << endl;
	 cout << "extras commands:" << endl;
	 cout << "help" << endl;
	 cout << "cls" << endl;
	 cout << "exit" << endl;
 }
 void MainClass::initMain()
 {
	shorcuts= { {"show districts", showDistricts}, { "show citizens",showCitizens },
	 { "show parties",showParties }, { "add citizen",addCitizen }
		 , { "add party",addParty }, { "add district",addDistrict }
		 , { "add party member",addPartyMember }, { "add vote",vote }
		 , { "show elections results",showResults }
		 , { "save elections round",saveElectionsRound }
		 , { "load elections round",loadElectionsRound } };
 }

 void MainClass::run()
 {
	 if (once)
		 return;
	 once = true;
	 try
	 {
	
		 initMain();
	     startMenu();
	 
	   if (election_manager == nullptr)
		   throw NullException("election manager is nullptr!!!!");
	 cout << "*****"<<election_manager->getType()<<
		 " ELECTION SYSTEM " << election_manager->getDate() << "*****" << endl; 
	 printMenu();
	 std::string line;
	 while (alive)
	 {
		 cout << "command:";
		 getline(std::cin, line);
		 if (line == "exit")
			 break;
		 if (line == "help")
		 {
			 printMenu();
			 continue;
		 }
		 if (line == "cls")
		 {
			 system("cls");
			 continue;
		 }
		 
		 bool executed = false;
		 line = Templates::Transform(line, toupper);
		 for (size_t i = 0; i < shorcuts.size() && !executed; i++)
		 {
			std::string shorcutNameUpper=Templates::Transform(shorcuts[i].name,toupper);
		 
			 if (shorcutNameUpper == line || line == to_string(i + 1))
			 {
			 cout << "*****************" << shorcutNameUpper << "***************" << endl;
				 executed = true;
				 try
				 {
					 shorcuts[i].execute();
					 if (shorcuts[i].name == "load elections round")
					 {
						 
					  cout << "*****" << election_manager->getType()
					  << " ELECTION SYSTEM " << election_manager->getDate() << "*****" << endl;
						
					 }
				 }
				 catch (std::exception&e)
				 {
					 cout << e.what() << endl;
				 }
			 }
		 }
		 if (!executed)
			 cout << "wrong input press help to see menu" << endl;    
	 }

	 }//MAIN TRY ENDS
	 catch (exception&e)
	 {
		 cout << e.what() << endl;
	 }
	 cout << "exiting program..." << endl;
	 if(election_manager)
	 delete election_manager;
	 election_manager = nullptr;

	 
 }


 void MainClass::addCitizen()
 {
	 int id;
	 std::string name;
	 int birth;
	 int district_id=-1;

	 cout << "citizen name:";
	 getline(std::cin, name);
	 cout << "citizen id:";
	 std::cin >> id;
	 cout << "birth year:";
	 std::cin >> birth;
	 if (dynamic_cast<NormalElections*>(election_manager))
	 {
		 cout << "district id:";
		 std::cin >> district_id;
	 }
	 
	 try
	 {

		 Citizen c(name, id, birth);
		election_manager->addCitizen(c, district_id);
		cout << "inserted " << name << " to database succesfuly" << endl
				 << "citizen details:"
				 << endl << *election_manager->getCitizensBook().getCitizens().back() << endl;
		
	 }
	 catch (...)
	 {
		 std::cin.ignore();
		 throw;
	 }
	 std::cin.ignore();




 }
 void MainClass::addParty()
 {
	 std::string party_name;
	 int leader_id;
	 cout << "party name:";
	 getline(std::cin, party_name);
	 cout << "leader id:";
	 std::cin >> leader_id;

	 try
	 {
		 election_manager->addParty(party_name, leader_id);
		 cout << "inserted party " << party_name << " to db succesfuly" << endl;
		 cout << "party details:" << endl;
		 cout << *election_manager->getParties().back() << endl;
	 }
	 catch (...)
	 {
		 std::cin.ignore();
		 throw;
	 }
	
	 std::cin.ignore();
  }
 void MainClass::addPartyMember()
 {
	 int citizen_id, party_id, district_id=-1;
	 cout << "citizen id:";
	 std::cin >> citizen_id;
	 cout << "party id:";
	 std::cin >> party_id;
	 if (dynamic_cast<NormalElections*>(election_manager))
	 {
		 cout << "district id:";
		 std::cin >> district_id;
	 }
	 try
	 {
		 (election_manager->addPartyMember(citizen_id, party_id, district_id));
		 const Citizen &c = *election_manager->getCitizensBook().find(citizen_id);
		 cout << "added citizen " << c.getName() << " to " << c.getParty()->getName()
			 << " party succesfuly!!!" << endl;
	 }
	 catch (...)
	 {
		 std::cin.ignore();
		 throw;
	 }
	
	 std::cin.ignore();
 }
 void MainClass::showResults()
 {
	 election_manager->showResults();
 }
 void MainClass::showCitizens()
 {
	 election_manager->showCitizens();
  }
 void MainClass::showParties()
 {
	 election_manager->showParties();
 }
 void MainClass::showDistricts()
 {
	 election_manager->showDistricts();
 }
 void MainClass::addDistrict()
 {
	 NormalElections* normal_elections = dynamic_cast<NormalElections*>(election_manager);
	 if (!normal_elections)
	 {
		 cout << "Add District is undefined Operations for this elections type"<<endl;
		 return;
	 }
	 std::string name;
	 int rep;
	 int type;
	 cout << "district name:";
	 getline(std::cin, name);
	 cout << "reprenstaives number:";
	 std::cin >> rep;
	 cout << "district type(1-divided 2-unifrom):";
	 std::cin >> type;
	 if (!(type >= 1 && type <= 2))
	 {
		 cout << "bad type entered" << endl;
	 }
	 else 
	 {
		
		 District*d = nullptr;
		 try
		 { 
			 if (type == 1)
				 d = new DividedDistrict(name, rep);
			 else d = new UnifromDistrict(name, rep);

			 (election_manager->addDistrict(d));
			 cout << "inserted distrct " << name << " to db succesfuly!!!" << endl;
			 cout << "distrect details:" << endl;
			 cout << *d << endl;
		 }
		 catch(...)//memory or adddistrict failed
		 {
			 if(d)//delete if add district failes
			 delete d;
			 std::cin.ignore();
			 throw;
		 }
	 }
	
	 std::cin.ignore();
 }

 void MainClass::vote()
 {
	 int citizen_id, party_id;
	 cout << "citizen id:";
	 std::cin >> citizen_id;
	 cout << "party id:";
	 std::cin >> party_id;
	 try
	 {
		 (election_manager->vote(citizen_id, party_id));
		 cout << "citizen " << citizen_id << " voted to party with id:" << party_id
			 << " succesfuly!!!" << endl;
	 }
	 catch (...)
	 {
		 std::cin.ignore();
		 throw;
	 }
	
	 std::cin.ignore();
 }

  void MainClass::startMenu()
 {
	  const int NEW_ROUND = 1;
	  const int LOAD_ROUND = 2;
	  const int EXIT_PROG = 3;
	  cout << "***START MENU***" << endl;
	  cout << "1." << "create new election round" << endl;
	  cout << "2." << "load election round" << endl;
	  cout << "3." << "exit" << endl;
	  cout << "choose option:";
	  int option;
	  std::cin >> option;
	  while (!(option >= 1 && option <= 3))
	  {
		  cout << "wrong option!!!" << endl;
		  cout << "choose option:";
		  std::cin >> option;
	  }
	  if (option ==NEW_ROUND)
	  {
		  cout << "***************NEW ROUND PAGE**********" << endl;
		  cout << "enter date(format:30 12 1995):" << endl;
		  Date d;
		  std::cin >> d;
		  const int NORMAL_TYPE = 1;
		  int electionsType;
		  cout << "round type:"<<endl;
		  cout << std::string("Normal-")+to_string(NORMAL_TYPE)+std::string(" Simple-otherwise") << endl;
		  cout << "enter type:";
		  std::cin >> electionsType;
		  try
		  {
			  switch (electionsType)
			  {
			  case NORMAL_TYPE:
				  election_manager = new NormalElections(d);
				  break;
			  default:
				  int rep_number;
				  cout << "enter rep_number:";
				  std::cin >> rep_number;
				  while (!(rep_number > 0))
				  {
					  cout << "reprenstives numbers must be positive!!!" << endl;
					  cout << "re enter rep_number:";
					  std::cin >> rep_number;

				  }
				  election_manager = new SimpleElections(d, rep_number);

				  break;
			  }
			  system("cls");
			  std::cin.ignore();
		  }
		  catch (std::bad_alloc&e)
		  {
			  throw MemoryException(string("at MainClass::startMenu")+e.what());
		  }
	  }
	  else if (option ==LOAD_ROUND)
	  {
		  std::cin.ignore();
		  system("cls");
		  std::cout << "**********LOAD ROUND PAGE*****" << std::endl;
		  loadElectionsRound();
		  
	  }
	  else if(option==EXIT_PROG)
	  {
		  throw std::exception("bye bye");
	  }
	 

 }
   void MainClass::saveElectionsRound()
  {
	   std::string file_name;
	   cout << "file name:";
	   std::cin >> file_name;
	   cout << "saving..." << endl;
	   ofstream out(file_name.c_str(), ios::binary | ios::out);
	   if (!out)
	   {
		   cout << "failed to open file!!!" << endl;
		   std::cin.ignore();
		   return;
	   }
	   try
	   {
		   District::saveState(out);
		   Party::saveState(out);
		   ElectionsLoader::save(election_manager, out);
		   //system("pause");
		   cout << "saved elections round to file succesfuly" << endl;
		   
	   }
	   catch (...)
	   {
		   if (out.is_open())
			  out.close();
		   std::cin.ignore();
		   cout << "failed to save elections round to file!!!" << endl;
	   }
	   out.close();
	   std::cin.ignore();
  }
   void MainClass::loadElectionsRound()
  {
	   std::string file_name;
	   cout << "file name:";
	   std::cin >> file_name;
	   cout << "loading..." << endl;
	   Elections*newRound = nullptr;
	   ifstream in(file_name.c_str(), ios::binary | ios::in);
	   if (!in)
	   {
		   std::cin.ignore();
		   throw IOException(std::string("failed to open ") + file_name);
	   }
	   int partyIdGen = Party::getState();
	   int DistrictIdGen = District::getState();
	   try
	   {
		   District::loadState(in);
		   Party::loadState(in);
		   newRound = ElectionsLoader::load(in);
		   delete election_manager;
		   election_manager = newRound;
		   cout << "loaded elections round from file succesfuly" << endl;
	   }
	   catch (...)
	   {
		   Party::setState(partyIdGen);//return to old id
		   District::setState(DistrictIdGen);
		   if (in.is_open())
			   in.close();
		   std::cin.ignore();
		  throw IOException(std::string("failed to load elections round from ") + file_name);
	   }
	   std::cin.ignore();
	   in.close();
	   
  }

#include<list>
#include<vector>
#include<array>
 
 int main()
 {
	 
	 _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	 MainClass::run();
	
	 
}

 
 
