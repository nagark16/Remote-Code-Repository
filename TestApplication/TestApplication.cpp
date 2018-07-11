///////////////////////////////////////////////////////////////////////
// TestApplication.cpp - designed to test deployment strategy        //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* - This is an example of how an application will use the NoSqlDb functionality
* - It needs to:
*   - Copy NoSqlDb folder into solution directory
*     - It doesn't need to add NoSqlDb packages to Application's code.
*       That's taken care of by the library it references, below.
*   - Define it's own PayLoad package
*   - Include the composite NoSqlDb.h header
*   - Include its own PayLoad.h
*   - Make a reference to the Executive.lib
*/
#include "../Executive/NoSqlDb.h"
#include "../PayLoad/PayLoad.h"

using namespace NoSqlDb;

/*
* - This demo code was simply lifted from Executive.cpp
*/
int main()
{
  Utilities::Title("TestApplication - demonstrates that deployment strategy works");
  Utilities::putline();

  Utilities::title("making demo database");
  DbCore<PayLoad> db;

  DbElement<PayLoad> elem = makeElement<PayLoad>("a name", "a description");
  PayLoad pl;
  pl.value("a payload value");
  pl.categories().push_back("some category");
  elem.payLoad(pl);
  db["first"] = elem;

  std::string name2 = "another name";
  elem.name(name2);
  pl.value("another payload value");
  pl.categories().push_back("some other category");
  elem.payLoad(pl);
  db["second"] = elem;

  showDb(db);
  Utilities::putline();

  elem.payLoad().showDb(db);
  Utilities::putline();

  Utilities::title("demonstrating query");
  Query<PayLoad> q(db);
  q.select(
    [=](DbElement<PayLoad>& elem) {
    if (elem.name() == name2)
    {
      std::cout << "\n  " << elem.name();
      return true;
    }
    return false;
  }
  ).show();
  Utilities::putline();

  std::cout << "\n\n";
  return 0;
}

