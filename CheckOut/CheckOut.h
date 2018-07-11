#pragma once
/////////////////////////////////////////////////////////////////////////
// CheckOut.h - Lets us checkout files to repository                   //
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
*   This package provides one CheckOut class to copy file and all dependents to
*	a folder. And this class one checkoutFile public method to do the action.
*
*
* Build Process:
* ---------------
* - Required files: CheckOut.h,CheckOut.cpp,DbCore,Version.h,FileSystem.h,FileSystem.cpp,DbCore,DateTime
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
#include <string>
#include <vector>
#include <algorithm>
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../FileSystem/FileSystem.h"


using namespace NoSqlDb;
using namespace FileSystem;


namespace Repository 
{
	
	template <typename T>
	class CheckOut {
	public:

	    static void identify(std::ostream& out = std::cout);
		std::vector<std::string> checkOutFile(const Key& filename_, std::string& destination,DbCore<T>& db_);

	private:
		void getChildKeys(const Key& filename_, Keys& tempResultSet,DbCore<T>& db_);
		void copy(const std::string& source, const std::string& destination);
	};
	//----< helper function to identiry files>---------------------------
	template <typename T>
	void CheckOut<T>::identify(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
		out << "This will retrieve package files, remove version from the filenames and copy files to a destination folder";
	}
	//----< helper function to child keys for a paritcular file>---------------------------
	template <typename T>
	void CheckOut<T> ::getChildKeys(const Key& filename, Keys& tempResultSet, DbCore<T>& db_)
	{
		if (std::find(tempResultSet.begin(), tempResultSet.end(), filename) != tempResultSet.end())
		{
			return;
		}
		tempResultSet.push_back(filename);
		DbElement<T> temp = db_[filename];
		Children children = temp.children();
		size_t i = 0;
		while (children.size() > i)
		{
			if (children.size() > 0)
			{
				for (auto key : children)
				{
					getChildKeys(key, tempResultSet,db_);
					i++;
				}
			}
		}
	}
	//----< helper function to copy files from one location to another>---------------------------
	template <typename T>
	void CheckOut<T> ::copy(const std::string& source, const std::string& destination)
	{
		bool srcStatus = File::exists(source);
		if (srcStatus == false)
		{
			std::cout << "\nSource file doesn't exist";
			return;
		}
		else if (destination.empty())
		{
			std::cout << "\nSource file doesn't exist";
			return;
		}
		else
			File::copy(source, destination, true);
	}

	
	//----< helper function to that initiate check out process>---------------------------
	template <typename T>
	std::vector<std::string> CheckOut<T> ::checkOutFile(const Key& filename, std::string& destination, DbCore<T>& db)
	{
		std::vector<std::string> checkOutFiles;
		std::string dir = getCurrentWorkingDirectory();
		Keys childKeys;
		DbElement<T> dbEle;
		PayLoad pl;
		getChildKeys(filename, childKeys,db);
		std::sort(childKeys.begin(), childKeys.end());
		childKeys.erase(std::unique(childKeys.begin(), childKeys.end()), childKeys.end());
		for (auto key : childKeys)
			checkOutFiles.push_back(key);
		return checkOutFiles;
	}
}