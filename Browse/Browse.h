#pragma once

/////////////////////////////////////////////////////////////////////////
// Browse.h - Manages browse functionality                             //
//                                                                     //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* Purpose:
* ----------------
*   This package has 1 class which has 2 methods
*		1. selectFile -- to check existing of particular file
*		2. displayFile -- display the contents of a file
*
*
* Build Process:
* ---------------
* - Required files: Browse.h,Browse.cpp,Process.h,XmlDocument,DateTime
* - Compiler command: devenv Project2.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 27th April 2018
*  - second release
*  ver 1.0 : 4th March 2018
*  - first release
*/

#include <iostream>
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../Process/Process.h"
#include "../Query/Query.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#define WINDOWS  /* uncomment this line to use it for windows.*/ 
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

namespace Repository {

	using namespace NoSqlDb;

	template <typename T>
	class Browse {
	public:
		static void identify(std::ostream& out = std::cout);
		std::vector<std::string> browseFile(const Key& fileName, DbCore<T>& db_);
		void displayFile(const Key& fileName, DbCore<T>& db_);
	};
	//----< helper function to idetify files>---------------------------
	template<typename T>
	void Browse<T>::identify(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
		out << " -- This will provide capability to locate files and also view their contents";
	}

	//----< Get current directory >------------------------------------------
	inline std::string GetCurrentWorkingDir(void) {
		char buff[FILENAME_MAX];
		GetCurrentDir(buff, FILENAME_MAX);
		std::string current_working_dir(buff);
		return current_working_dir;
	}
	//----< helper function to display a file>---------------------------
	template <typename T>
	void Browse<T>::displayFile(const Key& fileName, DbCore<T>& db_)
	{
		Process p;
		p.title("Viewing file called " + fileName);
		DbElement<T> dbEle = db_[fileName];
		PayLoad pl = dbEle.payLoad();
		std::string appPath = "c:/windows/system32/notepad.exe";
		p.application(appPath);
		std::string cmdLine = "/A ";
		std::string dir = GetCurrentWorkingDir();
		size_t val = dir.find("TestClassProj");
		if (val != std::string::npos)
			cmdLine = cmdLine.append("../");
		
		cmdLine = cmdLine.append(pl.value()).append("/").append(fileName);
		p.commandLine(cmdLine);
		p.create();

		CBP callback = []() { std::cout << "\n  --- Notepad closed ---"; };
		p.setCallBackProcessing(callback);
		p.registerCallback();

		std::cout.flush();
		
	}
	//----< helper function to browse repository>---------------------------
	template <typename T>
	std::vector<std::string> Browse<T>::browseFile(const Key& fileName, DbCore<T>& db_)
	{
		showDb(db_);
		std::cout << "\n\nTrying to browse the child of given file" << endl;
		DbElement<PayLoad> ele = db_[fileName];
		Children children = ele.children();
		std::cout << "\nBuilding the query to retrieve the description of the children" << endl;
		Query<PayLoad> q1(db_);
		Keys keys{ children };
		Conditions<PayLoad> conds0;
		conds0.description("This*");
		q1.select(conds0);
		Keys keys2 = q1.keys();
		std::vector<std::string> categories;
		for (auto key : keys2) {
			DbElement<PayLoad> dbEle = db_[key];
			Children child = dbEle.children();
			std::string childInfo;
			if (child.size() > 0) {
				for (auto ch : child) {
					childInfo.append(ch).append(" ");
				}
			}
			std::string temp;
			if(child.size() > 0)
				temp = key.substr(0, key.find_last_of(".")) + "--" + childInfo;
			else
				temp = key.substr(0, key.find_last_of(".")) + "--" + "No child";
			categories.push_back(temp);
		}
		std::cout << "\nDemonstrating: Display all of the files in any category that have no parents";
		std::cout << "\n I am demonstrating this requirement by checking whether a key is child of another key via running a query on database";
		std::vector<std::string> keys3 = db_.keys();
		std::vector<std::string> noParentKeys;
		for (string key : keys3) {
			Query<PayLoad> q2(db_);
			Keys keys4{ key };
			Conditions<PayLoad> conds1;
			conds1.children(keys4);
			Query<PayLoad> result = q2.select(conds1);
			if (result.keys().size() == 0)
				noParentKeys.push_back(key);
		}
		categories.push_back("No parent keys");
		std::string noParentList;
		for (auto key : noParentKeys)
			noParentList.append(key).append(" ");
		categories.push_back(noParentList);
		return categories;
	}
	
	
}