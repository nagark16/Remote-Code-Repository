///////////////////////////////////////////////////////////////////////////
// Translater.cpp - Translates messages to/from managed and native types //
//                                                                       //
// Author: Naga Rama Krishna, nrchalam@syr.edu                           //
// Reference: Jim Fawcett                                                //
// Application: RepositoryApp                                            //
// Environment: C++ console                                              // 
// Platform: Lenovo T460                                                 // 
// Operating System: Windows 10                                          //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  This C++\Cli Package contains one class, Translater.  It's purpose is to convert
*  managed messages, CsMessage, to native messages, Message, and back.
*
*  Required Files:
* -----------------
*  Translater.h, Translater.cpp
*  CsMessage.h
*  Required References:
* ----------------------
*  CommLibWrapper.dll
*  Message.lib
*
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 6th April 2018
*/

#include "CsMessage.h"
#include "Translater.h"

using namespace MsgPassingCommunication;

int main()
{
  std::cout << "\n  Testing Translator Package";
  std::cout << "\n ============================\n";

  std::cout << "\n  testing CsEndPoint";
  std::cout << "\n --------------------";
  CsEndPoint^ ep = gcnew CsEndPoint;
  ep->machineAddress = "localhost";
  ep->port = 8081;
  String^ epStr = CsEndPoint::toString(ep);
  Console::Write("\n  {0}", epStr);
  CsEndPoint^ newEp = CsEndPoint::fromString(epStr);
  Console::Write("\n  machineAddress = {0}\n  port = {1}", newEp->machineAddress, newEp->port);
  std::cout << "\n";
  std::cout << "\n  testing CsMessage";
  std::cout << "\n -------------------";
  CsEndPoint^ ep2 = gcnew CsEndPoint;
  ep2->machineAddress = "localhost";
  ep2->port = 8080;
  CsMessage^ csMsg = gcnew CsMessage;
  csMsg->add("to", CsEndPoint::toString(ep2));
  csMsg->add("from", CsEndPoint::toString(ep));
  csMsg->add("command", "echo");
  csMsg->add("verbose", "beep");
  csMsg->show();
  std::cout << "\n";
  std::cout << "\n  testing Translater access to Comm";
  std::cout << "\n -----------------------------------";
  Translater^ tran = gcnew Translater;
  tran->listen(ep);
  for (size_t i = 0; i < 5; ++i)
  {
    tran->postMessage(csMsg);
    csMsg->remove("verbose");
  }
  for (size_t i = 0; i < 5; ++i)
  {
    CsMessage^ reply = tran->getMessage();
    reply->show();
  }
  std::cout << "\n\n";
  return 0;
}

