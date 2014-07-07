//
//  HtmlText.h
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __Musee__HtmlText__
#define __Musee__HtmlText__

#include <iostream>
#include <vector>

#include "HtmlNode.h"
#include "FontTools.h"

class HtmlText : public HtmlNode {

   std::string _text;

public:
   HtmlText();

   std::string text();
   void text(std::string);

   virtual std::string toString();


   
   std::vector<GlyphPosition> characterPositions;
};

#endif /* defined(__Musee__HtmlText__) */
