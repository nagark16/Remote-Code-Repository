#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally usefule, helper classes            //
//                                                                   //
// Author: Naga Rama Krishna, nrchalam@syr.edu                       //
// Reference: Jim Fawcett                                            //
// Application: RepositoryApp                                        //
// Environment: C++ console                                          // 
// Platform: Lenovo T460                                             // 
// Operating System: Windows 10                                      //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for 
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1: 6th April 2018
*
*/
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>

namespace Utilities
{
  ///////////////////////////////////////////////////////////////////
  // Utilities for std::string
  // - split accepts comma separated list of items and returns
  //   std::vector containing each item
  // - Title writes src string to console with underline of '=' chars
  // - title writes src string to console with underline of '-' chars

  class StringHelper
  {
  public:
    static std::vector<std::string> split(const std::string& src);
    static void Title(const std::string& src, char underline = '=');
    static void title(const std::string& src);
    static std::string trim(const std::string& src);
    static std::string addHeaderAndFooterLines(const std::string& src);
  };

  ///////////////////////////////////////////////////////////////////
  // function writes return to console

  void putline();

  ///////////////////////////////////////////////////////////////////
  // DisplayLocation writes start address, ending address and size
  // to console

  std::string ToDecAddressString(size_t address);
  std::string ToHexAddressString(size_t address);

  template<typename T>
  void DisplayLocation(T& t)
  {
    size_t address = reinterpret_cast<size_t>(&t);
    size_t size = sizeof(t);

    std::cout << ToDecAddressString(address) 
              << " : " 
              << ToDecAddressString(address + size)
              << ", " << size;
  }

  ///////////////////////////////////////////////////////////////////
  // Converter converts template type T to and from string

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }
}
#endif
