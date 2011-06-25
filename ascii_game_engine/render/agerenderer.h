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
#include "../common/agelist.h"
#include "../common/agehashtable.h"
#include "../message/agemessage.h"

#define MAX_CACHED_FRAME_COUNT 16

struct Frame;
struct Sprite;

typedef struct {
	s8 shape;
	Color color;
	s32 zorder;

	struct Frame* ownerFrames[MAX_CACHED_FRAME_COUNT];
	s32 frameCount;
} Pixel;

typedef struct {
	struct Sprite* parent;
	Pixel* tex;
} Frame;

typedef struct {
	Size frameSize;

	Frame* frames;
	s32 frameCount;

	s32 currentFrame;

	MessageMap messageMap;
	ControlProc control;
} Sprite;

typedef struct {
	Size size;
	Pixel* pixels;

	ht_node_t* sprites;
	s32 spriteCount;
} Canvas;

AGE_API Canvas* create_canvas(void);
AGE_API void destroy_canvas(Canvas* _cvs);

AGE_API void update_canvas(Canvas* _cvs, s32 _elapsedTime);
AGE_API void render_canvas(Canvas* _cvs, s32 _elapsedTime);

AGE_API Sprite* create_sprite(Canvas* _cvs, const Str _name, const Str _shapeFile, const Str _brushFile, const Str _paleteFile);
AGE_API void destroy_sprite(Canvas* _cvs, Sprite* _spr);

AGE_API void set_cursor_visible(bl _vis);
AGE_API void goto_xy(s32 _x, s32 _y);
AGE_API void set_color(Color _col);

#endif /* __AGE_RENDERER_H__ */
