//
//  HtmlNode.h
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __Musee__HtmlNode__
#define __Musee__HtmlNode__

#include <iostream>

#include "CssStyle.h"

class HtmlElement;
class HtmlDocument;

typedef enum {
   TEXT,
   ELEMENT
} NodeType;

class HtmlNode {
   NodeType type;
   
protected:
   float calculatedX;
   float calculatedY;
   float calculatedZ;
   float offsetW;
   float offsetH;
   float scrollW;
   float scrollH;


   HtmlDocument *doc;
   HtmlElement *parent;
   std::string tagName;

public:
   HtmlNode(NodeType type);
   NodeType getNodeType();

   void setDocument(HtmlDocument *document);
   HtmlDocument* document();
   void setParent(HtmlElement *parent);
   HtmlElement* parentNode();
   std::string getTagName();

   CssStyle calculatedStyle;
   CssStyle style;

   virtual std::string toString() {return "";};

   void setCalculatedBounds(float x, float y, float z, float offsetWidth, float offsetHeight, float scrollWidth, float scrollHeight);
   void getCalculatedBounds(float &x, float &y, float &z, float &offsetWidth, float &offsetHeight, float &scrollWidth, float &scrollHeight);
};

#endif /* defined(__Musee__HtmlNode__) */
