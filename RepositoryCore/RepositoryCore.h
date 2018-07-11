#pragma once
/////////////////////////////////////////////////////////////////////////
// RepositoryCore.h - Manages the whole repository                     //
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
*   This package has 1 class to provide repsitory functionalities
*	And this provide public functionalities
	1. CheckIn -- to check in a file
	2. CheckOut -- to checkout a file
	3. DisplayRepo -- to display content a repo
	4. DisplayAFile -- to display content of file in repo
	5. BrowseAFile -- browse repository the contents of a file
*
* Build Process:
* ---------------
* - Required files: RepositoryCore.h,RepositoryCore.cpp,CheckIn.h,CheckOut.h,Browse.h
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
#include <fstream>
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../DbCore/Definitions.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include "../Browse/Browse.h"
#include "../Version/Version.h"
#include "../Persist/Persist.h"

using namespace NoSqlDb;

namespace Repository {

	template<typename T>
	class RepositoryCore {
	public:
		//RepositoryCore(DbCore<T> & db):repo_(db) {}
		using VersionInfo = std::unordered_map<Key, size_t>;
		RepositoryCore();
		static void identify(std::ostream& out = std::cout);
		bool checkIn(Key key_, DbElement<T> elem_);
		std::vector<std::string> checkOut(const Key& key_,std::string dest);
		void displayRepo();
		void displayAFile(const Key& key_);
		std::vector<std::string> browseAFile(const Key& key_);
		void createDb(NoSqlDb::DbCore<NoSqlDb::PayLoad> & tempRepo_);
		std::vector<std::string> getMetaData(const Key& key_);
		void saveXML();
	private:
		DbCore<T> repo_;
		CheckIn<T> checkIn_;
		Browse<T> browse;
		CheckOut<T>  checkOut_;
		VersionInfo versionInfo_;
	};
	//----< helper function to identiry files>---------------------------
	template<typename T>
	void RepositoryCore<T>::identify(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
		out << " -- This provides means to check-in, version, browse, and check-out source code packages";
	}
	//----< helper function to checkin files>---------------------------
	template<typename T>
	bool RepositoryCore<T>::checkIn(Key key_, DbElement<T> elem_) {
		std::cout << "\nDemonstrating requirement #2: Repository server providing checkin functionality";
		return checkIn_.checkInAFile(key_,elem_,repo_, versionInfo_);
	}
	//----< helper function to check out files>---------------------------
	template<typename T>
	std::vector<std::string> RepositoryCore<T>::checkOut(const Key& key_, std::string dest) {
		std::cout << "\nDemonstrating requirement #2: Repository server providing checkout functionality";
		return checkOut_.checkOutFile(key_, dest,repo_);
	} 
	//----< helper function to display repo>---------------------------
	template<typename T>
	void RepositoryCore<T>::displayRepo() {
		showDb(repo_);
	}
	//----< helper function to save repo as XML>---------------------------
	template<typename T>
	void RepositoryCore<T>::saveXML() {
		std::cout << "\nDemonstrating: Saving to XML";
		Persist<PayLoad> persist(repo_);
		Xml xml = persist.toXml();
		ofstream myfile;
		myfile.open("db.xml");
		myfile << xml;
		myfile.close();
	}
	//----< helper function to display a file>---------------------------
	template<typename T>
	void RepositoryCore<T>::displayAFile(const Key& key_) {
		browse.displayFile(key_,repo_);
	}
	//----< helper function to browse a file>---------------------------
	template<typename T>
	std::vector<std::string> RepositoryCore<T>::browseAFile(const Key& key_) {
		saveXML();
		DbCore<T> tempRepo_;
		createDb(tempRepo_);
		std::cout << "\nDemonstrating requirement #2: Repository server providing browse functionality";
		return browse.browseFile(key_, tempRepo_);
	}

	//----< helper function to browse a file>---------------------------
	template<typename T>
	std::vector<std::string> RepositoryCore<T>::getMetaData(const Key& key_) {
		std::cout << "\nDemonstrating requirement of viewing metadata of a file. In GUI it can be seen in View MetaData";
		DbCore<T> tempRepo_;
		createDb(tempRepo_);
		DbElement<T> elem_ = tempRepo_[key_];
		std::vector<std::string> metaData;
		std::string temp;
		metaData.push_back(temp.append("Name: ").append(elem_.name()));
		temp = "";
		metaData.push_back(temp.append("DateTime: ").append(elem_.dateTime()));
		temp = "";
		metaData.push_back(temp.append("Description: ").append(elem_.descrip()));
		Children children = elem_.children();
		temp = "";
		if (children.size() > 0) {
			
			std::string temp3;
			for (auto key : children)
			{
				temp3.append(key).append(",");
			}
			metaData.push_back(temp.append("Children: ").append(temp3));
		}else
			metaData.push_back(temp.append("Children: ").append("No Children"));
		PayLoad pl = elem_.payLoad();
		temp = "";
		if(pl.isClose())
			metaData.push_back(temp.append("Status: ").append("Closed"));
		else
			metaData.push_back(temp.append("Status: ").append("Open"));
		std::string temp2;
		for (auto cat : pl.categories())
			temp2.append("    " + cat);
		temp = "";
		metaData.push_back(temp.append("Categories: ").append(temp2));
		temp = "";
		metaData.push_back(temp.append("Path: ").append(pl.value()));
		return metaData;
	}
	template<typename T>
	RepositoryCore<T>::RepositoryCore() {
		
	}

	//----< make Repository Test database >-----------------------------------

	void makeTestDb2(NoSqlDb::DbCore<NoSqlDb::PayLoad> & tempRepo_) {
		PayLoad pl;
		pl.value() = "codeRepository\\remoteRepositoryFiles";
		pl.categories().push_back("repositoryCore");
		pl.isClose() = true;
		DbElement<PayLoad> elem;
		elem.name("naga");
		pl.categories().clear();
		pl.categories().push_back("NoSqldb");
		elem.descrip("Provides datetime stamp for database");
		elem.children().clear();
		elem.payLoad(pl);
		tempRepo_["DateTime.cpp.1"] = elem;
		tempRepo_["DateTime.h.1"] = elem;
		elem.descrip("This defines they type of data stored in NoSQL DB");
		elem.children().clear();
		elem.children().push_back("DateTime.cpp.1");
		tempRepo_["PayLoad.cpp.1"] = elem;
		tempRepo_["PayLoad.h.1"] = elem;
		elem.descrip("Provides functionality to save/restore db as XML");
		elem.children().clear();
		tempRepo_["XmlDocument.cpp.1"] = elem;
		tempRepo_["XmlDocument.h.1"] = elem;
		elem.descrip("This is central to NoSQL DB");
		elem.children().clear();
		elem.children().push_back("DateTime.cpp.1");
		elem.children().push_back("PayLoad.cpp.1");
		pl.isClose() = false;
		elem.payLoad(pl);
		tempRepo_["DbCore.cpp.1"] = elem;
		tempRepo_["DbCore.h.1"] = elem;
	}
	//----< make Repository Test database >-----------------------------------
	template<typename T>
	void RepositoryCore<T>::createDb(NoSqlDb::DbCore<NoSqlDb::PayLoad> & tempRepo_)
	{
		std::cout << "\nCreating database to demonstrate browse";
		PayLoad pl;
		pl.value() = "codeRepository\\remoteRepositoryFiles";
		pl.categories().push_back("repositoryCore");
		pl.isClose() = true;
		DbElement<PayLoad> elem;
		elem.name("naga");
		elem.descrip("cpp file to browse the files in repository");
		elem.children().push_back("DateTime.cpp.1");
		elem.payLoad(pl);
		tempRepo_["Browse.cpp.1"] = elem;
		elem.descrip("header file to browse the files in repository");
		tempRepo_["Browse.h.1"] = elem;
		elem.descrip("This is check in files to repository");
		elem.children().clear();
		elem.children().push_back("DateTime.cpp.1");
		tempRepo_["CheckIn.cpp.1"] = elem;
		tempRepo_["CheckIn.h.1"] = elem;
		elem.descrip("This is check out files from repository");
		elem.children().clear();
		tempRepo_["CheckOut.cpp.1"] = elem;
		tempRepo_["CheckOut.h.1"] = elem;
		elem.descrip("This is core for repository");
		elem.children().clear();
		elem.children().push_back("CheckIn.cpp.1");
		elem.children().push_back("CheckOut.cpp.1");
		elem.children().push_back("Browse.cpp.1");
		elem.children().push_back("PayLoad.cpp.1");
		tempRepo_["RepositoryCore.cpp.1"] = elem;
		tempRepo_["RepositoryCore.h.1"] = elem;
		elem.descrip("This is to generate version number for the files in repository");
		elem.children().clear();
		tempRepo_["Version.cpp.1"] = elem;
		tempRepo_["Version.h.1"] = elem;
		makeTestDb2(tempRepo_);
	}
}