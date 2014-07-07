//
//  CssStyleSheet.cpp
//  C++HTML
//
//  Created by Denis Stadniczuk on 03/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "CssStyleSheet.h"
#include "StringUtil.h"



CssStyle CssStyleSheet::parseAttributes(std::string attributes)
{
   //   remove_if(attributes.begin(), attributes.end(), isspace);

   CssStyle style;

   std::vector<std::string> attrs = StringSplit(attributes, ";");
   for (int i = 0; i < attrs.size(); i++) {
      std::vector<std::string> kv = StringSplit(attrs[i], ":");
      if (kv.size() < 2) {
         continue;
      }

      std::string key = kv[0];
      key.erase(remove_if(key.begin(), key.end(), isspace), key.end());
      std::string val = kv[1];

      if (key == "display") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         if (val == "inline") {
            style.display = DISPLAY_INLINE;
         } else if (val == "inline-block") {
            style.display = DISPLAY_INLINE_BLOCK;
         } else if (val == "block") {
            style.display = DISPLAY_BLOCK;
         }
      } else if (key == "position") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         if (val == "absolute") {
            style.position = POSITION_ABSOLUTE;
         } else if (val == "relative") {
            style.position = POSITION_RELATIVE;
         } else if (val == "fixed") {
            style.position = POSITION_FIXED;
         } else if (val == "static") {
            style.position = POSITION_STATIC;
         } else if (val == "inherit") {
            style.position = POSITION_INHERIT;
         }
      } else if (key == "font-size") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.fontSize = val;
      } else if (key == "color") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.color = val;
      } else if (key == "width") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.width = val;
      } else if (key == "height") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.height = val;
      } else if (key == "border") {
         if (val == "inherit") {

         } else if (val == "initial") {

         } else {
            std::vector<std::string> vals = StringSplit(val);
            if (vals.size() > 0) {
               style.borderTopWidth = vals[0];
               style.borderBottomWidth = vals[0];
               style.borderLeftWidth = vals[0];
               style.borderRightWidth = vals[0];
            }
            if (vals.size() > 1) {
               if (vals[1] == "none") {
                  style.borderTopStyle = BORDERSTYLE_NONE;
                  style.borderBottomStyle = BORDERSTYLE_NONE;
                  style.borderLeftStyle = BORDERSTYLE_NONE;
                  style.borderRightStyle = BORDERSTYLE_NONE;
               } else if (vals[1] == "hidden") {
                  style.borderTopStyle = BORDERSTYLE_HIDDEN;
                  style.borderBottomStyle = BORDERSTYLE_HIDDEN;
                  style.borderLeftStyle = BORDERSTYLE_HIDDEN;
                  style.borderRightStyle = BORDERSTYLE_HIDDEN;
               } else if (vals[1] == "dotted") {
                  style.borderTopStyle = BORDERSTYLE_DOTTED;
                  style.borderBottomStyle = BORDERSTYLE_DOTTED;
                  style.borderLeftStyle = BORDERSTYLE_DOTTED;
                  style.borderRightStyle = BORDERSTYLE_DOTTED;
               } else if (vals[1] == "dashed") {
                  style.borderTopStyle = BORDERSTYLE_DASHED;
                  style.borderBottomStyle = BORDERSTYLE_DASHED;
                  style.borderLeftStyle = BORDERSTYLE_DASHED;
                  style.borderRightStyle = BORDERSTYLE_DASHED;
               } else if (vals[1] == "solid") {
                  style.borderTopStyle = BORDERSTYLE_SOLID;
                  style.borderBottomStyle = BORDERSTYLE_SOLID;
                  style.borderLeftStyle = BORDERSTYLE_SOLID;
                  style.borderRightStyle = BORDERSTYLE_SOLID;
               } else if (vals[1] == "double") {
                  style.borderTopStyle = BORDERSTYLE_DOUBLE;
                  style.borderBottomStyle = BORDERSTYLE_DOUBLE;
                  style.borderLeftStyle = BORDERSTYLE_DOUBLE;
                  style.borderRightStyle = BORDERSTYLE_DOUBLE;
               } else if (vals[1] == "groove") {
                  style.borderTopStyle = BORDERSTYLE_GROOVE;
                  style.borderBottomStyle = BORDERSTYLE_GROOVE;
                  style.borderLeftStyle = BORDERSTYLE_GROOVE;
                  style.borderRightStyle = BORDERSTYLE_GROOVE;
               } else if (vals[1] == "ridge") {
                  style.borderTopStyle = BORDERSTYLE_RIDGE;
                  style.borderBottomStyle = BORDERSTYLE_RIDGE;
                  style.borderLeftStyle = BORDERSTYLE_RIDGE;
                  style.borderRightStyle = BORDERSTYLE_RIDGE;
               } else if (vals[1] == "inset") {
                  style.borderTopStyle = BORDERSTYLE_INSET;
                  style.borderBottomStyle = BORDERSTYLE_INSET;
                  style.borderLeftStyle = BORDERSTYLE_INSET;
                  style.borderRightStyle = BORDERSTYLE_INSET;
               } else if (vals[1] == "outset") {
                  style.borderTopStyle = BORDERSTYLE_OUTSET;
                  style.borderBottomStyle = BORDERSTYLE_OUTSET;
                  style.borderLeftStyle = BORDERSTYLE_OUTSET;
                  style.borderRightStyle = BORDERSTYLE_OUTSET;
               } else if (vals[1] == "initial") {
                  style.borderTopStyle = BORDERSTYLE_INITIAL;
                  style.borderBottomStyle = BORDERSTYLE_INITIAL;
                  style.borderLeftStyle = BORDERSTYLE_INITIAL;
                  style.borderRightStyle = BORDERSTYLE_INITIAL;
               } else if (vals[1] == "inherit") {
                  style.borderTopStyle = BORDERSTYLE_INHERIT;
                  style.borderBottomStyle = BORDERSTYLE_INHERIT;
                  style.borderLeftStyle = BORDERSTYLE_INHERIT;
                  style.borderRightStyle = BORDERSTYLE_INHERIT;
               }
            }

            if (vals.size() > 2) {
               style.borderTopColor = vals[2];
               style.borderBottomColor = vals[2];
               style.borderLeftColor = vals[2];
               style.borderRightColor = vals[2];
            }
         }
      } else if (key == "background-color") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.backgroundColor = val;
      } else if (key == "padding-top") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.paddingTop = val;
      } else if (key == "padding-bottom") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.paddingBottom = val;
      } else if (key == "padding-left") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.paddingLeft = val;
      } else if (key == "padding-right") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.paddingRight = val;
      } else if (key == "padding") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.paddingTop = val;
         style.paddingBottom = val;
         style.paddingLeft = val;
         style.paddingRight = val;
      } else if (key == "margin") {
         val.erase(remove_if(val.begin(), val.end(), isspace), val.end());
         style.marginTop = val;
         style.marginBottom = val;
         style.marginLeft = val;
         style.marginRight = val;
      }
   }
   return style;
}

void CssStyleSheet::parseAttributes(std::string attributes, CssStyle *style)
{
   CssStyle s = parseAttributes(attributes);
   applyStyle(s, style);
}

void CssStyleSheet::applyStyle(CssStyle styleToApply, CssStyle *styleToApplyTo)
{
   if (styleToApply.position != POSITION_UNDEFINED) {
      styleToApplyTo->position = styleToApply.position;
   }
   if (styleToApply.display != DISPLAY_UNDEFINED) {
      styleToApplyTo->display = styleToApply.display;
   }


   if (styleToApply.borderTopStyle != BORDERSTYLE_UNDEFINED) {
      styleToApplyTo->borderTopStyle = styleToApply.borderTopStyle;
   }
   if (styleToApply.borderBottomStyle != BORDERSTYLE_UNDEFINED) {
      styleToApplyTo->borderBottomStyle = styleToApply.borderBottomStyle;
   }
   if (styleToApply.borderLeftStyle != BORDERSTYLE_UNDEFINED) {
      styleToApplyTo->borderLeftStyle = styleToApply.borderLeftStyle;
   }
   if (styleToApply.borderRightStyle != BORDERSTYLE_UNDEFINED) {
      styleToApplyTo->borderRightStyle = styleToApply.borderRightStyle;
   }



   if (styleToApply.width != "") {
      styleToApplyTo->width = styleToApply.width;
   }
   if (styleToApply.height != "") {
      styleToApplyTo->height = styleToApply.height;
   }

   if (styleToApply.color != "") {
      styleToApplyTo->color = styleToApply.color;
   }
   if (styleToApply.fontSize != "") {
      styleToApplyTo->fontSize = styleToApply.fontSize;
   }


   if (styleToApply.borderTopColor != "") {
      styleToApplyTo->borderTopColor = styleToApply.borderTopColor;
   }
   if (styleToApply.borderBottomColor != "") {
      styleToApplyTo->borderBottomColor = styleToApply.borderBottomColor;
   }
   if (styleToApply.borderLeftColor != "") {
      styleToApplyTo->borderLeftColor = styleToApply.borderLeftColor;
   }
   if (styleToApply.borderRightColor != "") {
      styleToApplyTo->borderRightColor = styleToApply.borderRightColor;
   }

   if (styleToApply.borderTopWidth != "") {
      styleToApplyTo->borderTopWidth = styleToApply.borderTopWidth;
   }
   if (styleToApply.borderBottomWidth != "") {
      styleToApplyTo->borderBottomWidth = styleToApply.borderBottomWidth;
   }
   if (styleToApply.borderLeftWidth != "") {
      styleToApplyTo->borderLeftWidth = styleToApply.borderLeftWidth;
   }
   if (styleToApply.borderRightWidth != "") {
      styleToApplyTo->borderRightWidth = styleToApply.borderRightWidth;
   }


   if (styleToApply.backgroundColor != "") {
      styleToApplyTo->backgroundColor = styleToApply.backgroundColor;
   }

   if (styleToApply.paddingTop != "") {
      styleToApplyTo->paddingTop = styleToApply.paddingTop;
   }
   if (styleToApply.paddingBottom != "") {
      styleToApplyTo->paddingBottom = styleToApply.paddingBottom;
   }
   if (styleToApply.paddingLeft != "") {
      styleToApplyTo->paddingLeft = styleToApply.paddingLeft;
   }
   if (styleToApply.paddingRight != "") {
      styleToApplyTo->paddingRight = styleToApply.paddingRight;
   }

   if (styleToApply.marginTop != "") {
      styleToApplyTo->marginTop = styleToApply.marginTop;
   }
   if (styleToApply.marginBottom != "") {
      styleToApplyTo->marginBottom = styleToApply.marginBottom;
   }
   if (styleToApply.marginLeft != "") {
      styleToApplyTo->marginLeft = styleToApply.marginLeft;
   }
   if (styleToApply.marginRight != "") {
      styleToApplyTo->marginRight = styleToApply.marginRight;
   }
}



CssStyleSheet::CssStyleSheet(std::string sheet)
{
   for (int i = 0; i < sheet.length(); i++) {
      if (sheet[i] == '\n') {
         sheet.erase(i, 1);
         i--;
      }
   }

//  //   remove_if(sheet.begin(), sheet.end(), isspace);
   std::vector<std::string> rules = StringSplit(sheet, "}");

   for (std::vector<std::string>::iterator it = rules.begin(); it != rules.end(); it++) {

      std::string selector;
      std::string rules;

      for (int i = 0; i < (*it).length()-1; i++) {
         if ((*it)[i] == '{') {
            selector = (*it).substr(0, i);
            rules = (*it).substr(i+1);
         }
      }
      if (selector == "") {
         continue;
      }

      for (int i = 0; i < selector.length(); i++) {
         if (selector[i] != ' ') {
            break;
         }
         selector.erase(i, 1);
         i--;
      }
      for (int i = (int)selector.length()-1; i >= 0; i--) {
         if (selector[i] != ' ') {
            break;
         }
         selector.erase(i, 1);
         i--;
      }

      for (int i = 1; i < selector.length()-1; i++) {
         if ((selector[i+1] == '>' || selector[i+1] == '+') && selector[i] == ' ') {
            selector.erase(i, 1);
            i--;
         }
         if ((selector[i] == '>' || selector[i] == '+') && selector[i-1] == ' ') {
            selector.erase(i-1, 1);
            i--;
         }
      }


      if (selector != "" && rules != "") {
         cssRules[selector] = parseAttributes(rules);
      }
   }
}

void CssStyleSheet::addRule(std::string selector, std::string rule, int index)
{
   cssRules[selector] = parseAttributes(rule);
}