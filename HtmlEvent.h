//
//  HtmlEvent.h
//  C++HTML
//
//  Created by Denis Stadniczuk on 07/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __C__HTML__HtmlEvent__
#define __C__HTML__HtmlEvent__

#include <iostream>

class HtmlNode;

class EventData {
public:
};

class ScrollEventData : public EventData {
public:
   float x, y;
   float deltaX;
   float deltaY;
};

class Event {

public:
   Event();
   ~Event();
   void initEvent(std::string type, bool canBubble, bool cancelable);

   std::string type;

   HtmlNode *target;
   HtmlNode *currentTarget;

   unsigned short eventPhase;
   bool bubbles;
   bool cancelable;
   unsigned int timeStamp;

   void stopPropagation();
   void preventDefault();
   bool canceled;

   EventData *data;
};

#endif /* defined(__C__HTML__HtmlEvent__) */
