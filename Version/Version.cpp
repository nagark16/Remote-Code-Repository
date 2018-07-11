/////////////////////////////////////////////////////////////////////////
// Version.cpp - Manages the version number to checkin files           //
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
*   This cpp file will test methods implemented in header file.
* 
*   This package has 1 Version class which provides 2 public methods
*		1. getVersionInfo -- This method will give current version number of a particular file
*		2. incrementVersion -- This method will increment version of input file
*
*
* Build Process:
* ---------------
* - Required files: Version.h,Version.cpp
* - Compiler command: devenv Project2.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 4th March 2018
*  - first release
*/
#ifdef VERSION_TEST

#include "Version.h"
#include <iostream>
#include "../DbCore/Definitions.h"

using namespace Repository;
using namespace std;


//----< test stub>---------------------------
int main() {
	Version v;
	std::unordered_map<std::string, size_t> versionInfo;
	std::string key = "test";
	cout << "Checking test file version";
	cout << v.getVersionInfo(key, versionInfo) << "\n";
	cout << "Increment test file version";
	cout << v.incrementVersion(key, versionInfo) << "\n";
	cout << "Checking test file version";
	cout << v.getVersionInfo(key, versionInfo) << "\n";
	getchar();
	return 0;
}

#endif // VERSION_TEST

