/*
** This source file is part of MY-BASIC
**
** For the latest info, see http://code.google.com/p/ascii-game-engine/
**
** Copyright (c) 2011 Tony & Tony's Toy Game Development Team
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __AGE_RENDERER_H__
#define __AGE_RENDERER_H__

#include "../ageconfig.h"
#include "../common/agetype.h"

typedef struct {
	s8 c;
	Color color;
} Pixel;

typedef struct {
	Size size;
	Pixel* tex;
} Frame;

typedef struct {
	Size frameSize;
	s32 frameCount;
	Frame* frames;
} Sprite;

typedef struct {
	Size size;
	Pixel* pixels;
	vector_t* sprites;
} Canvas;

AGE_API Canvas* create_canvas(void);
AGE_API void destroy_canvas(Canvas* _cvs);

AGE_API void set_cursor_visible(bl _vis);
AGE_API void goto_xy(s32 _x, s32 _y);
AGE_API void set_color(Color _col);

#endif /* __AGE_RENDERER_H__ */
