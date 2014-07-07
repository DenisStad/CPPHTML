//
//  HtmlText.cpp
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "HtmlText.h"
#include "HtmlDocument.h"

HtmlText::HtmlText()
:HtmlNode(TEXT)
{
   tagName = "#TEXT";
}

std::string HtmlText::toString()
{
   return _text;
}

std::string HtmlText::text()
{
   return _text;
}

void HtmlText::text(std::string t)
{
   _text = t;

   if (document() != NULL) {
      document()->needsReRendering();
   }
}
