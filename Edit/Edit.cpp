/////////////////////////////////////////////////////////////////////////
// Edit.cpp - Edit metadata values                                     //
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
*  This package defines a single Edit class that:
*  - accepts a DbElement<P> instance when constructed
*  - provides facilities to edit it's metadata
*
*  Required Files:
*  ---------------
*  Edit.h, Edit.cpp
*  DbCore.h, DbCore.cpp
*  Query.h, Query.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 17 Feb 2018
*  - first release
*/

#include <string>
#include <iostream>

#ifdef TEST_EDIT

#include "Edit.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
using namespace NoSqlDb;

int main()
{
  Utilities::Title("Demonstrating DbElement Edits");

  DbElement<std::string> elem;
  elem.name("original name");
  elem.descrip("test element");
  elem.payLoad("elem payload");
  showElem(elem);

  Edit<std::string> edit(elem);
  edit.name("new name");
  edit.description("edited test element");
  DateTime& edt = edit.dateTime();
  edt -= DateTime::makeDuration(48, 0);
  edit.payLoad("edited elem payload");
  showElem(edit.DbElement());
  std::cout << "\n\n";
}
#endif