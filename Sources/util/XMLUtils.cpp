/**
* Facebook Internet Explorer Toolbar Software License 
* Copyright (c) 2009 Facebook, Inc. 
*
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (which, together with any graphical images included with such
* software, are collectively referred to below as the "Software") to (a) use,
* reproduce, display, distribute, execute, and transmit the Software, (b)
* prepare derivative works of the Software (excluding any graphical images
* included with the Software, which may not be modified or altered), and (c)
* permit third-parties to whom the Software is furnished to do so, all
* subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.  
*
* Facebook, Inc. retains ownership of the Software and all associated
* intellectual property rights.  All rights not expressly granted in this
* license are reserved by Facebook, Inc.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/ 

#include "StdAfx.h"
#include "XMLUtils.h"


namespace facebook{ namespace XMLUtils{

String xmlToString(MSXML2::IXMLDOMDocumentPtr xmlDoc) {
  BSTR rawBSTR;
  const HRESULT getXMLRes = xmlDoc->get_xml(&rawBSTR);

  if FAILED(getXMLRes) {
    _com_raise_error(getXMLRes);  
  }

  _bstr_t xmlAsBSTR(rawBSTR, false);

  return String(xmlAsBSTR);
}


String getNodeText(MSXML2::IXMLDOMNodePtr& node) {
  BSTR outputValue;
  HRESULT getTextRes = node->get_text(&outputValue); 
  if (FAILED(getTextRes)) {
    _com_raise_error(getTextRes);
  }
  _bstr_t valudHolder(outputValue);
  String res(valudHolder);
  return res;
}


String getChildText(MSXML2::IXMLDOMNodePtr& parent, const String& childNodeName) {
  MSXML2::IXMLDOMNodePtr child = parent->selectSingleNode(childNodeName.c_str());
  if (child == NULL) {
    throw XMLUtils::ParseError();
  }

  return getNodeText(child);
}


MSXML2::IXMLDOMNodePtr createNode(MSXML2::IXMLDOMDocumentPtr& xmlDoc, 
      const String& nodeName) {
  MSXML2::IXMLDOMNodePtr res = xmlDoc->createNode(1, nodeName.c_str(), _T("")); 
  if (res == NULL) {
    throw XMLUtils::ParseError();
  }

  return res;
}

void setNodeText(MSXML2::IXMLDOMNodePtr& node, const String& text) {
  _bstr_t nodeText(text.c_str());
  const HRESULT putTextRes = node->put_text(nodeText);
  if (FAILED(putTextRes)) {
    _com_raise_error(putTextRes);
  }
}


MSXML2::IXMLDOMNodePtr createNodeWithText(MSXML2::IXMLDOMDocumentPtr& xmlDoc, 
      const String& nodeName, const String& text) {
  MSXML2::IXMLDOMNodePtr res = createNode(xmlDoc, nodeName);
  if (res == NULL) {
    throw XMLUtils::ParseError();
  }

  setNodeText(res, text);

  return res;
}

size_t getChildrenCount(MSXML2::IXMLDOMNodePtr& node) {
  MSXML2::IXMLDOMNodeListPtr children;
  const HRESULT getChildNodesRes = node->get_childNodes(&children);
  if (FAILED(getChildNodesRes)) { 
    _com_raise_error(getChildNodesRes);
  }

  LONG chilrenCount = 0;
  const HRESULT getChildenCountRes = children->get_length(&chilrenCount);
  if (FAILED(getChildenCountRes)) {
    _com_raise_error(getChildenCountRes);
  }

  return chilrenCount;
}


void  addTextAttributeToNode(MSXML2::IXMLDOMDocumentPtr& xmlDoc,
      MSXML2::IXMLDOMNodePtr& targetNode, const String& attributeName, 
      const String& attributeValue) {
  MSXML2::IXMLDOMNodePtr attributeNode = 
  xmlDoc->createNode(NODE_ATTRIBUTE, attributeName.c_str(), _T("")); 

  setNodeText(attributeNode, attributeValue);
  MSXML2::IXMLDOMNamedNodeMapPtr attributes = targetNode->Getattributes();
  if (attributes == NULL) {
    throw XMLUtils::ParseError();
  }

  attributes->setNamedItem(attributeNode);
}

bool hodeHasAttribute(MSXML2::IXMLDOMNodePtr& node, 
   const String& attributeName, const String attributeNameSpace) {
  MSXML2::IXMLDOMNamedNodeMapPtr attributes = node->Getattributes();
  MSXML2::IXMLDOMNodePtr res = attributes->getQualifiedItem(attributeName.c_str(), 
    attributeNameSpace.c_str());

  return res != NULL;
}

}}// !namespace facebook::XMLUtils
