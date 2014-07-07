//
//  HtmlOpenGLRenderer.h
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __Musee__HtmlOpenGLRenderer__
#define __Musee__HtmlOpenGLRenderer__

#include <iostream>

// The OpenGL libraries, make sure to include the GLUT and OpenGL frameworks
#include <OpenGL/gl.h>

#include "Renderer.h"

#include "HtmlElement.h"
#include "HtmlText.h"
#include "FontTools.h"

#include <map>

void setNeedsDisplay();

class HtmlOpenGLRenderer : public Renderer {

   HtmlDocument* html;
   FontTools fontTools;
   std::map<Font*, GLuint> font_texture;

   HtmlOpenGLRenderer();

   float viewportWidth, viewportHeight;
   float x, y;
   std::vector<HtmlNode*> toDraw;

   void draw(HtmlNode *node);
   void traverse(HtmlNode *node, float z, float &w, float &h, float parentW, float parentH);


   //click event
   int downX, downY;
   void traverseEvent(HtmlElement *node, Event &event, int x, int y);
public:

   static void initialize();
   static HtmlOpenGLRenderer* sharedRenderer();

   void setDocumentToRender(HtmlDocument* document);
   virtual void render();
   virtual void setNeedsRerendering();
   void rerenderIfNecessary();

   virtual void setViewportSize(int w, int h);
//   void enterMainLoop();

   virtual void mouseBtn(int button, int state, int x, int y);
   virtual void mouseMove(int x, int y);
   virtual void mouseScroll(int x, int y, int dx, int dy);
};

#endif /* defined(__Musee__HtmlOpenGLRenderer__) */
