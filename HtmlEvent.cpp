//
//  HtmlEvent.cpp
//  C++HTML
//
//  Created by Denis Stadniczuk on 07/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "HtmlEvent.h"

Event::Event()
{
   data = NULL;
}

Event::~Event()
{
   if (data != NULL) {
      delete data;
   }
}

void Event::initEvent(std::string type, bool canBubble, bool cancelable)
{
   this->type = type;
   this->bubbles = canBubble;
   this->cancelable = cancelable;
   canceled = false;
}

void Event::stopPropagation()
{
   bubbles = false;
}

void Event::preventDefault()
{
   if (this->cancelable) {
      canceled = true;
   }
}