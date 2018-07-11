#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
/////////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface        //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
Module Purpose:
===============
ITokCollection is an interface that supports substitution of different
types of scanners for parsing.  In this solution, we illustrate that
by binding two different types of collections, SemiExp and XmlParts,
to this interface.  This is illustrated in the test stubs for the
SemiExpression and XmlElementParts modules.

Note that we only use the SemiExpression collector in the parser and
all of the higher level executives.  We certainly could bind the
XmlParts class to parser's ITokCollector pointer, but didn't due to
lack of time.

Build Process:
* ---------------
* - Required files: itokcollection.h
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include <string>

struct ITokCollection
{
  virtual bool get()=0;
  virtual int length()=0;
  virtual std::string& operator[](int n)=0;
  virtual int find(const std::string& tok)=0;
  virtual void push_back(const std::string& tok)=0;
  virtual bool remove(const std::string& tok)=0;
  virtual void toLower()=0;
  virtual void trimFront()=0;
  virtual void clear()=0;
  virtual std::string show()=0;
  virtual ~ITokCollection() {};
};

#endif
