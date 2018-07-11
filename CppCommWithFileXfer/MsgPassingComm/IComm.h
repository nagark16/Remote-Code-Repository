#pragma once
/////////////////////////////////////////////////////////////////////
// IComm.h - interface for message-passing communication facility  //
//                                                                 //
// Author: Naga Rama Krishna, nrchalam@syr.edu                     //
// Reference: Jim Fawcett                                          //
// Application: RepositoryApp                                      //
// Environment: C++ console                                        // 
// Platform: Lenovo T460                                           // 
// Operating System: Windows 10                                    //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides the interface to be implmented.
*
*  
*  Maintenance History:
*  --------------------
*  ver 1: 6th April 2018
*/

#include <string>
#include "../Message/Message.h"

namespace MsgPassingCommunication
{
  class IComm
  {
  public:
    static IComm* create(const std::string& machineAddress, size_t port);
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void postMessage(Message msg) = 0;
    virtual Message getMessage() = 0;
    virtual std::string name() = 0;
    virtual ~IComm() {}
  };
}