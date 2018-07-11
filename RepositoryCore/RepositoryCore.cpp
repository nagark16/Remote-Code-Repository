/////////////////////////////////////////////////////////////////////////
// RepositoryCore.cpp - Manages the whole repository                   //
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
*   This package has 1 class to provide repository functionalities
*	And this provide public functionalities
	1. CheckIn -- to check in a file
	2. CheckOut -- to checkout a file
	3. DisplayRepo -- to display content a repo
	4. DisplayAFile -- to display content of file in repo
	5. BrowseAFile -- browse repository the contents of a file
*
*
* Build Process:
* ---------------
* - Required files: RepositoryCore.h,RepositoryCore.cpp,CheckIn.h,CheckOut.h,Browse.h
* - Compiler command: devenv Project2.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 4th March 2018
*  - first release
*/

#include "../DbCore/DbCore.h"
#include "RepositoryCore.h"

#ifdef REPOSITORYCORE_TEST

using namespace NoSqlDb;
using namespace Repository;

//----< test stub>---------------------------
int main() {
	RepositoryCore<PayLoad> repoObj;
	repoObj.browseAFile("DbCore.h.1");
	return 0;
}

#endif // REPOSITORYCORE_TEST

