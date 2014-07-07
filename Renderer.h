//
//  Renderer.h
//  C++HTML
//
//  Created by Denis Stadniczuk on 04/07/14.
//  Copyright (c) 2014 Denis Stadniczuk. All rights reserved.
//

#ifndef __C__HTML__Renderer__
#define __C__HTML__Renderer__

#include <iostream>

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 3

#define BTN_DOWN 4
#define BTN_UP 5

class Renderer {

public:
   virtual void render() =0;
   virtual void setNeedsRerendering() =0;
   virtual void setViewportSize(int w, int h) =0;

   virtual void mouseBtn(int button, int state, int x, int y) =0;
   virtual void mouseMove(int x, int y) =0;
   virtual void mouseScroll(int x, int y, int dx, int dy) =0;
};

#endif /* defined(__C__HTML__Renderer__) */
