//
//  HtmlDocument.cpp
//  C++HTML
//
//  Created by Denis Stadniczuk on 03/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "HtmlDocument.h"
#include "StringUtil.h"
#include "HtmlElement.h"
#include "HtmlText.h"

const std::string blockElements[] = {
   "ADDRESS",
   "ARTICLE",
   "ASIDE",
   "AUDIO",
   "BLOCKQUOTE",
   "CANVAS",
   "DD",
   "DIV",
   "DL",
   "FIELDSET",
   "FIGCAPTION",
   "FIGURE",
   "FOOTER",
   "FORM",
   "H1",
   "H2",
   "H3",
   "H4",
   "H5",
   "H6",
   "HEADER",
   "HGROUP",
   "HR",
   "NOSCRIPT",
   "OL",
   "OUTPUT",
   "P",
   "PRE",
   "SECTION",
   "TABLE",
   "UL",
   "VIDEO"
};
const int numBlockElements = 32;

HtmlDocument::HtmlDocument(std::string html)
{
   renderer = NULL;


   head = new HtmlElement("head");
   body = new HtmlElement("body");

   head->setDocument(this);
   body->setDocument(this);

   xmlDocPtr doc = xmlReadMemory(html.c_str(), html.length(), "noname.xml", NULL, 0);
   xmlNode *root_element = xmlDocGetRootElement(doc);

   parse_traverse(root_element, body);

   //first apply all rules that we collected in external stylesheets
   //TODO

   //second apply all rules that we collected in style elements
   //sort the rules first
   //TODO

   for (int i = 0; i < styleSheets.size(); i++) {
      for (std::map<std::string, CssStyle>::iterator it = styleSheets[i]->cssRules.begin(); it != styleSheets[i]->cssRules.end(); it++) {
         std::vector<HtmlElement*> nodes = querySelectorAll(it->first);
         for (int j = 0; j < nodes.size(); j++) {
            CssStyleSheet::applyStyle(it->second, &nodes[j]->style);
         }
      }
   }

   //and now apply all inline rules + calculate calculatedStyle
   applyCss(body);

   xmlFreeDoc(doc);
   xmlCleanupParser();
}

HtmlDocument::~HtmlDocument()
{
   delete head;
   delete body;

   for (int i = 0; i < styleSheets.size(); i++) {
      CssStyleSheet *sheet = styleSheets[i];
      styleSheets.erase(styleSheets.begin() +i);
      delete sheet;
      i--;
   }
}

void HtmlDocument::parse_traverse(xmlNode *node, HtmlElement *root)
{
   xmlNode *cur_node = NULL;

   for (cur_node = node; cur_node; cur_node = cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {

         std::string n = (char*) cur_node->name;

         HtmlElement *e = new HtmlElement(n);
         e->setDocument(this);
         //         printf("node type: Element, name: %s\n", cur_node->name);

         xmlAttr *attr = cur_node->properties;
         while (attr) {
            //            printf("%s= %s\n", attr->name, xmlGetProp(cur_node, attr->name));
            std::string n = (char*) attr->name;
            std::string v = (char*) xmlGetProp(cur_node, attr->name);
            e->setAttribute(n, v);

            attr = attr->next;
         }
         //         printf("%s\n", cur_node->content);

         if (toUpper(n) == "HEAD") {
            parse_traverse(cur_node->children, head);
         } else if (toUpper(n) == "BODY") {
            parse_traverse(cur_node->children, body);
         } else if (toUpper(n) == "HTML") {
            parse_traverse(cur_node->children, body);
         } else {
            root->appendChild(e);
            parse_traverse(cur_node->children, e);
         }

      } else if (cur_node->type == XML_TEXT_NODE) {
         HtmlText *text = new HtmlText();
         std::string s = (char*) cur_node->content;

         for (int i = 0; i < s.length(); i++) {
            if (s[i] == '\n' || s[i] == '\r') {
               s.replace(i, 1, " ");
            }
            if (i > 0 && s[i] == ' ' && s[i-1] == ' ') {
               s.replace(i-1, 2, " ");
               i--;
            }
         }

         text->text(s);
         root->appendChild(text);

         if (root->getTagName() == "STYLE") {

            styleSheets.push_back(new CssStyleSheet(s));
         }
      }
   }
}

void HtmlDocument::applyCss(HtmlElement *root)
{
   root->calculatedStyle.position = POSITION_STATIC;
   root->calculatedStyle.display = DISPLAY_INLINE;
   for (int i = 0; i < numBlockElements; i++) {
      if (root->getTagName() == blockElements[i]) {
         root->calculatedStyle.display = DISPLAY_BLOCK;
         break;
      }
   }
   root->calculatedStyle.borderTopStyle = BORDERSTYLE_NONE;
   root->calculatedStyle.borderBottomStyle = BORDERSTYLE_NONE;
   root->calculatedStyle.borderLeftStyle = BORDERSTYLE_NONE;
   root->calculatedStyle.borderRightStyle = BORDERSTYLE_NONE;



   if (root->hasAttribute("style")) {
      CssStyleSheet::parseAttributes(root->getAttribute("style"), &root->style);
   }
   CssStyleSheet::applyStyle(root->style, &root->calculatedStyle);

   if (root->calculatedStyle.color == "") {
      HtmlElement *parent = root->parentNode();
      while (parent != NULL) {
         if (parent->calculatedStyle.color != "") {
            root->calculatedStyle.color = parent->calculatedStyle.color;
            break;
         }
         parent = parent->parentNode();
      }
   }
   if (root->calculatedStyle.color == "") {
      root->calculatedStyle.color = "black";
   }


   std::vector<HtmlNode*> childNodes = root->childNodes();
   for (std::vector<HtmlNode*>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {
      if ((*it)->getNodeType() == ELEMENT) {
         applyCss((HtmlElement*)*it);
      } else {
         (*it)->calculatedStyle.color = root->calculatedStyle.color;
      }
   }
}


std::vector<HtmlElement*> HtmlDocument::querySelectorAll(std::string selector)
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

   if (firstPart == "head") {
      if (nextPart == "") {
         std::vector<HtmlElement*> ret;
         ret.push_back(head);
         return ret;
      }
      return head->querySelectorAll(nextPart);
   } else if (firstPart == "body") {
      if (nextPart == "") {
         std::vector<HtmlElement*> ret;
         ret.push_back(body);
         return ret;
      }
      return body->querySelectorAll(nextPart);
   } else {
      std::vector<HtmlElement*> r1 = head->querySelectorAll(selector);
      std::vector<HtmlElement*> r2 = body->querySelectorAll(selector);

      r1.insert( r1.end(), r2.begin(), r2.end() );

      return r1; //make it unique (std::set)

   }
}

void HtmlDocument::reCalcStyles()
{
   for (int i = 0; i < styleSheets.size(); i++) {
      for (std::map<std::string, CssStyle>::iterator it = styleSheets[i]->cssRules.begin(); it != styleSheets[i]->cssRules.end(); it++) {
         
         std::vector<HtmlElement*> nodes = querySelectorAll(it->first);
         for (int j = 0; j < nodes.size(); j++) {
            CssStyleSheet::applyStyle(it->second, &nodes[j]->style);
         }
      }
   }

   //and now apply all inline rules + calculate calculatedStyle
   applyCss(body);
}

void HtmlDocument::needsReRendering()
{
   if (renderer != NULL) {
      renderer->setNeedsRerendering();
   }
}
