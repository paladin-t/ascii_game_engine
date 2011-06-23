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

#include "../common/ageallocator.h"
#include "../common/ageutil.h"
#include "agerenderer.h"

static const Color COLOR_MAP[] = {
	0,   1,   2,   3,   4,   5,   6,   7,
	8,   9,   10,  11,  12,  13,  14,  15,
};

Canvas* create_canvas(void) {
	int count = CANVAS_WIDTH * CANVAS_HEIGHT;
	Canvas* result = AGE_MALLOC(Canvas);

	result->size.w = CANVAS_WIDTH;
	result->size.h = CANVAS_HEIGHT;
	result->pixels = AGE_MALLOC_ARR(Pixel, count);

	return result;
}

void destroy_canvas(Canvas* _cvs) {
	AGE_FREE(_cvs->pixels);
	AGE_FREE(_cvs);
}

Sprite* create_sprite(Canvas* _cvs, const Str _shapeFile, const Str _brushFile, const Str _paleteFile) {
	Sprite* result = 0;
	FILE* fp = 0;
	s8 buf[AGE_STR_LEN];
	Str bs = buf;

	assert(_cvs);

	result = AGE_MALLOC(Sprite);

	/* Shape */
	fp = fopen(_shapeFile, "rb+");
	if(fp != 0) {
		freadln(fp, &bs);
		fclose(fp);
	}

	return result;
}

void destroy_sprite(Canvas* _cvs, Sprite* _spr) {
	assert(_cvs);

	AGE_FREE(_spr);
}

void set_cursor_visible(bl _vis) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = _vis;
	SetConsoleCursorInfo(hOut, &cci);
}

void goto_xy(s32 _x, s32 _y) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = _x;
	pos.Y = _y;
	SetConsoleCursorPosition(hOut, pos);
}

void set_color(Color _col) {
	HANDLE hConsole;
	assert(_col >= 0 && _col < _countof(COLOR_MAP));
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, COLOR_MAP[_col]);
}
