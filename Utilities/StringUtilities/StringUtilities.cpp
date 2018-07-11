/////////////////////////////////////////////////////////////////////////
// StringUtilities.cpp - small, generally useful, helper classes       //
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
* This package provides functions:
* - Title(text)           display title
* - title(text)           display subtitle
* - putline(n)            display n newlines
* - trim(str)             remove leading and trailing whitespace
* - split(str, 'delim')   break string into vector of strings separated by delim char
* - showSplit(vector)     display splits
*
* Build Process:
* ---------------
* - Required files: StringUtilities.h,StringUtilities.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include <cctype>
#include <iostream>
#ifdef TEST_STRINGUTILITIES

#include "StringUtilities.h"
#include "../CodeUtilities/CodeUtilities.h"

using namespace Utilities;

int main()
{
  Title("Testing Utilities Package");
  putline();

  title("test split(std::string, ',')");

  std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
  std::cout << "\n  test string = " << test;

  std::vector<std::string> result = split(test);
  showSplits(result);

  title("test split(std::string, ' ')");

  std::cout << "\n  test string = " << test;

  result = split(test, ' ');
  showSplits(result);

  putline(2);
  return 0;
}
#endif
