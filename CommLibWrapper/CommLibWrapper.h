#pragma once
/////////////////////////////////////////////////////////////////////
// CommLibWrapper.h - Comm object factory                          //
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
#include <string>
#include "../CppCommWithFileXfer/MsgPassingComm/IComm.h"

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

namespace MsgPassingCommunication
{
  extern "C" {
    struct CommFactory {
      static DLL_DECL IComm* create(const std::string& machineAddress, size_t port);
    };
  }
}