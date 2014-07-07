//
//  HtmlNode.cpp
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "HtmlNode.h"
#include "HtmlElement.h"
#include "StringUtil.h"

HtmlNode::HtmlNode(NodeType t)
:type(t)
{
   doc = NULL;
   parent = NULL;

   calculatedX = 0;
   calculatedY = 0;
   calculatedZ = 0;
   offsetW = 0;
   offsetH = 0;
   scrollW = 0;
   scrollH = 0;
}

NodeType HtmlNode::getNodeType()
{
   return type;
}

std::string HtmlNode::getTagName()
{
   return toUpper(tagName);
}

void HtmlNode::setDocument(HtmlDocument *document)
{
   doc = document;
   if (getNodeType() == ELEMENT) {
      HtmlElement *el = (HtmlElement*) this;
      for (int i = 0; i < el->childNodes().size(); i++) {
         el->childNodes()[i]->setDocument(document);
      }
   }
}

HtmlDocument* HtmlNode::document()
{
   return doc;
}

void HtmlNode::setParent(HtmlElement *_parent)
{
   parent = _parent;
}

HtmlElement* HtmlNode::parentNode()
{
   return parent;
}

void HtmlNode::setCalculatedBounds(float x, float y, float z, float offsetWidth, float offsetHeight, float scrollWidth, float scrollHeight)
{
   calculatedX = x;
   calculatedY = y;
   calculatedZ = z;
   offsetW = offsetWidth;
   offsetH = offsetHeight;
   scrollW = scrollWidth;
   scrollH = scrollHeight;
}

void HtmlNode::getCalculatedBounds(float &x, float &y, float &z, float &offsetWidth, float &offsetHeight, float &scrollWidth, float &scrollHeight)
{
   x = calculatedX;
   y = calculatedY;
   z = calculatedZ;
   offsetWidth = offsetW;
   offsetHeight = offsetH;
   scrollWidth = scrollW;
   scrollHeight = scrollH;
}
