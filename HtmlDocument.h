//
//  HtmlDocument.h
//  C++HTML
//
//  Created by Denis Stadniczuk on 03/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __C__HTML__HtmlDocument__
#define __C__HTML__HtmlDocument__

#include <iostream>
#include <vector>

#include <libxml/parser.h>

#include "Renderer.h"
#include "CssStyleSheet.h"
#include "HtmlElement.h"

class HtmlDocument {
   
   void parse_traverse(xmlNode *node, HtmlElement *root);
   void applyCss(HtmlElement *root);

public:
   HtmlDocument(std::string html);
   ~HtmlDocument();

   std::vector<CssStyleSheet*> styleSheets;

   HtmlElement *head;
   HtmlElement *body;

   std::vector<HtmlElement*> querySelectorAll(std::string selector);

   void reCalcStyles();
   Renderer *renderer;
   void needsReRendering();

//   void DOMNodeInsertedIntoDocument(HtmlNode *node);
};

#endif /* defined(__C__HTML__HtmlDocument__) */
