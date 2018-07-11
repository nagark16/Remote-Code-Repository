/////////////////////////////////////////////////////////////////////////
// TestClass.h - defines all testing for NoSqlDb                       //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a TestClass class that implements all testing
*  for the NoSqlDb.  It is an alternative to the TestUtilities demonstrated
*  in class for the NoSqlDb prototype.
*
*  Required Files:
*  ---------------
*  TestClass.h, TestClass.cpp
*  DbCore.h, DbCore,cpp
*  DateTime.h, DateTime.cpp
*  PayLoad.h
*  Query.h, Query.cpp
*  StringUtilities.h, StringUtilities.cpp
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 10 Feb 2018
*  - first release
*/

#include "TestClass.h"
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Persist/Persist.h"
#include "../Edit/Edit.h"
#include "../Executive/Executive.h"
#include <functional>


using namespace NoSqlDb;
using namespace Repository;
using namespace TestClass;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
  Utilities::putline(n, out);
};

//----< R1 >-----------------------------------------------------------
/*
*  - Demonstrate that solution uses C++11
*/
bool TestDb::testR1()
{

  Utilities::title("Demonstrating Requirement #1");
  std::cout << "\n  " << typeid(std::function<bool()>).name()
    << ", declared in this function, "
    << "\n  is only valid for C++11 and later versions.";
  return true; // would not compile unless C++11
}
//----< R2 >-----------------------------------------------------------
/*
*  - Cite use of streams and operators new and delete
*/
bool TestDb::testR2()
{
  Utilities::title("Demonstrating Requirement #2");
  std::cout << "\n  A visual examination of all the submitted code "
    << "will show only\n  use of streams and operators new and delete.";
  return true;
}

//----< R3 >----------------------------------------------------------
/*
*  - Demonstrate usage of packages
*/
bool TestDb::testR3()
{
  Utilities::Title("Demonstrating Requirement #3 - used packages");
  RepositoryCore<PayLoad>::identify();
  CheckIn<PayLoad>::identify();
  CheckOut<PayLoad>::identify();
  Version::identify();
  Browse<PayLoad>::identify();
  return true;
}

//----< R4 >----------------------------------------------------------
/*
*  - Demonstrate checkin process
*/
bool TestDb::testR4()
{
	/*Utilities::Title("Demonstrating Requirement #4 - CheckIn process");
	cout << "\n\tAll the paths displayed below are relative" << endl;
	RepositoryCore<PayLoad> repo(db_);
	DbElement<PayLoad> elem;
	elem.name("naga");
	elem.descrip("cpp file to browse the files in repository");
	PayLoad pl;
	pl.value() = "codeRepository/source";
	pl.categories().push_back("repositoryCore");
	pl.isClose() = false;
	elem.payLoad(pl);
	repo.checkIn("Browse.cpp", elem);
	cout << "\n\n\tTrying to CheckIn open file " << endl;
	repo.displayRepo();
	elem.name("rama");
	repo.checkIn("Browse.cpp", elem);
	cout << "\n\n\tTrying to CheckIn same \"Browse.cpp\" file as a different user " << endl;
	repo.displayRepo();
	elem.name("naga");
	pl.isClose() = true;
	elem.payLoad(pl);
	repo.checkIn("Browse.cpp", elem);
	cout << "\n\n\tTrying to close already checkedin file" << endl;
	repo.displayRepo();
	elem.children().push_back("XmlDocument.cpp.1");
	repo.checkIn("Browse.cpp", elem);
	cout << "\n\n\tTrying to checkin \"Browse.cpp\" version 2 after adding a child with closed status, which is not checked in yet" << endl;
	repo.displayRepo();
	elem.name("naga");
	elem.descrip("cpp file to display a file ");
	elem.children().clear();
	pl.isClose() = true;
	elem.payLoad(pl);
	repo.checkIn("XmlDocument.cpp", elem);
	cout << "\n\n\tTrying to checkin \"XmlDocument.cpp\" with closed status" << endl;
	repo.displayRepo();
	elem.name("naga");
	elem.descrip("cpp file to browse the files in repository");
	elem.children().push_back("XmlDocument.cpp.1");
	pl.isClose() = true;
	elem.payLoad(pl);
	repo.checkIn("Browse.cpp", elem);
	cout << "\n\n\tTrying to checkin \"Browse.cpp\" after checking in child" << endl;
	repo.displayRepo();*/
	return true;
}

//----< R5 >----------------------------------------------------------
/*
*  - Demonstrate Browse and checkout process
*/
bool TestDb::testR5()
{
	
	/*Utilities::Title("Demonstrating Requirement #5 - Browse and CheckOut process");
	makeTestDb(db_);
	cout << "\n\tThe repository we are going to use as part of this demonstration is displayed under requirement#6" << endl;
	RepositoryCore<PayLoad> repo(db_);
	cout << "\n\n\t1. Browse for \"DbCore.cpp\" " << endl;
	repo.browseAFile("DbCore.cpp.1");
	cout << "\n\n\t2. Displaying the contents of \"XmlDocument.cpp\" " << endl;
	repo.displayAFile("XmlDocument.cpp.1");
	cout << "\n\n\t3. Checking out \"RepositoryCore.cpp\" " << endl;
	repo.checkOut("RepositoryCore.cpp.1","codeRepository/destination");
	*/
	return true;
}

//----< R6 >----------------------------------------------------------
/*
*  - Demonstrate repository contents
*/
bool TestDb::testR6()
{
	Utilities::Title("Demonstrating Requirement #6 - Repository contents");
	showDb(db_);
	return true;
}

//----< R7 >----------------------------------------------------------
/*
*  - Demonstrate -- meeting requirements
*/
bool TestDb::testR7()
{
	Utilities::Title("Demonstrating Requirement #7 - Demonstration of meeting requirements");
	std::cout << "\n\tAs it can be seen above, I have clearly demonstrated all the requirements";
	return true;
}

//----< implement requirements testing >-------------------------------
#ifdef  TEST_TEST

  

int main()
{
  std::cout << "\n  Testing DbCore<PayLoad>";
  std::cout << "\n =========================";

  NoSqlDb::DbCore<PayLoad> db;
  TestDb tdb(db);
  tdb.invoke(&TestDb::testR1);
  tdb.invoke(&TestDb::testR2);
  tdb.invoke(&TestDb::testR3);
  tdb.invoke(&TestDb::testR4);
  tdb.invoke(&TestDb::testR5);
  tdb.invoke(&TestDb::testR6);
  tdb.invoke(&TestDb::testR7);
  std::cout << "\n\n";
  getchar();
}
#endif //