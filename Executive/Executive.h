#pragma once
/////////////////////////////////////////////////////////////////////////
// Executive.h - used to set up deployment process                     //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* - This package defines a single header file that includes all the NoSqlDb parts.
*   We do this so that an application needs only to include two headers:
*   - NoSqlDb.h : declare NoSqlDb functionality
*   - PayLoad.h : declare application specific payload functionality
*   The application needs to:
*   - include PayLoad.cpp in its project
*   - make a reference to the Executive project's static library
*/

#include <iostream>
#include "NoSqlDb.h"

class Executive
{
public:
  static void identify(std::ostream& out = std::cout)
  {
    out << "\n  \"" << __FILE__ << "\"";
  }
};