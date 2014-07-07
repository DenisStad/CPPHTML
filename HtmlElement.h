//
//  HtmlElement.h
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __Musee__HtmlElement__
#define __Musee__HtmlElement__

#include <iostream>

#include <vector>
#include <map>

#include "HtmlNode.h"
#include "HtmlEvent.h"

typedef void (*traverseFunc)(void* userPointer, HtmlNode *node);

class EventListener {

public:
   virtual bool call(Event event) =0;
};


class HtmlElement : public HtmlNode {

   std::string identifier;
   std::vector<std::string> classes;

   std::vector<HtmlNode*> children;

   std::map<std::string, std::string> attributes;

   bool simpleSelectorApplies(std::string simpleSelector);
   std::vector<HtmlElement*> traverseSelector(std::string simpleSelector);

   std::map<std::string, std::vector<EventListener*>> eventListeners;

   float scrollY, scrollX;
public:
   HtmlElement(std::string tag);
   ~HtmlElement();
   virtual std::string toString();

   void traverse(void* userPointer, traverseFunc func);


   void appendChild(HtmlNode *child);
   void removeChild(HtmlNode *child);
   std::vector<HtmlNode*> childNodes();

   HtmlNode* lastChild();

   void setAttribute(std::string attrName, std::string attrVal);
   bool hasAttribute(std::string attrName);
   std::string getAttribute(std::string attrName);

   std::vector<HtmlElement*> querySelectorAll(std::string selector);

   //events
   void addEventListener(std::string type, EventListener* listener);
   void removeEventListener(std::string type, EventListener* listener);
   void dispatchEvent(Event &event);

   float scrollLeft();
   float scrollTop();
};

#endif /* defined(__Musee__HtmlElement__) */
