#pragma once
/////////////////////////////////////////////////////////////////////////
// TestClass.h - defines all testing for NoSqlDb                       //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a TestClass class that implements all testing
*  for the NoSqlDb.  It is an alternative to the TestUtilities demonstrated
*  in class for the NoSqlDb prototype.
*
*  Required Files:
*  ---------------
*  TestClass.h, TestClass.cpp
*  DbCore.h, DbCore,cpp
*  DateTime.h, DateTime.cpp
*  PayLoad.h
*  Query.h, Query.cpp
*  StringUtilities.h, StringUtilities.cpp
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 10 Feb 2018
*  - first release
*/

#include <string>
#include <iostream>
#include <vector>
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../Browse/Browse.h"
#include "../Version/Version.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include "../RepositoryCore/RepositoryCore.h"

#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Persist/Persist.h"
#include "../Edit/Edit.h"
#include "../Executive/Executive.h"
#include <functional>


using namespace NoSqlDb;
using namespace Repository;


namespace TestClass {

	class TestDb
	{
	public:
		using MPtr = bool(TestDb::*)();

		TestDb(NoSqlDb::DbCore<NoSqlDb::PayLoad>& db) : db_(db) {}
		void makeTestDb(NoSqlDb::DbCore<NoSqlDb::PayLoad> & db_);
		static void identity(std::ostream& out = std::cout);
		bool invoke(MPtr mPtr);
		bool testR1();
		bool testR2();
		bool testR3();
		bool testR4();
		bool testR5();
		bool testR6();
		bool testR7();

	private:
		NoSqlDb::DbCore<NoSqlDb::PayLoad>& db_;

	};

	inline void TestDb::identity(std::ostream& out)
	{
		out << "\n  \"" << __FILE__ << "\"";
	}

	inline bool TestDb::invoke(TestDb::MPtr mPtr)
	{
		try {
			bool result = ((*this).*mPtr)();
			if (result)
			{
				std::cout << "\npassed <<<<\n";
			}
			else
			{
				std::cout << "\nfailed <<<<\n";
			}
			return result;
		}
		catch (std::exception& ex)
		{
			std::cout << "\n    exception thrown:";
			std::cout << "\n    " << ex.what();
			std::cout << "\nfailed <<<<\n";
			return false;
		}
	}

	//----< make Repository Test database >-----------------------------------

	inline void makeTestDb2(NoSqlDb::DbCore<NoSqlDb::PayLoad> & db_) {
		PayLoad pl;
		pl.value() = "codeRepository/source";
		pl.categories().push_back("repositoryCore");
		pl.isClose() = true;
		DbElement<PayLoad> elem;
		elem.name("naga");
		pl.categories().clear();
		pl.categories().push_back("NoSqldb");
		elem.descrip("Provides datetime stamp for database");
		elem.children().clear();
		elem.payLoad(pl);
		db_["DateTime.cpp.1"] = elem;
		db_["DateTime.h.1"] = elem;
		elem.descrip("This defines they type of data stored in NoSQL DB");
		elem.children().clear();
		elem.children().push_back("DbCore.cpp.1");
		db_["PayLoad.cpp.1"] = elem;
		db_["PayLoad.h.1"] = elem;
		elem.descrip("Provides functionality to save/restore db as XML");
		elem.children().clear();
		db_["XmlDocument.cpp.1"] = elem;
		db_["XmlDocument.h.1"] = elem;
		elem.descrip("This is central to NoSQL DB");
		elem.children().clear();
		elem.children().push_back("DateTime.cpp.1");
		elem.children().push_back("PayLoad.cpp.1");
		pl.isClose() = false;
		elem.payLoad(pl);
		db_["DbCore.cpp.1"] = elem;
		db_["DbCore.h.1"] = elem;
	}
	//----< make Repository Test database >-----------------------------------
	inline void TestDb::makeTestDb(NoSqlDb::DbCore<NoSqlDb::PayLoad> & db_)
	{
		PayLoad pl;
		pl.value() = "codeRepository/source";
		pl.categories().push_back("repositoryCore");
		pl.isClose() = true;
		DbElement<PayLoad> elem;
		elem.name("naga");
		elem.descrip("cpp file to browse the files in repository");
		elem.children().push_back("DateTime.cpp.1");
		elem.payLoad(pl);
		db_["Browse.cpp.1"] = elem;
		elem.descrip("header file to browse the files in repository");
		db_["Browse.h.1"] = elem;
		elem.descrip("This is check in files to repository");
		elem.children().clear();
		elem.children().push_back("DateTime.cpp.1");
		db_["CheckIn.cpp.1"] = elem;
		db_["CheckIn.h.1"] = elem;
		elem.descrip("This is check out files from repository");
		elem.children().clear();
		db_["CheckOut.cpp.1"] = elem;
		db_["CheckOut.h.1"] = elem;
		elem.descrip("This is core for repository");
		elem.children().clear();
		elem.children().push_back("CheckIn.cpp.1");
		elem.children().push_back("CheckOut.cpp.1");
		elem.children().push_back("Browse.cpp.1");
		elem.children().push_back("PayLoad.cpp.1");
		db_["RepositoryCore.cpp.1"] = elem;
		db_["RepositoryCore.h.1"] = elem;
		elem.descrip("This is to generate version number for the files in repository");
		elem.children().clear();
		db_["Version.cpp.1"] = elem;
		db_["Version.h.1"] = elem;
		makeTestDb2(db_);
	}
}
