//
//  CssStyleSheet.h
//  C++HTML
//
//  Created by Denis Stadniczuk on 03/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __C__HTML__CssStyleSheet__
#define __C__HTML__CssStyleSheet__

#include <iostream>
#include <map>
#include "CssStyle.h"

class CssStyleSheet {


public:
   static void parseAttributes(std::string attributes, CssStyle *style);
   static CssStyle parseAttributes(std::string attributes);
   static void applyStyle(CssStyle styleToApply, CssStyle *styleToApplyTo);


   CssStyleSheet(std::string sheet);

   std::map<std::string, CssStyle> cssRules;
   void addRule(std::string selector, std::string rule, int index=-1);
};

#endif /* defined(__C__HTML__CssStyleSheet__) */
