/////////////////////////////////////////////////////////////////////////
//  XmlElementParts.cpp - Collects tokens for XML analysis             //
//	                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                         //
// Reference: Jim Fawcett                                              //
// Application: NoSQL Database                                         //
// Environment: C++ console                                            // 
// Platform: Lenovo T460                                               // 
// Operating System: Windows 10                                        //
/////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines an XmlParts class.  Its instances build a sequence
of tokens obtained from the Tokenizer module for analysis of XML.
XmlElementParts are lists of tokens that terminate on one of the tokens:
< or >.

This structure is designed to collect just the right amount of inform-
ation from an XML file for analysis.  That is, it collects the smallest
set of tokens that lead to the detection of grammatical constructs,
without having to save part of the sequence for later use or get more
to complete the detection process.  This simplifies the design of code
analysis tools.

Note that assignment and copying of XmlParts instances is supported, using
the default operations provided by the C++ language.  Copies and assignments
result in both source and target XmlParts instances sharing the same toker.

Public Interface:
=================
Toker t;                                  // create tokenizer instance
XmlParts parts(&t);                       // create instance and attach
if(xml.get())                             // collect an XmlElementParts
std::cout << parts.showXmlParts().c_str();  // show it
int n = parts.length();                   // number of tokens in parts
std::string tok = parts[2] = "a change";  // indexed read and write
if(parts.find("aTok") < parts.length())   // search for a token
std::cout << "found aTok";
parts.clear();                            // remove all tokens



* Build Process:
* ---------------
* - Required files: XmlElementParts.h, XmlElementParts.cpp, Tokenizer.h, Tokenizer.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include <algorithm>
#include "XmlElementParts.h"

//----< construct XmlParts instance >---------------------------

XmlParts::XmlParts(Toker* pTokr) : pToker(pTokr), Verbose(false)
{
}
//----< destructor >-------------------------------------------

XmlParts::~XmlParts()
{
}
//----< is this a terminating token? >-------------------------

bool XmlParts::isTerminator(std::string tok)
{
  if(tok.length() == 0)
    return false;
  if(tok == ">")
    return true;
  return false;
}
//
//----< collect an XmlElementParts sequence >------------------

bool XmlParts::get()
{
  toks.clear();
  std::string tok;
  do
  {
    if(pToker->isFileEnd())
    {
      if(length() > 0)
        return true;
      return false;
    }
    tok = pToker->getTok();
    if(Verbose)
    {
      if(tok != "\n")
        std::cout << "\n--tok=" << tok << std::endl;
      else
        std::cout << "\n--tok=newline\n";
    }
    if(toks.size() > 0 && tok == "<")
    {
      pToker->putback('<');
      break;
    }
    if(tok != "\n")
      toks.push_back(tok);
  } while(!isTerminator(tok));
  return true;
}
//----< index operator >---------------------------------------

std::string& XmlParts::operator[](int n)
{
  if(n < 0 || toks.size() <= (size_t)n)
    throw std::exception("XmlParts index out of range");
  return toks[n];
}
//----< collect semi-expression as space-seperated string >----

std::string XmlParts::show()
{
  if(toks.size() == 0)
    return "";
  std::string temp(" ");
  for(size_t i=0; i<toks.size(); ++i)
    if(toks[i] != "\n")
      temp.append(" ").append(toks[i]);
  return temp;
}
//
//----< is tok found in XmlElementParts? >---------------------

int XmlParts::find(const std::string& tok)
{
  for(int i=0; i<length(); ++i)
    if(tok == toks[i])
      return i;
  return length();
}
//----< remove tok if found in semi-expression >---------------

bool XmlParts::remove(const std::string& tok)
{
  std::vector<std::string>::iterator it;
  it = std::find(toks.begin(),toks.end(),tok);
  if(it != toks.end())
  {
    toks.erase(it);
    return true;
  }
  return false;
}
//----< remove leading newlines >------------------------------

void XmlParts::trimFront()
{
  while(toks.size() > 0 && toks[0] == "\n")
    remove("\n");
}
//----< make tokens lowercase >--------------------------------

void XmlParts::toLower()
{
  for(int i=0; i<length(); ++i)
  {
    for(size_t j=0; j<(*this)[i].length(); ++j)
    {
      (*this)[i][j] = tolower((*this)[i][j]);
    }
  }
}
//
//----< test stub >--------------------------------------------

#ifdef TEST_XMLELEMENTPARTS

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing XmlParts class\n "
            << std::string(23,'=') << std::endl;
  std::cout
    << "\n  Note that quotes are returned as single tokens\n\n";

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-') << "\n\n";
    try
    {
      Toker toker(argv[i]);
      toker.setMode(Toker::xml);
      XmlParts parts(&toker);
    //parts.verbose();                 // uncomment to show token details
      while(parts.get())
        std::cout << parts.show().c_str() << std::endl;
      std::cout << "\n\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }
}

#endif
