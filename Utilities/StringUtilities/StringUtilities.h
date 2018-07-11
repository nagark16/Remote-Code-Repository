#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H
/////////////////////////////////////////////////////////////////////////
// StringUtilities.h - small, generally useful, helper classes         //
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
* This package provides functions:
* - Title(text)           display title
* - title(text)           display subtitle
* - putline(n)            display n newlines
* - trim(str)             remove leading and trailing whitespace
* - split(str, 'delim')   break string into vector of strings separated by delim char
* - showSplit(vector)     display splits
*
* Build Process:
* ---------------
* - Required files: StringUtilities.h,StringUtilities.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <locale>

namespace Utilities
{
  /////////////////////////////////////////////////////////////////////
  // String Helper functions
 
  //----< display underlined title >-----------------------------------

  inline void Title(const std::string& text, std::ostream& out = std::cout, char underline = '=')
  {
    out << "\n  " << text;
    out << "\n " << std::string(text.size() + 2, underline);
  }
  //----< display underlined subtitle >--------------------------------

  inline void title(const std::string& text, std::ostream& out = std::cout, char underline = '-')
  {
    out << "\n  " << text;
    out << "\n " << std::string(text.size() + 2, underline);
  }
  //----< display j newlines >-----------------------------------------

  inline void putline(size_t j = 1, std::ostream& out = std::cout)
  {
    for (size_t i = 0; i < j; ++i)
      out << "\n";
  }
  /*--- remove whitespace from front and back of string argument ---*/
  /*
  *  - does not remove newlines
  */
  template <typename T>
  inline std::basic_string<T> trim(const std::basic_string<T>& toTrim)
  {
    if (toTrim.size() == 0)
      return toTrim;
    std::basic_string<T> temp;
    std::locale loc;
    typename std::basic_string<T>::const_iterator iter = toTrim.begin();
    while (isspace(*iter, loc) && *iter != '\n')
    {
      if (++iter == toTrim.end())
      {
        break;
      }
    }
    for (; iter != toTrim.end(); ++iter)
    {
      temp += *iter;
    }
    typename std::basic_string<T>::reverse_iterator riter;
    size_t pos = temp.size();
    for (riter = temp.rbegin(); riter != temp.rend(); ++riter)
    {
      --pos;
      if (!isspace(*riter, loc) || *riter == '\n')
      {
        break;
      }
    }
    if (0 <= pos && pos < temp.size())
      temp.erase(++pos);
    return temp;
  }

  /*--- split sentinel separated strings into a vector of trimmed strings ---*/

  template <typename T>
  inline std::vector<std::basic_string<T>> split(const std::basic_string<T>& toSplit, T splitOn = ',')
  {
    std::vector<std::basic_string<T>> splits;
    std::basic_string<T> temp;
    typename std::basic_string<T>::const_iterator iter;
    for (iter = toSplit.begin(); iter != toSplit.end(); ++iter)
    {
      if (*iter != splitOn)
      {
        temp += *iter;
      }
      else
      {
        splits.push_back(trim(temp));
        temp.clear();
      }
    }
    if (temp.length() > 0)
      splits.push_back(trim(temp));
    return splits;
  }
  /*--- show collection of string splits ------------------------------------*/

  template <typename T>
  inline void showSplits(const std::vector<std::basic_string<T>>& splits, std::ostream& out = std::cout)
  {
    out << "\n";
    for (auto item : splits)
    {
      if (item == "\n")
        out << "\n--" << "newline";
      else
        out << "\n--" << item;
    }
    out << "\n";
  }
}
#endif
