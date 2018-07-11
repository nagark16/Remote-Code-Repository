/////////////////////////////////////////////////////////////////////////
// XmlElement.cpp - define XML Element types                           //
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
* Abstract Syntax Trees are unordered trees with two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in this package will be used in the AST defined in
* the AbstractSyntaxTree package, included in this Visual Studio 2013
* solution.
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration
*
* Build Process:
* ---------------
* - Required files: XmlElement.h, XmlElement.cpp
* - Compiler command: devenv NoSqlDb.sln /rebuild debug
*
* Maintenance History:
*  --------------------
*  ver 1.0 : 4th Feb 2018
*  - first release
*/

#include "XmlElement.h"
#include <iostream>

using namespace XmlProcessing;

size_t AbstractXmlElement::count = 0;
size_t AbstractXmlElement::tabSize = 2;

//////////////////////////////////////////////////////////////////////////
// Global Factory methods

//----< factory for doc elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeDocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
  std::shared_ptr<AbstractXmlElement> ptr(new DocElement(pRoot));
  return ptr;
}

//----< factory for tagged elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeTaggedElement(const std::string& tag, const std::string& text)
{
  std::shared_ptr<AbstractXmlElement> ptr(new TaggedElement(tag));
  if (text.size() > 0)
  {
    std::shared_ptr<AbstractXmlElement> ptrText(new TextElement(text));
    ptr->addChild(ptrText);
  }
  return ptr;
}
//----< factory for text elements >------------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeTextElement(const std::string& text)
{
  std::shared_ptr<AbstractXmlElement> ptr(new TextElement(text));
  return ptr;
}
//----< factory for comment elements >---------------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeCommentElement(const std::string& text)
{
  std::shared_ptr<AbstractXmlElement> ptr(new CommentElement(text));
  return ptr;
}
//----< factory for processing instruction elements >------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeProcInstrElement(const std::string& text)
{
  std::shared_ptr<AbstractXmlElement> ptr(new ProcInstrElement(text));
  return ptr;
}
//----< factory for XML Declaration elements >-------------------------------

std::shared_ptr<AbstractXmlElement> XmlProcessing::makeXmlDeclarElement()
{
  std::shared_ptr<AbstractXmlElement> ptr(new XmlDeclarElement());
  return ptr;
}
/////////////////////////////////////////////////////////////////////////////
// Derived class method definitions
//
// DocElement methods

//----< doc element constructor and destructor >-----------------------------

DocElement::DocElement(std::shared_ptr<AbstractXmlElement> pRoot) 
{
  //std::shared_ptr<AbstractXmlElement> pDecl = makeXmlDeclarElement();
  //pDecl->addAttrib("version", "1.0");
  //children_.push_back(pDecl);
  if (pRoot != nullptr)
    children_.push_back(pRoot);
}
//----< private helper for DocElement::addChild(...) >-----------------------

bool DocElement::hasXmlRoot()
{
  for (auto pElement : children_)
  {
    if (dynamic_cast<TaggedElement*>(pElement.get()) != nullptr)
      return true;
  }
  return false;
}
//----< add only one child to doc element using pointer to child >-----------

bool DocElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
  // don't add TextElements
  TextElement* pTxEl = dynamic_cast<TextElement*>(pChild.get());
  if (pTxEl != 0)  // don't add text elements to DocElement
    return false;

  // don't add another DocElement
  DocElement* pDcEl = dynamic_cast<DocElement*>(pChild.get());
  if (pDcEl != 0)  // don't add text elements to DocElement
    return false;

  // add ProcInstrElements and CommentElements 
  TaggedElement* te = dynamic_cast<TaggedElement*>(pChild.get());
  if (te == nullptr) // is not a TaggedElement
  {
    children_.push_back(pChild);
    return true;
  }

  // add only one TaggedElement
  if (!hasXmlRoot())
  {
    children_.push_back(pChild);
    return true;
  }
  return false;
}
//----< remove child from doc element using pointer to child >---------------

bool DocElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
  auto iter = std::find(begin(children_), end(children_), pChild);
  if (iter != end(children_))
  {
    children_.erase(iter);
    return true;
  }
  return false;
}
//----< return value = tag from doc element >-----------------------------

std::string DocElement::value() { return std::string(""); }

//----< generate xml string for tagged element >-----------------------------

std::string DocElement::toString()
{
  std::string rtn;
  for (auto elem : children_)
    rtn += elem->toString();
  return rtn;
}
/////////////////////////////////////////////////////////////////////////////
// TaggedElement methods

//----< add child to tagged element using pointer to child >-----------------

bool TaggedElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
  children_.push_back(pChild);
  return true;
}
//----< remove child from tagged element using pointer to child >------------

bool TaggedElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
  auto iter = std::find(begin(children_), end(children_), pChild);
  if (iter != end(children_))
  {
    children_.erase(iter);
    return true;
  }
  return false;
}
//----< add attribute to tagged element >------------------------------------

bool TaggedElement::addAttrib(const std::string& name, const std::string& value)
{
  std::pair<std::string, std::string> atPair(name, value);
  attribs_.push_back(atPair);
  return true;
}
//----< remove attribute from tagged element >-------------------------------

bool TaggedElement::removeAttrib(const std::string& name)
{
  for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
  {
    if (iter->first == name)
    {
      attribs_.erase(iter);
      return true;
    }
  }
  return false;
}
//----< return value = tag from tagged element >-----------------------------

std::string TaggedElement::value() { return tag_; }

//----< generate xml string for tagged element >-----------------------------

std::string TaggedElement::toString()
{
  std::string spacer(tabSize*++count, ' ');
  std::string xml = "\n" + spacer + "<" + tag_;
  for (auto at : attribs_)
  {
    xml += " ";
    xml += at.first;
    xml += "=\"";
    xml += at.second;
    xml += "\"";
  }
  xml += ">";
  for (auto pChild : children_)
    xml += pChild->toString();
  xml += "\n" + spacer + "</" + tag_ + ">";
  --count;
  return xml;
}
/////////////////////////////////////////////////////////////////////////////
// TextElement methods

//----< generate xml string for text element >-------------------------------

std::string TextElement::toString()
{
  std::string spacer(tabSize * ++count, ' ');
  std::string xml = "\n" + spacer + text_;
  --count;
  return xml;
}
/////////////////////////////////////////////////////////////////////////////
// ProcInstrElement methods

//----< add attribute to ProcInstElement >-----------------------------------

bool ProcInstrElement::addAttrib(const std::string& name, const std::string& value)
{
  std::pair<std::string, std::string> atPair(name, value);
  attribs_.push_back(atPair);
  return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool ProcInstrElement::removeAttrib(const std::string& name)
{
  for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
  {
    if (iter->first == name)
    {
      attribs_.erase(iter);
      return true;
    }
  }
  return false;
}
//----< generate xml string for ProcInstr element >--------------------------

std::string ProcInstrElement::toString()
{
  std::string spacer(tabSize*++count, ' ');
  std::string xml = "\n" + spacer + "<" + "!";
  for (auto at : attribs_)
  {
    xml += " ";
    xml += at.first;
    xml += "=\"";
    xml += at.second;
    xml += "\"";
  }
  xml += "!>";
  --count;
  return xml;
}
/////////////////////////////////////////////////////////////////////////////
// XmlDeclarElement methods

//----< generate xml string for text element >-------------------------------

std::string XmlDeclarElement::toString()
{
  std::string spacer(tabSize * ++count, ' ');
  std::string xml = "\n" + spacer + "<?xml";
  for (auto at : attribs_)
  {
    xml += " ";
    xml += at.first;
    xml += "=\"";
    xml += at.second;
    xml += "\"";
  }
  xml += " ?>";
  --count;
  return xml;
}
//----< add attribute to ProcInstElement >-----------------------------------

bool XmlDeclarElement::addAttrib(const std::string& name, const std::string& value)
{
  std::pair<std::string, std::string> atPair(name, value);
  attribs_.push_back(atPair);
  return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool XmlDeclarElement::removeAttrib(const std::string& name)
{
  for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
  {
    if (iter->first == name)
    {
      attribs_.erase(iter);
      return true;
    }
  }
  return false;
}
/////////////////////////////////////////////////////////////////////////////
// CommentElement methods

//----< generate xml string for ProcInstr element >--------------------------

std::string CommentElement::toString()
{
  std::string spacer(tabSize*++count, ' ');
  std::string xml = "\n" + spacer + "<" + "!-- ";
  xml += commentText_;
  xml += " -->";
  --count;
  return xml;
}
/////////////////////////////////////////////////////////////////////////////
// Global Helper Methods

//----< helper function displays titles >------------------------------------

void XmlProcessing::title(const std::string& title, char underlineChar)
{
  std::cout << "\n  " << title;
  std::cout << "\n " << std::string(title.size() + 2, underlineChar);
}

//----< test stub >----------------------------------------------------------

#ifdef TEST_XMLELEMENT

int main()
{
  title("Testing XmlElement Package", '=');
  std::cout << "\n";

  using sPtr = std::shared_ptr < AbstractXmlElement > ;

  sPtr root = makeTaggedElement("root");
  root->addChild(makeTextElement("this is a test"));

  sPtr child = makeTaggedElement("child");
  child->addChild(makeTextElement("this is another test"));
  child->addAttrib("first", "test1");
  child->addAttrib("second", "test2");
  root->addChild(child);
  AbstractXmlElement::Attributes attribs = child->attributes();
  std::cout << "\n  Attributes:";
  for (auto item : attribs)
  {
    std::cout << "\n    " << item.first << "=\"" << item.second << "\"";
  }
  std::cout << "\n  attribute value for name = " << "first" << " is \"" << child->attributeValue("first") << "\"\n";
  sPtr docEl = makeDocElement(root);
  std::cout << "  " << docEl->toString();
  std::cout << "\n\n";
}

#endif
