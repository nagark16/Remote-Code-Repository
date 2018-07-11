#pragma once
/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
//                                                                     //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: RepositoryApp                                          //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  Package contains one class, Server, that contains a Message-Passing Communication
*  facility. It processes each message by invoking an installed callable object
*  defined by the message's command key.
*
*  Message handling runs on a child thread, so the Server main thread is free to do
*  any necessary background processing (none, so far).
*
*  Required Files:
* -----------------
*  ServerPrototype.h, ServerPrototype.cpp
*  Comm.h, Comm.cpp, IComm.h
*  Message.h, Message.cpp
*  FileSystem.h, FileSystem.cpp
*  Utilities.h
*
*  Maintenance History:
* ----------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 4/6/2018
*  - first release
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include <windows.h>
#include <tchar.h>
#include "../RepositoryCore/RepositoryCore.h"
#include "../PayLoad/PayLoad.h"



namespace NoSqlDb
{
  using File = std::string;
  using Files = std::vector<File>;
  using Dir = std::string;
  using Dirs = std::vector<Dir>;
  using SearchPath = std::string;
  using Key = std::string;
  using Msg = MsgPassingCommunication::Message;
  using ServerProc = std::function<Msg(Msg)>;
  using MsgDispatcher = std::unordered_map<Key,ServerProc>;
  
  const SearchPath storageRoot = "../Storage";  // root for all server file storage
  const MsgPassingCommunication::EndPoint serverEndPoint("localhost", 8080);  // listening endpoint

  class Server
  {
  public:
    Server(MsgPassingCommunication::EndPoint ep, const std::string& name);
	void start();
    void stop();
    void addMsgProc(Key key, ServerProc proc);
    void processMessages();
    void postMessage(MsgPassingCommunication::Message msg);
    MsgPassingCommunication::Message getMessage();
    static Dirs getDirs(const SearchPath& path = storageRoot);
    static Files getFiles(const SearchPath& path = storageRoot);
	Msg browse(Msg msg);
	Msg checkOut(Msg msg);
	Msg checkIn(Msg msg);
	Msg checkInFiles(Msg msg);
	Msg viewMetadata(Msg msg);
  private:
    MsgPassingCommunication::Comm comm_;
    MsgDispatcher dispatcher_;
    std::thread msgProcThrd_;
	Repository::RepositoryCore<PayLoad> repo_;
	
  };
  //----< initialize server endpoint and give server a name >----------

  inline Server::Server(MsgPassingCommunication::EndPoint ep, const std::string& name)
    : comm_(ep, name) {}

  //----< start server's instance of Comm >----------------------------

  inline void Server::start()
  {
    comm_.start();
  }
  //----< stop Comm instance >-----------------------------------------

  inline void Server::stop()
  {
    if(msgProcThrd_.joinable())
      msgProcThrd_.join();
    comm_.stop();
  }
  //----< pass message to Comm for sending >---------------------------

  inline void Server::postMessage(MsgPassingCommunication::Message msg)
  {
    comm_.postMessage(msg);
  }
  //----< get message from Comm >--------------------------------------

  inline MsgPassingCommunication::Message Server::getMessage()
  {
    Msg msg = comm_.getMessage();
    return msg;
  }
  //----< add ServerProc callable object to server's dispatcher >------

  inline void Server::addMsgProc(Key key, ServerProc proc)
  {
    dispatcher_[key] = proc;
  }
  //----< start processing messages on child thread >------------------

  inline void Server::processMessages()
  {
    auto proc = [&](){
      if (dispatcher_.size() == 0){
        std::cout << "\n  no server procs to call";
        return;
      }
      while (true){
        Msg msg = getMessage();
        std::cout << "\n\n  received message: " << msg.command() << " from " << msg.from().toString();
        if (msg.containsKey("verbose")){
			std::cout << "\n";msg.show();
        }
        if (msg.command() == "serverQuit")
          break;
		Msg reply;
		if (msg.command() == "browseDescription") 
			reply = browse(msg);
		else if (msg.command() == "metadataContent")
			reply = viewMetadata(msg);
		else if (msg.command() == "checkOutFiles")
			reply = checkOut(msg);
		else if (msg.command() == "checkIn")
			reply = checkIn(msg);
		else if (msg.command() == "checkInFiles")
			reply = checkInFiles(msg);
		else {
			reply = dispatcher_[msg.command()](msg);
			if (msg.to().port == msg.from().port)  // avoid infinite message loop
				std::cout << "\n  server attempting to post to self";
		}
		std::cout << "\nDemonstrating requirement #4 and #5: "
			<<"\n\t4. Message passing communication system: The below request message is received, via sockets,"
			<<"\n\t   from GUI(one process) to access specific functionality of Repository(another process) "
			<<"\n\t5. Asynchronous communication: The below request and reply messages are communicated in HTTP style."
			<<"\n\t   Also, the sending process(GUI) sent the messages and is not waiting for reply messages.";
		std::cout << "\nRequest Message";std::cout << "\n----------------------";
		msg.show();
		std::cout << "\nReply Message";std::cout << "\n----------------------";
		reply.show();
		postMessage(reply);
      }
      std::cout << "\n  server message processing thread is shutting down";
    };
    std::thread t(proc);
    std::cout << "\n  starting server thread to process messages";
    msgProcThrd_ = std::move(t);
  }

  
}