#ifndef DEFINITIONS_H
#define DEFINITIONS_H
/////////////////////////////////////////////////////////////////////////
// Definitions.h - define aliases used throughout NoSqlDb namespace    //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

namespace NoSqlDb
{
  using Key = std::string;
  using Keys = std::vector<Key>;
  using Children = std::vector<Key>;
  using Parents = std::vector<Key>;

  const bool showKey = true;
  const bool doNotShowKey = false;
}
#endif
