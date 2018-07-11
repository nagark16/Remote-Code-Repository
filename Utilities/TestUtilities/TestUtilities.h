#pragma once
/////////////////////////////////////////////////////////////////////////
// TestUtilities.h - provides single-user test harness                 //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - TestExecutor    Executes single test in the context of a try-catch block
* - TestExecutive   Uses TestExecutor to run a sequence of tests
*
* Build Process:
* ---------------
* - Required files: Test.h,Test.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include <vector>

/////////////////////////////////////////////////////////////////////
// TestExecutor class
// - supports execution of callable objects for testing in the
//   context of a try-catch block.

template<typename T>
class TestExecutor
{
public:
  bool execute(T t, const std::string& name, std::ostream& out = std::cout);
private:
  void check(bool result, std::ostream& out);
};
//----< execute tests in the context of a try-catch block >----------

template <typename T>
bool TestExecutor<T>::execute(T t, const std::string& name, std::ostream& out)
{
  bool result = false;
  try
  {
    result = t();
    check(result, out);
    out << " -- \"" << name << "\"\n";
  }
  catch (std::exception& ex)
  {
    check(false, out);
    out << " -- \"" << name << "\" ";
    out << ex.what() << "\n";
  }
  return result;
}
//----< display test results >---------------------------------------

template<typename T>
void TestExecutor<T>::check(bool result, std::ostream& out)
{
  if (result)
    out << "  passed";
  else
    out << "  failed";
}

///////////////////////////////////////////////////////////////////////
// TestExecutive class
// - executes a sequence of tests with the help of TestExecutor

class TestExecutive
{
public:
  using Test = std::function<bool()>;
  using TestStr = struct {
    Test test;
    std::string testName;
  };
  using Tests = std::vector<TestStr>;

  bool doTests();
  void registerTest(Test t, const std::string& testName);
  void registerTest(TestStr ts);
private:
  Tests tests_;
};

inline void TestExecutive::registerTest(Test t, const std::string& testName)
{
  TestStr ts{ t, testName };
  tests_.push_back(ts);
}

inline void TestExecutive::registerTest(TestStr ts)
{
  tests_.push_back(ts);
}

inline bool TestExecutive::doTests()
{
  TestExecutor<Test> tester;
  bool result = true;
  for (auto item : tests_)
  {
    bool tResult = tester.execute(item.test, item.testName);
    if (tResult == false)
      result = false;
  }
  return result;
}


