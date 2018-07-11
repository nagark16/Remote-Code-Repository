#ifndef NOSQLDB_H
#define NOSQLDB_H
/////////////////////////////////////////////////////////////////////////
// NoSqlDb.h - used to pull all required includes into one header      //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* - This package defines this single header file that includes all the NoSqlDb parts.
*   We do this so that an application needs only to include two headers:
*   - NoSqlDb.h : declare NoSqlDb functionality
*   - PayLoad.h : declared by application to provide specific payload functionality
*   The application needs to:
*   - include the application's PayLoad.cpp in its project
*   - make a reference to the Executive project's static library
*/

#include "../DateTime/DateTime.h"
#include "../DbCore/DbCore.h"
#include "../DbCore/Definitions.h"
#include "../Edit/Edit.h"
#include "../Persist/Persist.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

#endif
