//
//  FontTools.h
//  Musee
//
//  Created by Denis Stadniczuk on 27/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __Musee__FontTools__
#define __Musee__FontTools__

#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

struct GlyphPosition {
   float x, y;
   float w, h;
   float u0, v0, u1, v1;
};

struct GlyphInfo {
   float u0, v0, u1, v1;
   float w, h;
   float offsetX, offsetY;
};

class Font {
   FT_Face face;

   unsigned char *textureMapData;
   int textureMapLength;
   int lastAddedCPX;
   int lastAddedCPY;
   int maxH;
   int currentQuadrant;
   bool dirtyTexture;

   std::map<int, GlyphInfo> codepointPosition; //key of map is codepoint, pair is x,y in texture

public:
   Font(FT_Library lib, std::string fileName, int fontSize);


   //you should first call stringInfo with the string you want to draw. Then check if the map is updated and upload it to GL if necessary
   GlyphPosition* stringInfo(std::string text, int &w, int &h, int &numInfos); //you're responsible for deleting the infos
   bool isTextureMapUpdated();
   unsigned char* getTextureMap(int &w, int &h);

   //gettexturemap
};

class FontTools {
   FT_Library library;

   std::map<std::string, std::pair<int, Font*>> cachedFonts;

public:
   FontTools();

   void loadFont(std::string fontName, int fontSize);
   Font* getFont(std::string fontName, int fontSize);

   void unloadFont(std::string fontName, int fontSize);
};

#endif /* defined(__Musee__FontTools__) */
