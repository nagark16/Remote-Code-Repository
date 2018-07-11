/////////////////////////////////////////////////////////////////////////
// PayLoad.h - application defined payload                             //
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
*  This package provides a single class, PayLoad:
*  - holds a payload string and vector of categories
*  - provides means to set and access those values
*  - provides methods used by Persist<PayLoad>:
*    - Sptr toXmlElement();
*    - static PayLoad fromXmlElement(Sptr elem);
*  - provides a show function to display PayLoad specific information
*  - PayLoad processing is very simple, so this package contains only
*    a header file, making it easy to use in other packages, e.g.,
*    just include the PayLoad.h header.
*
*  Required Files:
*  ---------------
*    PayLoad.h, PayLoad.cpp - application defined package
*    DbCore.h, DbCore.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 19 Feb 2018
*  - added inheritance from IPayLoad interface
*  Ver 1.0 : 10 Feb 2018
*  - first release
*
*/

#include "PayLoad.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace NoSqlDb;
using namespace XmlProcessing;

#ifdef TEST_PAYLOAD

int main()
{
  Utilities::Title("Demonstrating Application Specific PayLoad class");
  Utilities::putline();

  using Sptr = std::shared_ptr<AbstractXmlElement>;

  Utilities::title("creating xml string from payload instance");
  PayLoad pl;
  pl.value("demo payload value");
  pl.categories().push_back("cat1");
  pl.categories().push_back("cat2");
  Sptr sPpl = pl.toXmlElement();
  XmlDocument doc(sPpl);
  std::cout << doc.toString();
  Utilities::putline();

  Utilities::title("creating payload instance from an XmlElement");
  PayLoad newPl = pl.fromXmlElement(sPpl);
  std::cout << "\n  payload value = " << pl.value();
  std::cout << "\n  payload categories:\n    ";
  for (auto cat : newPl.categories())
  {
    std::cout << cat << " ";
  }
  std::cout << "\n\n";
}
#endif
