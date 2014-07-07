//
//  HtmlElement.cpp
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "HtmlElement.h"
#include "HtmlDocument.h"
#include "StringUtil.h"

HtmlElement::HtmlElement(std::string tag)
:HtmlNode(ELEMENT)
{
   tagName = tag;
   scrollY = 0;
   scrollX = 0;
}

HtmlElement::~HtmlElement()
{
   while (children.size() > 0) {
      HtmlNode *child = children[0];
      children.erase(children.begin());
      delete child;
   }
}

void HtmlElement::appendChild(HtmlNode *child)
{
   child->setParent(this);
   child->setDocument(document());
   children.push_back(child);

   Event nodeInsertedEvent;
   nodeInsertedEvent.initEvent("DOMNodeInserted", true, false);
   nodeInsertedEvent.target = this;
   nodeInsertedEvent.currentTarget = this;
   this->dispatchEvent(nodeInsertedEvent);

//   nodeInsertedEvent.initEvent("DOMNodeInsertedIntoDocument", false, false);
//   document()->dispatchEvent(nodeInsertedEvent);

   if (document() != NULL) {
      document()->needsReRendering();
   }
}

void HtmlElement::removeChild(HtmlNode *child)
{
   for (int i = (int)children.size()-1; i >= 0; i--) {
      if (children[i] == child) {
         children.erase(children.begin() + i);
         break;
      }
   }
   if (document() != NULL) {
      document()->needsReRendering();
   }
}

std::vector<HtmlNode*> HtmlElement::childNodes()
{
   return children;
}

HtmlNode* HtmlElement::lastChild()
{
   if (children.size() > 0) {
      return children[children.size()-1];
   }
   return NULL;
}

void HtmlElement::setAttribute(std::string attrName, std::string attrVal)
{
   attributes[attrName] = attrVal;
}

bool HtmlElement::hasAttribute(std::string attrName)
{
   return attributes.find(attrName) != attributes.end();
}

std::string HtmlElement::getAttribute(std::string attrName)
{
   return attributes[attrName];
}

std::string HtmlElement::toString()
{
   std::string out = "<" + tagName;

   out += ">";

   for (int i = 0; i < childNodes().size(); i++) {
      out += childNodes()[i]->toString();
   }

   out += "</" + tagName + ">";
   return out;
}

void HtmlElement::traverse(void* userPointer, traverseFunc func)
{
   (*func)(userPointer, this);
   
   for (int i = 0; i < childNodes().size(); i++) {
      if (childNodes()[i]->getNodeType() == ELEMENT) {
         ((HtmlElement*)childNodes()[i])->traverse(userPointer, func);
      } else if (childNodes()[i]->getNodeType() == TEXT) {
         (*func)(userPointer, childNodes()[i]);
      }
   }
}



bool HtmlElement::simpleSelectorApplies(std::string simpleSelector)
{
   if (simpleSelector[0] == '.') {
      std::string cssClass = simpleSelector.substr(1);
      if (hasAttribute("class")) {
         std::string elemClasses = getAttribute("class");
         std::vector<std::string> classes = StringSplit(elemClasses);
         for (std::vector<std::string>::iterator it = classes.begin(); it != classes.end(); it++) {
            if ((*it) == cssClass) {
               return true;
            }
         }
      }
   } else if (simpleSelector[0] == '#') {
      std::string cssId = simpleSelector.substr(1);
      if (hasAttribute("id")) {
         std::string elemId = getAttribute("id");
         if (elemId == cssId) {
            return true;
         }
      }
   } else {
      std::string tagName = toUpper(simpleSelector);
      struct convert {
         void operator()(char& c) { c = toupper((unsigned char)c); }
      };

      for_each(tagName.begin(), tagName.end(), convert());

      if (getTagName() == tagName) {
         return true;
      }
   }
   
   return false;
}

std::vector<HtmlElement*> HtmlElement::traverseSelector(std::string simpleSelector)
{
   std::vector<HtmlElement*> ret;
   for (int i = 0; i < childNodes().size(); i++) {
      if (childNodes()[i]->getNodeType() == ELEMENT) {
         HtmlElement *elem = (HtmlElement*)childNodes()[i];
         if (elem->simpleSelectorApplies(simpleSelector)) {
            ret.push_back(elem);
         }
         std::vector<HtmlElement*> cs = elem->traverseSelector(simpleSelector);
         for (int j = 0; j < cs.size(); j++) {
            ret.push_back(cs[j]);
         }
      }
   }
   return ret;
}

std::vector<HtmlElement*> HtmlElement::querySelectorAll(std::string selector)
{

   //sanitize it first

   if (selector == "") {
      return std::vector<HtmlElement*>();
   }

   std::string firstPart;
   std::string nextPart;

   for (int i = 1; i < selector.length(); i++) {
      if (selector[i] == ' ' || selector[i] == '>' || selector[i] == '+') {
         firstPart = selector.substr(0, i);
         nextPart = selector.substr(i);
         break;
      }
   }

   if (firstPart == "" && nextPart == "") {
      firstPart = selector;
   }

   std::vector<HtmlElement*> ret = traverseSelector(firstPart);

   if (nextPart != "" && nextPart[0] == ' ') {

      std::vector<HtmlElement*> next;
      for (int i = 0; i < ret.size(); i++) {
         std::vector<HtmlElement*> np = ret[i]->querySelectorAll(nextPart.substr(1));
         for (int j = 0; j < np.size(); j++) {
            next.push_back(np[j]);
         }
      }
      ret = next;
   }

   return ret;
}


//events

void HtmlElement::addEventListener(std::string event, EventListener* listener)
{
   if (eventListeners.find(event) == eventListeners.end()) {
      eventListeners[event] = std::vector<EventListener*>();
   }
   eventListeners[event].push_back(listener);
}

void HtmlElement::dispatchEvent(Event &event)
{

   HtmlElement *parent = this;
   while (parent != NULL) {
      event.target = parent;
      if (event.type == "scroll") {
         ScrollEventData *data = (ScrollEventData*)event.data;
         float formerX = parent->scrollX;
         float targetX = parent->scrollX - data->deltaX;
         if (targetX < 0) {
            targetX = 0;
         }
         if (targetX > parent->scrollW - parent->offsetW) {
            targetX = parent->scrollW - parent->offsetW;
         }
         float leftX = formerX - targetX;

         float formerY = parent->scrollY;
         float targetY = parent->scrollY - data->deltaY;
         if (targetY < 0) {
            targetY = 0;
         }
         if (targetY > parent->scrollH - parent->offsetH) {
            targetY = parent->scrollH - parent->offsetH;
         }
         float leftY = formerY - targetY;

         parent->scrollX = targetX;
         parent->scrollY = targetY;

         data->deltaX = leftX;
         data->deltaY = leftY;

         std::cout << parent->getTagName() << " " << parent->scrollX << " " << parent->scrollY<<"/"<< parent->scrollH<<":"<<parent->offsetH << "  " << leftX << " " << leftY << std::endl;

         if (document()) {
            document()->needsReRendering();
         }
      }

      if (parent->eventListeners.find(event.type) != parent->eventListeners.end()) {
         for (int i = 0; i < parent->eventListeners[event.type].size(); i++) {
            parent->eventListeners[event.type][i]->call(event);
         }
      }

      if (!event.bubbles) {
         break;
      }
      parent = parent->parentNode();
   }

   if (!event.canceled) {
      //default action
   }
}

float HtmlElement::scrollLeft()
{
   return scrollX;
}

float HtmlElement::scrollTop()
{
   return scrollY;
}
