/////////////////////////////////////////////////////////////////////////
// Browse.cpp - Manages browse functionality                           //
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
*   This package has 1 class which has 2 methods
*		1. selectFile -- to check existing of particular file
*		2. displayFile -- display the contents of a file
*
*
* Build Process:
* ---------------
* - Required files: Browse.h,Browse.cpp,Process.h,XmlDocument,DateTime
* - Compiler command: devenv Project2.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 4th March 2018
*  - first release
*/

#ifdef BROWSE_TEST

#include "Browse.h"
#include <iostream>

using namespace NoSqlDb;
using namespace Repository;


//----< test stub>---------------------------
int main() {

	DbCore<PayLoad> db;
	PayLoad pl;
	DbElement<PayLoad> elem;
	
	elem.name("naga");
	elem.descrip("Manages browse functionalidy Repository");
	pl.value() = "codeRepository/source";
	pl.categories().push_back("repositoryCore");
	elem.payLoad(pl);

	db["Browse.cpp.1"] = elem;

	Browse<PayLoad> Browser;
	showDb(db);
	Browser.displayFile("Browse.cpp.1",db);
	return 0;
}

#endif // BROWSE_TEST

