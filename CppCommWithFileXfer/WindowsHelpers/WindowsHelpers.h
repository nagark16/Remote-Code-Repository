#ifndef WINDOWSHELPERS_H
#define WINDOWSHELPERS_H
/////////////////////////////////////////////////////////////////////////
// WindowsHelper.h - small helper functions for using Windows API      //
//                                                                     //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: RepositoryApp                                          //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports programming to the Windows API.  It currently
* contains three global functions that convert strings to and from
* UTF-16, and retrieving error messages.
*
* Build Process:
* --------------
* Required Files: WindowsHelper.h, WindowwsHelper.cpp
*
* Build Command: devenv WindowsHelper.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1: 6th April 2018
*/

#include <string>
#include <vector>

namespace WindowsHelpers
{
  std::string wstringToString(const std::wstring& wstr);
  std::wstring stringToWideString(const std::string& str);
  std::string GetLastMsg(bool wantSocketMessage);
}
 
#endif
