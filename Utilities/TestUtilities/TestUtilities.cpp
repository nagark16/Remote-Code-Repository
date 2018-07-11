/////////////////////////////////////////////////////////////////////////
// TestUtilities.cp - provides single-user test harness                //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - TestExecutor    Executes single test in the context of a try-catch block
* - TestExecutive   Uses TestExecutor to run a sequence of tests
*
* Build Process:
* ---------------
* - Required files: Test.h,Test.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include <cctype>
#include <iostream>
#include <functional>
#include "TestUtilities.h"
#include "../StringUtilities/StringUtilities.h"

#ifdef TEST_TESTUTILITIES

using namespace Utilities;

///////////////////////////////////////////////////////////////////////
// define demo tests

bool test_always_passes() { return true; }
bool test_always_fails() { return false; }
bool test_always_throws() {
  std::exception ex("exception\n         -- msg: this test always throws -- ");
  throw(ex);
}

int main()
{
  Title("Testing TestUtilities Package");
  putline();

  TestExecutive ex;

  TestExecutive::TestStr ts1{ test_always_passes, "test_always_passes" };
  TestExecutive::TestStr ts2{ test_always_fails, "test_always_fails" };
  TestExecutive::TestStr ts3{ test_always_throws, "test_always_throws" };
  
  ex.registerTest(ts1);
  ex.registerTest(ts2);
  ex.registerTest(ts3);

  bool result = ex.doTests();
  if (result == true)
    std::cout << "\n  all tests passed";
  else
    std::cout << "\n  at least one test failed";

  putline(2);
  return 0;
}
#endif
