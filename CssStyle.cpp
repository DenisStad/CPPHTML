//
//  CssStyle.cpp
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "CssStyle.h"

CssStyle::CssStyle()
{
   display = DISPLAY_UNDEFINED;
   position = POSITION_UNDEFINED;

   borderTopStyle = BORDERSTYLE_UNDEFINED;
   borderBottomStyle = BORDERSTYLE_UNDEFINED;
   borderLeftStyle = BORDERSTYLE_UNDEFINED;
   borderRightStyle = BORDERSTYLE_UNDEFINED;
}

void CssStyle::calcLength(std::string s, float &l, float parentL)
{
//   printf(" _ %s\n", s.c_str());
   if (s.length() > 2 && s.substr(s.length()-2) == "px") {
      l = std::stof(s.substr(0, s.length()-2));
   } else if (s.length() > 2 && s.substr(s.length()-2) == "em") {
      l = 12 * std::stof(s.substr(0, s.length()-2));
   } else if (s.length() > 1 && s.substr(s.length()-1) == "%") {
      l = std::stof(s.substr(0, s.length()-1)) / 100.0f * parentL;
   }
}

void CssStyle::calcColor(std::string s, float &r, float &g, float &b, float &a)
{
   r = 0;
   g = 0;
   b = 0;
   a = 0;

   if (s.length() > 0 &&  s[0] == '#') {
      //TODO
      std::string color = s.substr(1);

      if (color.length() == 3) {
         r = (float)std::stoi(color.substr(0,1), NULL, 16) / 15.0f;
         g = (float)std::stoi(color.substr(1,1), NULL, 16) / 15.0f;
         b = (float)std::stoi(color.substr(2,1), NULL, 16) / 15.0f;
         a = 1;
      } else if (color.length() == 6) {
         r = (float)std::stoi(color.substr(0,2), NULL, 16) / 15.0f;
         g = (float)std::stoi(color.substr(2,2), NULL, 16) / 15.0f;
         b = (float)std::stoi(color.substr(4,2), NULL, 16) / 15.0f;
         a = 1;
      }
   } else if (s == "black") {
      r = 0; g = 0; b = 0; a = 1;
   } else if (s == "gray") {
      r = 0.5; g = 0.5; b = 0.5; a = 1;
   } else if (s == "white") {
      r = 1; g = 1; b = 1; a = 1;
   } else if (s == "red") {
      r = 1; b = 0; g = 0; a = 1;
   } else if (s == "green") {
      r = 0; g = 1; b = 0; a = 1;
   } else if (s == "blue") {
      r = 0; g = 0; b = 1; a = 1;
   }
}




void CssStyle::calcFontSize(float &s)
{
   s = 16;
   calcLength(fontSize, s, 0);
}

void CssStyle::calcTextColor(float &r, float &g, float &b, float &a)
{
   calcColor(color, r, g, b, a);
}


void CssStyle::calcWidthHeight(float &w, float &h, float parentW, float parentH)
{
   calcLength(width, w, parentW);
   calcLength(height, h, parentH);
}

void CssStyle::calcBorderWidth(float &t, float &r, float &b, float &l)
{
   t = 0;
   r = 0;
   b = 0;
   l = 0;

   calcLength(borderTopWidth, t, 0);
   calcLength(borderRightWidth, r, 0);
   calcLength(borderBottomWidth, b, 0);
   calcLength(borderLeftWidth, l, 0);
}

void CssStyle::calcBorderTopColor(float &r, float &g, float &b, float &a)
{
   calcColor(borderTopColor, r, g, b, a);
}

void CssStyle::calcBorderBottomColor(float &r, float &g, float &b, float &a)
{
   calcColor(borderBottomColor, r, g, b, a);
}

void CssStyle::calcBorderLeftColor(float &r, float &g, float &b, float &a)
{
   calcColor(borderLeftColor, r, g, b, a);
}

void CssStyle::calcBorderRightColor(float &r, float &g, float &b, float &a)
{
   calcColor(borderRightColor, r, g, b, a);
}

void CssStyle::calcBGColor(float &r, float &g, float &b, float &a)
{
   calcColor(backgroundColor, r, g, b, a);
}

void CssStyle::calcPadding(float &t, float &r, float &b, float &l, float parentW, float parentH)
{
   t = 0;
   r = 0;
   b = 0;
   l = 0;

   calcLength(paddingTop, t, parentH);
   calcLength(paddingRight, r, parentW);
   calcLength(paddingBottom, b, parentH);
   calcLength(paddingLeft, l, parentW);
}

void CssStyle::calcMargin(float &t, float &r, float &b, float &l, float parentW, float parentH)
{
   t = 0;
   r = 0;
   b = 0;
   l = 0;

   calcLength(marginTop, t, parentH);
   calcLength(marginRight, r, parentW);
   calcLength(marginBottom, b, parentH);
   calcLength(marginLeft, l, parentW);
}
