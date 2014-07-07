//
//  HtmlRenderer.cpp
//  Musee
//
//  Created by Denis Stadniczuk on 24/06/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#include "HtmlOpenGLRenderer.h"
#include "HtmlDocument.h"

#include <fstream>

// This is just an example using basic glut functionality.
// If you want specific Apple functionality, look up AGL

static HtmlOpenGLRenderer *sharedInstance;
//
//// Called at the start of the program, after a glutPostRedisplay() and during idle
//// to display a frame
//void display()
//{
//   sharedInstance->render();
//}
//
//void idle()
//{
//   sharedInstance->rerenderIfNecessary();
//}
//
//void mouse(int button, int state, int x, int y)
//{
//   sharedInstance->mouseBtn(button, state, x, y);
//}

void HtmlOpenGLRenderer::initialize()
{
   sharedInstance = new HtmlOpenGLRenderer();
}

HtmlOpenGLRenderer* HtmlOpenGLRenderer::sharedRenderer()
{
   return sharedInstance;
}

HtmlOpenGLRenderer::HtmlOpenGLRenderer()
{
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   glShadeModel(GL_SMOOTH);
   
   html = NULL;
}

void HtmlOpenGLRenderer::setDocumentToRender(HtmlDocument* document)
{
   html = document;
}

void HtmlOpenGLRenderer::render()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   if (html == NULL) {
      return;
   }

   html->reCalcStyles();
   
   toDraw.clear();

   float w = 0, h = 0;
   traverse(html->body, 1, w, h, viewportWidth, viewportHeight);

   html->body->setCalculatedBounds(0, 0, 0, viewportWidth, viewportHeight, viewportWidth, viewportHeight);
   toDraw.push_back(html->body);

   for (int i = (int)toDraw.size()-1; i >= 0; i--) {
      draw(toDraw[i]);
   }

//   glutSwapBuffers();
}

void HtmlOpenGLRenderer::setNeedsRerendering()
{
//   glutPostRedisplay();
   setNeedsDisplay();
}

void HtmlOpenGLRenderer::rerenderIfNecessary()
{
//   if (html->needsReRendering) {
//      html->reCalcStyles();
//      render();
//      html->needsReRendering = false;
//   }
}

void HtmlOpenGLRenderer::setViewportSize(int w, int h)
{
   viewportWidth = w;
   viewportHeight = h;

   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, w, h, 0, -1000, 1000);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void HtmlOpenGLRenderer::traverse(HtmlNode *node, float z, float &w, float &h, float parentW, float parentH)
{
   float pt, pr, pb, pl;
   node->calculatedStyle.calcPadding(pt, pr, pb, pl, parentW, parentH);

   float bt, br, bb, bl;
   node->calculatedStyle.calcBorderWidth(bt, br, bb, bl);

   float nx = pl + bl, ny = pt + bt;
   float maxChildW = 0, maxChildH = 0;


   float maxW = -1;
   float maxH = -1;
   maxW = parentW;
   maxH = parentH;
   node->calculatedStyle.calcWidthHeight(maxW, maxH, parentW, parentH);
   maxW -= pl + bl + pr + br;
   maxH -= pt + bt + pb + bb;

   if (node->getNodeType() == ELEMENT) {
      HtmlElement *elem = (HtmlElement*) node;
      for (int i = 0; i < elem->childNodes().size(); i++) {
         float nw, nh;
         traverse(elem->childNodes()[i], z+1, nw, nh, maxW, maxH);

         if (nx + nw - pl - bl > maxW || elem->childNodes()[i]->getTagName() == "LI" || elem->childNodes()[i]->getTagName() == "TR") {
            ny = maxChildH + pt + bt;
            nx = pl + bl;
         }

         if (ny + nh - pt - bt > maxChildH) {
            maxChildH = ny + nh - pt - bt;
         }

         if (nx + nw - pl - bl > maxChildW) {
            maxChildW = nx + nw - pl - bl;
         }

         float nmaxw = nw;
         if (nmaxw > maxW) {
            nmaxw = maxW;
         }
         float nmaxh = nh;
         if (nmaxh > maxH) {
            nmaxh = maxH;
         }

         elem->childNodes()[i]->setCalculatedBounds(nx, ny, z, nmaxw, nmaxh, nw, nh);

         printf(" pushing %s %f %f %f %f/%f %f/%f\n", elem->childNodes()[i]->getTagName().c_str(), nx, ny, z, nw, nmaxw, nh, nmaxh);

         toDraw.push_back(elem->childNodes()[i]);

         nx += nw;
      }

   } else if (node->getNodeType() == TEXT) {

      HtmlText *tNode = (HtmlText*)node;

//      printf("### %s\n", tNode->text.c_str());

      std::string fontName = "Arial";
      if (node->calculatedStyle.font != "") {
         fontName = node->calculatedStyle.font;
      }

      float size;
      node->calculatedStyle.calcFontSize(size);
      Font *font = fontTools.getFont(fontName, size);

      if (font == NULL) {
         fontTools.loadFont(fontName, size);
         font = fontTools.getFont(fontName, size);
      }

      int w, h;
      int num;
//      std::string text = tNode->text();
      GlyphPosition *info = font->stringInfo(tNode->text(), w, h, num);

      tNode->characterPositions.clear();

      int lineHeight = size;

//      maxChildH = 0;
      for (int i = 0; i < num; i++) {
         tNode->characterPositions.push_back(info[i]);
//         if (info[i].h > maxChildH) {
//            maxChildH = info[i].h;
//         }
      }

      maxChildW = info[num-1].x+info[num-1].w;
      maxChildH = lineHeight;

      free(info);
   }

   if (node->calculatedStyle.display == DISPLAY_BLOCK) {
      maxW = parentW - pl - pr - bl - br;
   } else {
      maxW = maxChildW;
   }

   maxH = maxChildH;

   if (maxW < maxChildW) {
      maxW = maxChildW;
   }

   w = bl + pl + maxW + pr + br;
   h = bt + pt + maxH + pb + bb;

   node->calculatedStyle.calcWidthHeight(w, h, parentW, parentH);

//   printf("   %s size %f %f \n", node->getTagName().c_str(), w, h);
}

void HtmlOpenGLRenderer::draw(HtmlNode *node)
{

   glEnable(GL_STENCIL_TEST);
   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
   glDepthMask(GL_FALSE);
   glStencilFunc(GL_NEVER, 0, 0xFF);
   glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

   // draw stencil pattern
   glStencilMask(0xFF);
   glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF


   float x, y, z, w, h, scrollw, scrollh;
   node->getCalculatedBounds(x, y, z, w, h, scrollw, scrollh);
   if (node->getNodeType() == ELEMENT) {
      x -= ((HtmlElement*)node)->scrollLeft();
      y -= ((HtmlElement*)node)->scrollTop();
   }

   HtmlElement *nextEl = (HtmlElement*) node->parentNode();
   while (nextEl != NULL) {
      float nx, ny, nz, nw, nh, nsw, nsh;
      nextEl->getCalculatedBounds(nx, ny, nz, nw, nh, nsw, nsh);
      float nscrollx = 0, nscrolly = 0;
      nscrollx = nextEl->scrollLeft();
      nscrolly = nextEl->scrollTop();

      x += nx - nscrollx;
      y += ny - nscrolly;

      nextEl = nextEl->parentNode();
   }

   std::vector<HtmlNode*> nodePath;
   HtmlNode* nextNode = node;
   while (nextNode != NULL) {
      nodePath.push_back(nextNode);
      nextNode = nextNode->parentNode();
   }

   int count = 0;
   float sx = 0, sy = 0;
   for (int i = nodePath.size()-1; i >= 0; i--) {
      float nx, ny, nz, nw, nh, nsw, nsh;
      nodePath[i]->getCalculatedBounds(nx, ny, nz, nw, nh, nsw, nsh);

      float nscrollx = 0, nscrolly = 0;
      if (nodePath[i]->getNodeType() == ELEMENT) {
         nscrollx = ((HtmlElement*)nodePath[i])->scrollLeft();
         nscrolly = ((HtmlElement*)nodePath[i])->scrollTop();
      }

      glBegin(GL_QUADS);

      sx += nx - nscrollx;
      sy += ny - nscrolly;

      glVertex3f(sx, sy, 0);
      glVertex3f(sx+nw, sy, 0);
      glVertex3f(sx+nw, sy+nh, 0);
      glVertex3f(sx, sy+nh, 0);
      
      glEnd();
      count++;


   }

   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
   glDepthMask(GL_TRUE);
   glStencilMask(0x00);

// draw only where stencil's value is 1
   glStencilFunc(GL_EQUAL, count, 0xFF);

////   if (node->getNodeType() == ELEMENT && ((HtmlElement*)node)->hasAttribute("id") && ((HtmlElement*)node)->getAttribute("id") == "input-list") {
//   if (node->getNodeType() == TEXT && ((HtmlText*)node)->text() == "/bin") {
//      unsigned char *data = (unsigned char*) malloc(sizeof(unsigned char) * 512 * 512);
//      glReadPixels(0, 0, 512, 512, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data);
//
//      std::ofstream myfile;
//      myfile.open ("/Users/denis/tmp/stencilBuffer.txt");
//
//      for (int i = 0; i < 512; i++) {
//         for (int j = 0; j < 512; j++) {
//            myfile << (int) data[i*512 + j] << " ";
//         }
//         myfile << std::endl;
//      }
//      myfile.close();
//      std::cout << count << std::endl;
//   }




   glPushMatrix();
   glTranslatef(x, y, z);

   if (node->calculatedStyle.borderLeftStyle == BORDERSTYLE_SOLID) {

      float bwl, bwr, bwt, bwb;
      node->calculatedStyle.calcBorderWidth(bwt, bwr, bwb, bwl);

      float br, bg, bb, ba;
      node->calculatedStyle.calcBorderLeftColor(br, bg, bb, ba);
      glColor4f(br, bg, bb, ba);

      glBegin(GL_TRIANGLES);

      //left
      glVertex3f(0, 0, 0);
      glVertex3f(bwl, 0, 0);
      glVertex3f(0, h, 0);

      glVertex3f(bwl, 0, 0);
      glVertex3f(bwl, h, 0);
      glVertex3f(0, h, 0);

      node->calculatedStyle.calcBorderRightColor(br, bg, bb, ba);
      glColor4f(br, bg, bb, ba);

      //right
      glVertex3f(w-bwr, 0, 0);
      glVertex3f(w, 0, 0);
      glVertex3f(w-bwr, h, 0);

      glVertex3f(w, 0, 0);
      glVertex3f(w, h, 0);
      glVertex3f(w-bwr, h, 0);

      node->calculatedStyle.calcBorderTopColor(br, bg, bb, ba);
      glColor4f(br, bg, bb, ba);

      //top
      glVertex3f(0, 0, 0);
      glVertex3f(w, 0, 0);
      glVertex3f(0, bwt, 0);

      glVertex3f(w, 0, 0);
      glVertex3f(w, bwt, 0);
      glVertex3f(0, bwt, 0);

      node->calculatedStyle.calcBorderBottomColor(br, bg, bb, ba);
      glColor4f(br, bg, bb, ba);

      //bottom
      glVertex3f(0, h-bwb, 0);
      glVertex3f(w, h-bwb, 0);
      glVertex3f(0, h, 0);

      glVertex3f(w, h-bwb, 0);
      glVertex3f(w, h, 0);
      glVertex3f(0, h, 0);

      glEnd();

      glTranslatef(bwl, bwt, 0);
   }

   float r, g, b, a;
   node->calculatedStyle.calcBGColor(r, g, b, a);

//   printf("drawing %s at %f %f %f %f %f %f %f", node->getTagName().c_str(), x, y, z, w, h, sw, sh);
//   if (node->getNodeType() == TEXT) {
//      printf(" (%s) ", ((HtmlText*)node)->text().c_str());
//   }
//   printf(" color %f %f %f %f \n", r, g, b, a);

   glColor4f(r, g, b, a);

   glBegin(GL_TRIANGLES);

   glVertex3f(0, 0, 0);
   glVertex3f(w, 0, 0);
   glVertex3f(0, h, 0);

   glVertex3f(w, 0, 0);
   glVertex3f(w, h, 0);
   glVertex3f(0, h, 0);

   glEnd();

   if (node->getNodeType() == TEXT) {

      HtmlText* tNode = (HtmlText*) node;

      std::string fontName = "Arial";
      if (node->calculatedStyle.font != "") {
         fontName = node->calculatedStyle.font;
      }

      glEnable(GL_TEXTURE_2D);

      float size;
      node->calculatedStyle.calcFontSize(size);
      Font *font = fontTools.getFont(fontName, size);

      if (font->isTextureMapUpdated()) {

         GLuint texture;
         if (font_texture.find(font) == font_texture.end()) {
            glGenTextures(1, &texture);
         } else {
            texture = font_texture[font];
         }

         glBindTexture(GL_TEXTURE_2D, texture);

         int tw, th;
         unsigned char* data = font->getTextureMap(tw, th);

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
         glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

         font_texture[font] = texture;
      }
      float r, g, b, a;
      tNode->calculatedStyle.calcTextColor(r, g, b, a);

      glColor4f(r, g, b, a);
      for (int i = 0; i < tNode->characterPositions.size(); i++) {
         glBegin(GL_QUADS);

         glTexCoord2f(tNode->characterPositions[i].u0, tNode->characterPositions[i].v0);
         glVertex3f(tNode->characterPositions[i].x, h+tNode->characterPositions[i].y, 1.0);

         glTexCoord2f(tNode->characterPositions[i].u1, tNode->characterPositions[i].v0);
         glVertex3f(tNode->characterPositions[i].x+tNode->characterPositions[i].w, h+tNode->characterPositions[i].y, 1.0);

         glTexCoord2f(tNode->characterPositions[i].u1, tNode->characterPositions[i].v1);
         glVertex3f(tNode->characterPositions[i].x+tNode->characterPositions[i].w, h+tNode->characterPositions[i].y+tNode->characterPositions[i].h, 1.0);

         glTexCoord2f(tNode->characterPositions[i].u0, tNode->characterPositions[i].v1);
         glVertex3f(tNode->characterPositions[i].x, h+tNode->characterPositions[i].y+tNode->characterPositions[i].h, 1.0);

         glEnd();
      }

      glDisable(GL_TEXTURE_2D);
   }

   glDisable(GL_STENCIL_TEST);
   glPopMatrix();
}

//void HtmlOpenGLRenderer::enterMainLoop()
//{
////   glutMainLoop();
//}


void HtmlOpenGLRenderer::traverseEvent(HtmlElement *node, Event &event, int x, int y)
{

   float cx, cy, cz, cw, ch, csw, csh;
   node->getCalculatedBounds(cx, cy, cz, cw, ch, csw, csh);
   HtmlNode *next = node->parentNode();
   while (next != NULL) {
      float nx, ny, nz, nw, nh, nsw, nsh;
      next->getCalculatedBounds(nx, ny, nz, nw, nh, nsw, nsh);
      cx += nx;
      cy += ny;
      next = next->parentNode();
   }

   if (cx <= x && x <= cx + cw && cy <= y && y <= cy + ch) {
      bool childIsTarget = false;
      for (int i = 0; i < node->childNodes().size(); i++) {

         if (node->childNodes()[i]->getNodeType() != ELEMENT) {
            continue;
         }

         float cx, cy, cz, cw, ch, csw, csh;
         node->childNodes()[i]->getCalculatedBounds(cx, cy, cz, cw, ch, csw, csh);
         HtmlNode *next = node->childNodes()[i]->parentNode();
         while (next != NULL) {
            float nx, ny, nz, nw, nh, nsw, nsh;
            next->getCalculatedBounds(nx, ny, nz, nw, nh, nsw, nsh);
            cx += nx;
            cy += ny;
            next = next->parentNode();
         }

         if (cx <= x && x <= cx + cw && cy <= y && y <= cy + ch) {
            childIsTarget = true;
            traverseEvent((HtmlElement*)node->childNodes()[i], event, x, y);
         }
      }

      if (!childIsTarget) {
         event.currentTarget = node;
         node->dispatchEvent(event);
      }
   }

//   if (child->hasAttribute("id")) {
//      printf("%s\n", child->getAttribute("id").c_str());
//   }
}


void HtmlOpenGLRenderer::mouseBtn(int button, int state, int x, int y)
{

   if (button == MOUSE_LEFT) {

      if (state == BTN_DOWN) {
         downX = x;
         downY = y;
         Event mouseDownEvent;
         mouseDownEvent.initEvent("mousedown", true, true);
         traverseEvent(html->body, mouseDownEvent, x, y);
      } else if (state == BTN_UP) {
         Event mouseUpEvent;
         mouseUpEvent.initEvent("mouseup", true, true);
         traverseEvent(html->body, mouseUpEvent, x, y);

         if (x == downX && y == downY) {
            Event clickEvent;
            clickEvent.initEvent("click", true, true);
            traverseEvent(html->body, clickEvent, x, y);
         }
      }
   }
}

void HtmlOpenGLRenderer::mouseMove(int x, int y)
{

}

void HtmlOpenGLRenderer::mouseScroll(int x, int y, int dx, int dy)
{
   Event scrollEvent;
   ScrollEventData *data = new ScrollEventData;
   data->x = x;
   data->y = y;
   data->deltaX = dx;
   data->deltaY = dy;
   scrollEvent.data = data;

   scrollEvent.initEvent("scroll", true, false);
   traverseEvent(html->body, scrollEvent, x, y);
}
