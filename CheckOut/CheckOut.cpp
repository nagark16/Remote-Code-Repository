/////////////////////////////////////////////////////////////////////////
// CheckOut.cpp - Manages CheckOut process from repository             //
//                                                                     //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* Purpose:
* ----------------
*   This package provides one CheckOut class to copy file and all dependents to 
*	a folder. And this class one checkoutFile public method to do the action.
*
*
* Build Process:
* ---------------
* - Required files: CheckOut.h,CheckOut.cpp,DbCore,Version.h,FileSystem.h,FileSystem.cpp,DbCore,DateTime
* - Compiler command: devenv Project2.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 4th March 2018
*  - first release
*/

#ifdef CHECKOUT_TEST
#include "CheckOut.h"
#include "../CheckIn/CheckIn.h"
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include <iostream>

using namespace Repository;
using namespace NoSqlDb;
using namespace std;
//----< test stub>---------------------------
int main() {
	CheckOut<PayLoad> checkout;
	DbCore<PayLoad> db_;
	DbElement<PayLoad> elem;
	elem.name("naga");
	elem.descrip("cpp file to browse the files in repository");
	PayLoad pl;
	pl.value() = "codeRepository/source";
	pl.categories().push_back("repositoryCore");
	pl.isClose() = false;
	elem.payLoad(pl);
	cout << "\nafter trying to CheckIn open item " << endl;
	showDb(db_);
	elem.name("rama");
	cout << "\nafter trying to CheckIn open item by different user " << endl;
	showDb(db_);
	elem.name("naga");
	pl.isClose() = true;
	elem.payLoad(pl);
	cout << "\nafter trying to CheckIn closed item " << endl;
	showDb(db_);
	elem.children().push_back("Process");
	cout << "\ntrying to checkin after adding a child but not checked in to repo" << endl;
	showDb(db_);
	elem.name("naga");
	elem.descrip("cpp file to display a file ");
	elem.children().clear();
	pl.isClose() = true;
	elem.payLoad(pl);
	cout << "\nafter trying to checkin Process.cpp with closed status" << endl;
	showDb(db_);
	elem.name("naga");
	elem.descrip("cpp file to browse the files in repository");
	elem.children().push_back("Process.cpp");
	cout << "\ntrying to checkin Browse.cpp after adding a child checked in this time" << endl;
	showDb(db_);
	cout << "about to copy file";
	std::string dest = "codeRepository/destination/Browse.cpp.1";
	checkout.checkOutFile("Browse.cpp.1",dest,db_);
	getchar();
	return 0;
 }

#endif // CHECKOUT_TEST

