//
//  CssStyle.h
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __Musee__CssStyle__
#define __Musee__CssStyle__

#include <iostream>

typedef enum {
   POSITION_UNDEFINED = 0,
   POSITION_ABSOLUTE,
   POSITION_RELATIVE,
   POSITION_FIXED,
   POSITION_STATIC,
   POSITION_INHERIT
} Position;

typedef enum {
   DISPLAY_UNDEFINED = 0,
   DISPLAY_INLINE,
   DISPLAY_INLINE_BLOCK,
   DISPLAY_BLOCK
} Display;

typedef enum {
   BORDERSTYLE_UNDEFINED = 0,
   BORDERSTYLE_NONE,
   BORDERSTYLE_HIDDEN,
   BORDERSTYLE_DOTTED,
   BORDERSTYLE_DASHED,
   BORDERSTYLE_SOLID,
   BORDERSTYLE_DOUBLE,
   BORDERSTYLE_GROOVE,
   BORDERSTYLE_RIDGE,
   BORDERSTYLE_INSET,
   BORDERSTYLE_OUTSET,
   BORDERSTYLE_INITIAL,
   BORDERSTYLE_INHERIT
} BorderStyle;

class CssStyle {
   static void calcLength(std::string s, float &l, float parentL);
   static void calcColor(std::string s, float &r, float &g, float &b, float &a);
public:
   CssStyle();

   Position position;
   Display display;

   std::string color;
   void calcTextColor(float &r, float &g, float &b, float &a);
   std::string fontSize;
   void calcFontSize(float &s);
   std::string font;

   std::string width;
   std::string height;
   void calcWidthHeight(float &w, float &h, float parentW, float parentH);

   std::string borderTopWidth;
   std::string borderBottomWidth;
   std::string borderLeftWidth;
   std::string borderRightWidth;
   void calcBorderWidth(float &t, float &r, float &b, float &l);

   BorderStyle borderTopStyle;
   BorderStyle borderBottomStyle;
   BorderStyle borderLeftStyle;
   BorderStyle borderRightStyle;

   std::string borderTopColor;
   std::string borderBottomColor;
   std::string borderLeftColor;
   std::string borderRightColor;
   void calcBorderTopColor(float &r, float &g, float &b, float &a);
   void calcBorderBottomColor(float &r, float &g, float &b, float &a);
   void calcBorderLeftColor(float &r, float &g, float &b, float &a);
   void calcBorderRightColor(float &r, float &g, float &b, float &a);

   std::string backgroundColor;
   void calcBGColor(float &r, float &g, float &b, float &a);

   std::string paddingTop;
   std::string paddingBottom;
   std::string paddingLeft;
   std::string paddingRight;
   void calcPadding(float &t, float &r, float &b, float &l, float parentW, float parentH);

   std::string marginTop;
   std::string marginBottom;
   std::string marginLeft;
   std::string marginRight;
   void calcMargin(float &t, float &r, float &b, float &l, float parentW, float parentH);


};

#endif /* defined(__Musee__CssStyle__) */
