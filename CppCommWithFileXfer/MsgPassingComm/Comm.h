#pragma once
/////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                 //
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
*  This package defines Sender and Receiver classes.
*  - Sender uses a SocketConnecter and supports connecting to multiple
*    sequential endpoints and posting messages.
*  - Receiver uses a SocketListener which returns a Socket on connection.
*  It also defines a Comm class
*  - Comm simply composes a Sender and a Receiver, exposing methods:
*    postMessage(Message) and getMessage()
*
*  Required Files:
*  ---------------
*  Comm.h, Comm.cpp,
*  Sockets.h, Sockets.cpp,
*  Message.h, Message.cpp,
*  Utilities.h, Utilities.cpp
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1: 6th April 2018
*/

#include "../Message/Message.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Sockets/Sockets.h"
#include "IComm.h"
#include <string>
#include <thread>

using namespace Sockets;

namespace MsgPassingCommunication
{
  ///////////////////////////////////////////////////////////////////
  // Receiver class

  class Receiver
  {
  public:
    Receiver(EndPoint ep, const std::string& name = "Receiver");
    template<typename CallableObject>
    void start(CallableObject& co);
    void stop();
    Message getMessage();
    BlockingQueue<Message>* queue();
  private:
	  BlockingQueue<Message> rcvQ;
    SocketListener listener;
    std::string rcvrName;
  };

  ///////////////////////////////////////////////////////////////////
  // Sender class

  class Sender
  {
  public:
    Sender(const std::string& name = "Sender");
    ~Sender();
    void start();
    void stop();
    bool connect(EndPoint ep);
    void postMessage(Message msg);
  private:
  	bool sendFile(Message msg);
	  BlockingQueue<Message> sndQ;
    SocketConnecter connecter;
    std::thread sendThread;
    EndPoint lastEP;
    std::string sndrName;
  };

  class Comm : public IComm
  {
  public:
    Comm(EndPoint ep, const std::string& name = "Comm");
    void start();
    void stop();
    void postMessage(Message msg);
    Message getMessage();
    std::string name();
  private:
    Sender sndr;
    Receiver rcvr;
    std::string commName;
    Sockets::SocketSystem socksys_;
  };

  inline IComm* IComm::create(const std::string& machineAddress, size_t port)
  {
    std::cout << "\n  creating an instance of Comm on the native heap";
    EndPoint ep(machineAddress, port);
    IComm* pComm = new Comm(ep, "created Comm");
    return pComm;
  }
}