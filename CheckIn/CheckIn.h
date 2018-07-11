#pragma once

/////////////////////////////////////////////////////////////////////////
// CheckIn.h - Manages checkin to repository                           //
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
*   This package provides one CheckIn class which manages checkin to repository
*	It has one public function checkInAFile to checkinfile with version number 
*	appended to file name
* 
*
*
* Build Process:
* ---------------
* - Required files: CheckIn.h,CheckIn.cpp,DbCore.h,Version.h,XMLDocument,DateTime
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
#include "../DbCore/Definitions.h"

#include "../DbCore/DbCore.h"
#include "../FileSystem/FileSystem.h"
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

using namespace NoSqlDb;
using namespace std;


namespace Repository 
{

	template<typename T>
	class CheckIn {
	public:
		bool copyAFileForCheckIn(std::string filename, size_t version, DbElement<T>& dbElem);
		static void identify(std::ostream& out = std::cout);
		bool checkInAFile(Key key_, DbElement<T>& ele, DbCore<T>& db_, std::unordered_map<Key, size_t>& versionInfo_);
	private:
		bool checkChildrenCheckIn(Children& child, DbCore<T>& db_, std::unordered_map<Key, size_t>& versionInfo_);
		void updateDB(string newKey, DbElement<T>& ele, DbCore<T>& db_);
		bool isCyclicDependent(DbCore<T>& db_, Key key_, std::unordered_map<Key, size_t>& versionInfo_,Children childs);
	};
	//----< helper function to identiry files>---------------------------
	template<typename T>
	void CheckIn<T>::identify(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
		out << " -- This will provides the functionality to start a new package version"
			<<"\n\t by accepting files, appending version numbers to their filenames, "
			<<"\n\t providing dependency and category information, creating metadata, "
			<<"\n\t and storing the files in a designated location";
	}
	//----< helper function to check in a file>---------------------------
	template<typename T>
	bool CheckIn<T>::checkInAFile(Key key_, DbElement<T>& ele, DbCore<T>& db_, std::unordered_map<Key, size_t>& versionInfo_) {
		Version ver;
		size_t fileVersion;
		std::string filename = key_;
		std::string filename2 = key_;
		if (checkChildrenCheckIn(ele.children(), db_, versionInfo_)) {
			if (ele.payLoad().isClose()) {
				ele.payLoad().status() = "Closed";
				fileVersion = ver.incrementVersion(key_, versionInfo_);
				updateDB(key_.append(".").append(to_string(fileVersion)), ele, db_);
				copyAFileForCheckIn(filename, fileVersion, ele);
			}
			else if (!ele.payLoad().isClose()) {
				ele.payLoad().status() = "Open";
				fileVersion = ver.getVersionInfo(key_,versionInfo_);
				updateDB(key_.append(".").append(to_string(fileVersion + 1)), ele, db_);
				copyAFileForCheckIn(filename, fileVersion + 1, ele);
			}
		}
		else {
			ele.payLoad().status() = "Open";
			if(ele.children().size() > 0 && ele.payLoad().isClose() && isCyclicDependent(db_,key_, versionInfo_,ele.children()))
				ele.payLoad().status() = "PendingClose";
			ele.payLoad().isClose() = false;
			fileVersion = ver.getVersionInfo(key_, versionInfo_);
			updateDB(key_.append(".").append(to_string(fileVersion + 1)), ele, db_);
			copyAFileForCheckIn(filename, fileVersion + 1, ele);
		}
		
		return db_.contains(key_);
	}

	//----< Get current directory >------------------------------------------
	inline std::string getCurrentWorkingDirectory(void) {
		char buff[FILENAME_MAX];
		GetCurrentDir(buff, FILENAME_MAX);
		std::string current_working_dir(buff);
		return current_working_dir;
	}
	//----< helper function to check cyclic dependency>---------------------------
	template<typename T>
	bool CheckIn<T>::isCyclicDependent(DbCore<T>& db_, Key key_, std::unordered_map<Key, size_t>& versionInfo_, Children childs) {
		Version ver;
		size_t fileVersion;
		for (auto child : childs) {
			fileVersion = ver.getVersionInfo(child, versionInfo_);
			if (fileVersion == 0)
				fileVersion = 1;
			std::string temp = child;
			if (db_.contains(temp.append(".").append(to_string(fileVersion)))) {
				Children childChilds = db_[child.append(".").append(to_string(fileVersion))].children();
				for (auto childChild : childChilds) {
					if (childChild.compare(key_) == 0)
						return true;
				}
			}
		}
		return false;
	}
	//----< helper function to convert files>---------------------------
	template<typename T>
	bool CheckIn<T>::copyAFileForCheckIn(std::string filename, size_t version, DbElement<T>& dbElem) {
		std::cout << "\ncopying a file internally";
		PayLoad p = dbElem.payLoad();
		std::string path = p.value();

		std::string dir = getCurrentWorkingDirectory();
		size_t val = dir.find("ServerPrototype");
		std::string newPath = "..\\";
		if (val != std::string::npos) {
			path = newPath.append(path);
		}
			
		std::string destPath = path;
		std::string srcPath = path.append("\\").append(filename);
		destPath = destPath.append("\\").append(filename).append(".").append(to_string(version));
		FileSystem::File::copy(srcPath, destPath, false);
		FileSystem::File::remove(srcPath);
		return true;
	}
	//----< helper function to check children before check in >---------------------------
	template<typename T>
	bool CheckIn<T>::checkChildrenCheckIn(Children& child, DbCore<T> & db_, std::unordered_map<Key, size_t>& versionInfo_) {
		Version ver;
		size_t fileVer;
		for (auto ch : child) {
			fileVer = ver.getVersionInfo(ch, versionInfo_);
			if (fileVer == 0)
				return false;

			if (db_.contains(ch.append(".").append(to_string(fileVer+1))))
				return false;

			if (db_.contains(ch.append(".").append(to_string(fileVer)))) {
				DbElement<T> dbele = db_[ch.append(".").append(to_string(fileVer))];
				if (!dbele.payLoad().isClose())
					return false;
			}
			
		}
		return true;
	}
	//----< helper function to update DB with new check in>---------------------------
	template<typename T>
	void CheckIn<T>::updateDB(string newKey, DbElement<T>& ele, DbCore<T>& db_) {
		DbElement<T> temp;
		if (db_.contains(newKey)) {
			temp = db_[newKey];
			if (temp.name().compare(ele.name()) == 0) {
				db_[newKey] = ele;
			}
		}
		else {
			db_[newKey] = ele;
		}
	}
}