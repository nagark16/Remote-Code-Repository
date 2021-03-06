/////////////////////////////////////////////////////////////////////
// CommLibWrapper.cpp - Comm object factory                        //
//                                                                 //
// Author: Naga Rama Krishna, nrchalam@syr.edu                     //
// Reference: Jim Fawcett                                          //
// Application: RepositoryApp                                      //
// Environment: C++ console                                        // 
// Platform: Lenovo T460                                           // 
// Operating System: Windows 10                                    //
/////////////////////////////////////////////////////////////////////
/*
*  Provides export and import declarations for Comm
*
*  Required Files:
* -----------------
*  CommLibWrapper.h, CommLibWrapper.cpp
*
*  Maintenance History:
* ----------------------
*  ver 1: 6th April 2018
*/

#define IN_DLL

#include "CommLibWrapper.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"  // definition of create

using namespace MsgPassingCommunication;

DLL_DECL IComm* CommFactory::create(const std::string& machineAddress, size_t port)
{
  std::cout << "\n  using CommFactory to invoke IComm::create";
  return IComm::create(machineAddress, port);
}


