#ifndef TOKENIZER_H
#define TOKENIZER_H
/////////////////////////////////////////////////////////////////////////
//  Tokenizer.h - Reads words from a file                              //
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
This module defines a tokenizer class.  Its instances read words from
an attached file or string.  Word boundaries occur when a character sequence
read from the file or string:
- changes between any of the character types: alphanumeric, punctuator,
or white space.
- certain characters are treated as single character tokens, e.g.,
"(", ")", "{", "}", "[". "]", ";", ".", and "\n".
You can change the selection with Toker::setMode(Toker::code) or
Toker::setMode(Toker::xml), or Toker::setSingleCharTokens(somestring);
- Toker::code is the default tokenizing mode.
A tokenizer is an important part of a scanner, used to read and interpret
source code or XML.

Public Interface:
=================
Toker t;                        // create tokenizer instance
returnComments();               // request comments return as tokens
if(t.attach(someFileName))      // select file for tokenizing
string tok = t.getTok();      // extract first token
int numLines = t.lines();       // return number of lines encountered
t.lines() = 0;                  // reset line count

Build Process:
* ---------------
* - Required files: tokenizer.h, tokenizer.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/
#include <string>
#include <iostream>
#include <vector>

class Toker
{
public:
  enum mode { code, xml, custom };
  Toker(const std::string& src = "", bool isFile = true);
  ~Toker();
  void setMode(mode md);
  void setSingleCharTokens(std::string tokChars);
  bool attach(const std::string& filename, bool isFile = true);
  std::string getTok();
  void returnComments(bool doReturn = true);
  int& lines();
  int  braceLevel();
  bool isFileEnd();
  int  peek();           // peek at next char (not token)
  void putback(int ch);  // put back char on stream (not token)
  enum state { default_state, comment_state, quote_state };

private:
  std::istream* pIn;
  char prevprevChar, prevChar, currChar, nextChar;
  std::string scTok;
  std::vector<char> putbacks;
  int numLines;
  int braceCount;
  bool doReturnComments;
  bool aCppComment;
  enum state _state;
  mode _mode;

  // private helper functions
  int  get();
  bool getChar();
  bool isSingleCharTok(char ch);
  bool isTokEnd();
  void stripWhiteSpace();
  bool isIdentifierChar(char ch);
  bool isBeginComment();
  bool isEndComment();
  std::string eatComment();
  bool aSingleQuote;
  bool isBeginQuote();
  bool isEndQuote();
  std::string eatQuote();

  // prohibit copying and assignment
  Toker(const Toker &tkr);
  Toker& operator=(const Toker&);
};

inline void Toker::returnComments(bool doReturn)
{
  doReturnComments = doReturn;
}

inline bool Toker::isFileEnd() { return (nextChar == -1); }

inline int& Toker::lines() { return numLines; }

inline int Toker::braceLevel() { return braceCount; }

#endif
