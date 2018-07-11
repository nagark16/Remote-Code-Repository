/////////////////////////////////////////////////////////////////////////
// Persist.h - persist DbCore<P> to and from XML file                  //
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
*  This package defines a single Persist class that:
*  - accepts a DbCore<P> instance when constructed
*  - persists its database to an XML string
*  - creates an instance of DbCore<P> from a persisted XML string
*
*  Required Files:
*  ---------------
*  Persist.h, Persist.cpp
*  DbCore.h, DbCore.cpp
*  Query.h, Query.cpp
*  PayLoad.h
*  XmlDocument.h, XmlDocument.cpp
*  XmlElement.h, XmlElement.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 12 Feb 2018
*  - first release
*/

//----< test stub >----------------------------------------------------

#ifdef TEST_PERSIST

#include "Persist.h"
#include "../PayLoad/PayLoad.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace NoSqlDb;

DbCore<PayLoad> makeTestDb()
{
  DbCore<PayLoad> db;
  PayLoad pl;
  // first record
  pl.value() = "test value1";
  pl.categories().push_back("cat1");
  pl.categories().push_back("cat2");
  DbElement<PayLoad> elem;
  elem.name("elem1");
  elem.descrip("descrip1");
  elem.children().push_back("elem2");
  elem.children().push_back("elem3");
  elem.payLoad(pl);
  db["one"] = elem;

  // second record
  pl.value() = "test value2";
  pl.categories().clear();
  pl.categories().push_back("cat1");
  pl.categories().push_back("cat3");
  elem.name("elem2");
  elem.descrip("descrip2 - a very long and boring description of virtually nothing");
  elem.children().clear();
  elem.children().push_back("elem1");
  elem.children().push_back("elem3");
  elem.payLoad(pl);
  db["two"] = elem;

  // third record
  pl.value() = "test value3 with some extra not very interesting triva";
  pl.categories().clear();
  pl.categories().push_back("cat2");
  elem.name("elem3");
  elem.descrip("descrip3");
  elem.children().clear();
  elem.children().push_back("elem2");
  elem.payLoad(pl);
  db["three"] = elem;

  return db;
}

int main()
{
  Utilities::Title("Demonstrating Persistance Package");
  Utilities::putline();

  Utilities::title("test database");
  DbCore<PayLoad> db = makeTestDb();
  showDb(db);
  Utilities::putline();

  Utilities::title("test database payload details");
  PayLoad::showDb(db);
  Utilities::putline();

  Persist<PayLoad> persist(db);
  Xml xml = persist.toXml();
  Utilities::title("XML string representation of test database:");
  std::cout << xml;
  Utilities::putline();

  Utilities::title("after rebuilding db from xml string");
  persist.fromXml(xml);
  showDb(db);
  Utilities::putline();
  PayLoad::showDb(db);

  std::cout << "\n\n";
  return 0;
}

#endif
