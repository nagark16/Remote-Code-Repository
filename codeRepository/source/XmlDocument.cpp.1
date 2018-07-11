/////////////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes                   //
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
* This package is intended to help students in CSE687 - Object Oriented Design
* get started with Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes but no children
*
* Build Process:
* ---------------
* - Required files: XmlDocument.h, XmlDocument.cpp, XmlElement.h, XmlElement.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include <iostream>
#include <functional>
#include "XmlDocument.h"
#include "..\XmlParser\XmlParser.h"
#include "../Utilities/Utilities.h"

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement > ;

/////////////////////////////////////////////////////////////////////////////
// Definitions of XmlDocument methods

XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType)
{
  XmlParser parser(src, (XmlParser::sourceType) srcType);
  XmlDocument* pDoc = parser.buildDocument();
  *this = std::move(*pDoc);
}
//----< move constructor >---------------------------------------------------

XmlDocument::XmlDocument(XmlDocument&& doc)
{
  pDocElement_ = doc.pDocElement_;
  doc.pDocElement_ = nullptr;
}
//----< move assignment >----------------------------------------------------

XmlDocument& XmlDocument::operator=(XmlDocument&& doc)
{
  if (&doc == this) return *this;
  pDocElement_ = doc.pDocElement_;
  doc.pDocElement_ = nullptr;
  return *this;
}
//----< return std::shared_ptr to XML root >---------------------------------

sPtr XmlDocument::xmlRoot()
{
  for (auto pElem : pDocElement_->children())
  {
    if (dynamic_cast<TaggedElement*>(pElem.get()))
      return pElem;
  }
  return nullptr;
}
//----< add XML root to an XmlDocument >-------------------------------------

bool XmlDocument::xmlRoot(sPtr pRoot)
{
  return pDocElement_->addChild(pRoot);
}
//----< find element(s) with this tag >--------------------------------------
/*
 *  if tag == "" returns pElem and all decendents
 */
bool XmlDocument::find(const std::string& tag, sPtr pElem, bool findall)
{
  if (pElem->tag() == tag || tag == "")
  {
    found_.push_back(pElem);
    if (!findall)
      return true;
  }
  for (auto pChild : pElem->children())
    find(tag, pChild);
  return (found_.size() > 0);
}
//----< find element with this tag >-----------------------------------------
/*
 *  found_[0] contains first element (DFS order) with tag, else empty
 */
XmlDocument& XmlDocument::element(const std::string& tag)
{
  found_.clear();
  find(tag, pDocElement_, false);
  return *this;
}
//----< find children of element with this tag >-----------------------------
/*
 * found_ contains all children found, else empty
 */
XmlDocument& XmlDocument::elements(const std::string& tag)
{
  found_.clear();
  element(tag);
  if (found_.size() > 0)
  {
    sPtr pElem = found_[0];
    found_.clear();                         // don't keep parent element
    for (auto pChild : pElem->children())
      found_.push_back(pChild);             // save children
  }
  return *this;
}
//----< find all decendents of last find with this tag >---------------------
/*
 *  returns all decendents if tag == ""
 */
XmlDocument& XmlDocument::descendents(const std::string& tag)
{
  if (found_.size() == 0)
    found_.push_back(xmlRoot());
  sPtr pElem = found_[0];
  found_.clear();
  for (auto pChild : pElem->children())
    find(tag, pChild, true);
  return *this;
}
//----< return found results >-----------------------------------------------

std::vector<sPtr> XmlDocument::select()
{
  return std::move(found_);  // returns results and clears found_
}
//----< return the number of elements in the document >----------------------

size_t XmlDocument::size()
{
  find("", pDocElement_, true);
  size_t size_ = found_.size() - 1;  // don't count docElement
  found_.clear();
  return size_;
}
//----< return XML string representation of XmlDocument >--------------------

std::string XmlDocument::toString()
{
  return pDocElement_->toString();
}

std::string enQuote(std::string s) { return "\"" + s + "\""; }

#ifdef TEST_XMLDOCUMENT

/////////////////////////////////////////////////////////////////////////////
// Test Functions

//----< build an XmlDocument for testing >-----------------------------------

XmlDocument buildDocument()
{
  sPtr pRoot = makeTaggedElement("root");
  XmlDocument doc(XmlProcessing::makeDocElement(pRoot));

  sPtr child1 = makeTaggedElement("child1");
  child1->addChild(makeTextElement("child1 text"));
  sPtr grandChild11 = makeTaggedElement("grandChild11");
  grandChild11->addChild(makeTextElement("grandchild11 text"));
  child1->addChild(grandChild11);
  pRoot->addChild(child1);
  sPtr secondChild1 = makeTaggedElement("child1");
  secondChild1->addChild(makeTextElement("text of second child1"));
  child1->addChild(secondChild1);

  sPtr child2 = makeTaggedElement("child2");
  child2->addChild(makeTextElement("child2 text"));
  pRoot->addChild(child2);
  return doc;
}
//----< test Depth First Search on XmlDocument >-----------------------------

void testDFS(XmlDocument& doc)
{
  title("testing global DFS on XmlDocument - printing tags");

  std::function<void(AbstractXmlElement&)> f;
  f = [](AbstractXmlElement& Elem) {
    if (Elem.tag().size() > 0)
      std::cout << "\n  " << Elem.tag();
  };
  
  DFS(doc, f);
  std::cout << "\n";
}
//----< test search for element with specified tag >-------------------------

void testElement(XmlDocument& doc)
{
  std::string testTag = "child1";
  title("testing element(" + enQuote(testTag) + ")");
  std::vector<sPtr> found = doc.element(testTag).select();
  if (found.size() > 0)
    std::cout << "\n  found " << found[0]->tag() << ", " << found[0]->value();
  else
    std::cout << "\n  couldn't find " + enQuote(testTag);
  std::cout << "\n";

  testTag = "foobar";
  title("testing element(" + enQuote(testTag) + ")");
  found = doc.element(testTag).select();
  if (found.size() > 0)
    std::cout << "\n  found " << found[0]->tag();
  else
    std::cout << "\n  couldn't find " + enQuote(testTag);
  std::cout << "\n";
}
//----< test search for children of element with specified tag >-------------

void testElements(XmlDocument& doc)
{
  std::string testTag = "child1";
  title("testing elements(" + enQuote(testTag) + ")");
  std::vector<sPtr> found = doc.elements(testTag).select();
  if (found.size() > 0)
  {
    for (auto pElem : found)
    {
      if (pElem->tag() != "")
        std::cout << "\n  found: " << pElem->tag();
      else
        std::cout << "\n  found: " << pElem->value();
    }
  }
  else
    std::cout << "\n  couldn't find child elements";
  std::cout << "\n";
}
//----< test search for all decendents of xmlRoot with specified tag >-------

void testDescendents(XmlDocument& doc)
{
  std::string testTag = "child1";
  title("testing doc.descendents(" + enQuote(testTag) + ")");
  std::vector<sPtr> found = doc.descendents(testTag).select();
  if (found.size() > 0)
  {
    for (auto pElem : found)
    {
      if (pElem->tag() != "")
        std::cout << "\n  found: " << pElem->tag();
      else
        std::cout << "\n  found: " << pElem->value();
    }
  }
  else
    std::cout << "\n  couldn't find descendent elements";
  std::cout << "\n";
}
//----< test search for all decendents of specified element >----------------

void testElementDescendents(XmlDocument& doc)
{
  std::string testTag = "child1";
  title("testing doc.element(" + testTag + ").descendents()");
  std::vector<sPtr> found = doc.element(testTag).descendents().select();
  if (found.size() > 0)
  {
    for (auto pElem : found)
    {
      if (pElem->tag() != "")
        std::cout << "\n  found: " << pElem->tag();
      else
        std::cout << "\n  found: " << pElem->value();
    }
  }
  else
    std::cout << "\n  couldn't find descendent elements";
  std::cout << "\n";
}
//----< test stub >----------------------------------------------------------

int main()
{
  title("Testing XmlDocument class");
  
  XmlDocument doc = buildDocument();
  std::cout << doc.toString();
  std::cout << "\n";
  std::cout << "\n  size of document = " << doc.size() << "\n";

  title("Testing XmlDocument::DFS - looking for tags");
  std::function<void(AbstractXmlElement&)> f;
  f = [](AbstractXmlElement& Elem) {
    if (Elem.tag().size() > 0)
      std::cout << "\n  " << Elem.tag();
  };
  doc.DFS(doc.xmlRoot(), f);
  std::cout << "\n";
   
  testDFS(doc);
  testElement(doc);
  testElements(doc);
  testDescendents(doc);
  testElementDescendents(doc);

  std::string path = "../XmlElementParts/LectureNote.xml";
  title("Attempting to build document from fileSpec: " + path);

  try
  {
    XmlDocument doc2(path, XmlDocument::file);
    std::cout << doc2.toString();
  }
  catch (std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what();
  }
  std::cout << "\n\n";
}

#endif
