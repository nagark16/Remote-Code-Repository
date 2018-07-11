#pragma once
/////////////////////////////////////////////////////////////////////////
// Version.h - Manages the version number to checkin files             //
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
*   This package has 1 Version class which provides 2 public methods
*		1. getVersionInfo -- This method will give current version number of a particular file
*		2. incrementVersion -- This method will increment version of input file
*
*
* Build Process:
* ---------------
* - Required files: Version.h,Version.cpp
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
#include <unordered_map>
#include "../DbCore/Definitions.h"

namespace Repository {

	class Version {
	public:
		using Key = std::string;

		static void identify(std::ostream& out = std::cout);
		size_t getVersionInfo(Key& key , std::unordered_map<Key, size_t> &versionInfo);
		size_t incrementVersion(Key& key, std::unordered_map<Key, size_t> &versionInfo);
	};
	//----< helper function to identiry files>---------------------------
	void Version::identify(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
		out << " -- This package will manages verion of files checkedin to repository";
	}
	//----< helper function to get version info of file>---------------------------
	inline size_t Version::getVersionInfo(Key& key, std::unordered_map<Key, size_t> &versionInfo_) {
		size_t count = std::count(key.begin(), key.end(), '.');
		if(count == 2)
			key = key.substr(0, key.find_last_of("."));
		std::unordered_map<Key, size_t>::iterator iter = versionInfo_.find(key);
		if (iter != versionInfo_.end())
			return versionInfo_[key];
		else
			return 0;
	}
	//----< helper function to increment version of particular file>---------------------------
	inline size_t Version::incrementVersion(Key& key, std::unordered_map<Key, size_t> &versionInfo_) {
		size_t count = std::count(key.begin(), key.end(), '.');
		if (count == 2)
			key = key.substr(0, key.find_last_of("."));
		std::unordered_map<Key, size_t>::iterator iter = versionInfo_.find(key);
		if (iter != versionInfo_.end()) {
			versionInfo_[key] = versionInfo_[key] + 1;
			return versionInfo_[key];
		}
		else {
			versionInfo_[key] = 1;
			return 1;
		}
	}
}