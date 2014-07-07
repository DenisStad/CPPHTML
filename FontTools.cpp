//
//  FontTools.cpp
//  Musee
//
//  Created by Denis Stadniczuk on 27/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "FontTools.h"
#include FT_GLYPH_H

//#if defined(_WIN32)
//#define FONTDIR "%WINDIR%\fonts"
//#elif defined(_APPLE_) && defined(_MACH_)
#define FONTDIR "/Library/Fonts/"
//#elif defined(linux) || defined(__linux)
//#define FONTDIR "/usr/share/fonts"
//#endif

Font::Font(FT_Library lib, std::string fileName, int fontSize)
{

   textureMapData = NULL;
   textureMapLength = 0;

   currentQuadrant = 0;
   lastAddedCPX = 0;
   lastAddedCPY = 0;
   maxH = 0;

   dirtyTexture = false;

   FT_New_Face(lib, fileName.c_str(), 0, &face);

   fontSize /= 3;
   FT_Set_Char_Size(
                    face,    /* handle to face object           */
                    0,       /* char_width in 1/64th of points  */
                    fontSize*64,   /* char_height in 1/64th of points */
                    300,     /* horizontal device resolution    */
                    300 );   /* vertical device resolution      */
}

GlyphPosition* Font::stringInfo(std::string text, int &w, int &h, int &numInfos)
{
   const char *language = "EN";

   hb_font_t *hb_ft_font = hb_ft_font_create( face, NULL );
   hb_buffer_t *buffer = hb_buffer_create();

   hb_buffer_set_direction( buffer, HB_DIRECTION_LTR );
   hb_buffer_set_script( buffer, HB_SCRIPT_LATIN );
   hb_buffer_set_language( buffer, hb_language_from_string(language, strlen(language)) );

   hb_buffer_add_utf8( buffer, text.c_str(), text.length(), 0, text.length() );
   hb_shape( hb_ft_font, buffer, NULL, 0 );

   unsigned int         glyph_count;
   hb_glyph_info_t     *glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);

   //no texture map yet
   if (textureMapData == NULL) {
      textureMapLength = 512;
      textureMapLength = 128;
      textureMapData = (unsigned char*) malloc(sizeof(unsigned char) * 4 * textureMapLength * textureMapLength);

      for (int i = 0; i < 4 * textureMapLength * textureMapLength; i++) {
         textureMapData[i] = 0;
      }
   }

   FT_Int32 flags =  FT_LOAD_RENDER;
   //iterate to see if we have all codepoints in map
   for (int i = 0; i < glyph_count; ++i)
   {
      if(codepointPosition.find(glyph_info[i].codepoint)==codepointPosition.end()) {

         dirtyTexture = true;

         FT_Load_Glyph( face, glyph_info[i].codepoint, flags );
         FT_Bitmap bitmap = face->glyph->bitmap;

         int nextLength = textureMapLength;
         if (currentQuadrant == 1) {
            nextLength /= 2;
         }
         float nx = lastAddedCPX + bitmap.width;
         if (nx > nextLength) { //doesn't fit in this row, so add it to next one

            if (currentQuadrant == 0) {
               lastAddedCPX = 0;
            } else if (currentQuadrant == 1) {
               lastAddedCPX = 0;
            } else if (currentQuadrant == 2) {
               lastAddedCPX = textureMapLength/2 +1;
            } else if (currentQuadrant == 3) {
               lastAddedCPX = 0;
            }

            lastAddedCPY += maxH + 1;
            maxH = 0;
         }

         nextLength = textureMapLength;
         if (currentQuadrant == 2) {
            nextLength /= 2;
         }

         float ny = lastAddedCPY + bitmap.rows;
         if (ny > nextLength) {
            if (currentQuadrant == 0 || currentQuadrant == 3) { //need to grow
               currentQuadrant = 1;

               textureMapLength *= 2;
               unsigned char *newTextureMapData = (unsigned char*) malloc(sizeof(unsigned char) * 4 * textureMapLength * textureMapLength);

               for (int r = 0; r < textureMapLength; r++) {
                  for (int c = 0; c < textureMapLength; c++) {
                     if (r < textureMapLength/2 && c < textureMapLength/2) {
                        newTextureMapData[(r*textureMapLength + c) *4 + 0] = textureMapData[(r*textureMapLength/2 + c) *4 + 0];
                        newTextureMapData[(r*textureMapLength + c) *4 + 1] = textureMapData[(r*textureMapLength/2 + c) *4 + 1];
                        newTextureMapData[(r*textureMapLength + c) *4 + 2] = textureMapData[(r*textureMapLength/2 + c) *4 + 2];
                        newTextureMapData[(r*textureMapLength + c) *4 + 3] = textureMapData[(r*textureMapLength/2 + c) *4 + 3];
                     } else {
                        newTextureMapData[(r*textureMapLength + c) *4 + 0] = 0;
                        newTextureMapData[(r*textureMapLength + c) *4 + 1] = 0;
                        newTextureMapData[(r*textureMapLength + c) *4 + 2] = 0;
                        newTextureMapData[(r*textureMapLength + c) *4 + 3] = 0;
                     }
                  }
               }

               free(textureMapData);
               textureMapData = newTextureMapData;

            }
            currentQuadrant++;

            if (currentQuadrant == 2) {
               lastAddedCPX = textureMapLength/2 +1;
               lastAddedCPY = 0;
            } else if (currentQuadrant == 3) {
               lastAddedCPX = 0;
               lastAddedCPY = textureMapLength/2 +1;
            } else if (currentQuadrant == 4) {
               lastAddedCPX = textureMapLength/2 +1;
               lastAddedCPY = textureMapLength/2 +1;
            }
            maxH = 0;
         }


         if (bitmap.rows > maxH) {
            maxH = bitmap.rows;
         }

         for (int r = 0; r < bitmap.rows; r++) {
            for (int c = 0; c < bitmap.width; c++) {

               textureMapData[((lastAddedCPY+r)*textureMapLength + lastAddedCPX+c) *4 + 0] = 255;
               textureMapData[((lastAddedCPY+r)*textureMapLength + lastAddedCPX+c) *4 + 1] = 255;
               textureMapData[((lastAddedCPY+r)*textureMapLength + lastAddedCPX+c) *4 + 2] = 255;
               textureMapData[((lastAddedCPY+r)*textureMapLength + lastAddedCPX+c) *4 + 3] = bitmap.buffer[r*bitmap.width+c];
            }
         }

         GlyphInfo info;
         info.u0 = lastAddedCPX;
         info.v0 = lastAddedCPY;
         info.u1 = info.u0 + bitmap.width;
         info.v1 = info.v0 + bitmap.rows;

         info.w = face->glyph->metrics.width/64;
         info.h = face->glyph->metrics.height/64;

         info.offsetX = face->glyph->metrics.horiBearingX/64;
         info.offsetY = face->glyph->metrics.horiBearingY/64;

         codepointPosition[glyph_info[i].codepoint] = info;

         lastAddedCPX += bitmap.width+1;
      }
   }

   hb_glyph_position_t *glyph_pos  = hb_buffer_get_glyph_positions(buffer, &glyph_count);
   GlyphPosition *positions = (GlyphPosition*) malloc(sizeof(GlyphPosition) * glyph_count);
   numInfos = glyph_count;

   w = 0;
   h = 0;

   int x = 0;

   for (int i = 0; i < glyph_count; ++i)
   {
      GlyphInfo info = codepointPosition[glyph_info[i].codepoint];

//      printf("%d %d %d %d\n", glyph_pos[i].x_offset, glyph_pos[i].y_offset, glyph_pos[i].x_advance, glyph_pos[i].y_advance);

      GlyphPosition position;
//      info.x = glyph_pos[i].x_offset/64;
//      info.y = glyph_pos[i].y_offset/64;
//      info.w = glyph_pos[i].x_advance/64;
//      info.h = glyph_pos[i].y_advance/64;

      position.x = x + glyph_pos[i].x_offset/64 + info.offsetX;
      position.y = glyph_pos[i].y_offset/64 - info.offsetY;
      position.w = info.w;
      position.h = info.h;

      x += glyph_pos[i].x_advance/64;


      w = position.x + position.w;
      h = position.y + position.h;

      position.u0 = info.u0 / (float)textureMapLength;
      position.v0 = info.v0 / (float)textureMapLength;
      position.u1 = info.u1 / (float)textureMapLength;
      position.v1 = info.v1 / (float)textureMapLength;

      positions[i] = position;
   }
   
   /* clean up the buffer, but don't kill it just yet */
   hb_buffer_reset(buffer);
   
   /* Cleanup */
   hb_buffer_destroy( buffer );
   return positions;
}

bool Font::isTextureMapUpdated()
{
   return dirtyTexture;
}

unsigned char* Font::getTextureMap(int &w, int &h)
{
   dirtyTexture = false;
   w = textureMapLength;
   h = textureMapLength;
   return textureMapData;
}




FontTools::FontTools()
{
   FT_Init_FreeType(&library);
}

void FontTools::loadFont(std::string fontName, int fontSize)
{
   std::string key = fontName+":"+std::to_string(42);;
   std::map<std::string, std::pair<int, Font*>>::iterator it = cachedFonts.find(key);
   if (it != cachedFonts.end()) {
      it->second.first++;
   } else {
      std::string path = FONTDIR + fontName + ".ttf";
      Font *f = new Font(library, path, fontSize);
      cachedFonts[key] = std::make_pair(1, f);
   }
}

Font* FontTools::getFont(std::string fontName, int fontSize)
{
   std::string key = fontName+":"+std::to_string(42);;
   std::map<std::string, std::pair<int, Font*>>::iterator it = cachedFonts.find(key);
   if (it == cachedFonts.end()) {
      return NULL;
   }
   return it->second.second;
}

void FontTools::unloadFont(std::string fontName, int fontSize)
{
   std::string key = fontName+":"+std::to_string(42);;
   std::map<std::string, std::pair<int, Font*>>::iterator it = cachedFonts.find(key);
   if (it != cachedFonts.end()) {

      it->second.first--;
      if (it->second.first == 0) {
         //really unload it
         Font *f = it->second.second;
         cachedFonts.erase(it);
         delete f;
      }
   }
}