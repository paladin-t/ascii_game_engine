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
#include "../common/agestringtable.h"
#include "agerenderer.h"

static const Color COLOR_MAP[] = {
	0,   1,   2,   3,   4,   5,   6,   7,
	8,   9,   10,  11,  12,  13,  14,  15,
};

static s32 _destroy_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	destroy_sprite((Canvas*)spr->owner, spr);

	return result;
}

static s32 _update_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->control) {
		spr->control(
			spr,
			spr->name,
			cvs->context.lastElapsedTime,
			cvs->context.lastLParam,
			cvs->context.lastWParam,
			cvs->context.lastExtra
		);
	}

	return result;
}

static s32 _fire_render_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	fire_render_sprite(cvs, spr, cvs->context.lastElapsedTime);

	return result;
}

static s32 _post_render_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	post_render_sprite(cvs, spr, cvs->context.lastElapsedTime);

	return result;
}

Canvas* create_canvas(const Str _name) {
	int count = CANVAS_WIDTH * CANVAS_HEIGHT;
	Str name = copy_string(_name);
	Canvas* result = AGE_MALLOC(Canvas);

	result->name = name;
	result->size.w = CANVAS_WIDTH;
	result->size.h = CANVAS_HEIGHT;
	result->pixels = AGE_MALLOC_ARR(Pixel, count);
	result->sprites = ht_create(0, ht_cmp_string, ht_hash_string, 0);

	return result;
}

void destroy_canvas(Canvas* _cvs) {
	destroy_all_sprites(_cvs);
	ht_destroy(_cvs->sprites);
	AGE_FREE(_cvs->pixels);
	AGE_FREE(_cvs->name);
	AGE_FREE(_cvs);
}

void update_canvas(Canvas* _cvs, s32 _elapsedTime) {
	ControlProc ctrl = 0;

	_cvs->context.lastElapsedTime = _elapsedTime;
	_cvs->context.lastLParam = 0;
	_cvs->context.lastWParam = 0;
	_cvs->context.lastExtra = 0;

	ctrl = get_controller_canvas(_cvs);
	if(ctrl) {
		ctrl(_cvs, _cvs->name, _elapsedTime, 0, 0, 0);
	}
	ht_foreach(_cvs->sprites, _update_sprite);
}

void render_canvas(Canvas* _cvs, s32 _elapsedTime) {
	ht_foreach(_cvs->sprites, _fire_render_sprite);
	ht_foreach(_cvs->sprites, _post_render_sprite);
}

Sprite* get_sprite_by_name(Canvas* _cvs, const Str _name) {
	Sprite* result = 0;

	assert(_cvs);

	ht_get(_cvs->sprites, _name, &result);

	return result;
}

Sprite* create_sprite(Canvas* _cvs, const Str _name, const Str _shapeFile, const Str _brushFile, const Str _paleteFile) {
	Sprite* result = 0;
	FILE* fp = 0;
	s8 buf[AGE_STR_LEN];
	Str str = 0;
	Str bs = buf;
	s32 c = 0;
	s32 w = 0;
	s32 h = 0;
	ht_node_t* sprites = 0;

	assert(_cvs);

	if(!get_sprite_by_name(_cvs, _name)) {
		result = AGE_MALLOC(Sprite);
		result->name = copy_string(_name);
		result->owner = (struct Canvas*)_cvs;

		/* shape */
		fp = fopen(_shapeFile, "rb+");
		if(fp != 0) {
			/* frame count */
			freadln(fp, &bs);
			str = buf + strlen(ST_SPRITE_FRAME_COUNT);
			c = atoi(str);
			/* frame width */
			freadln(fp, &bs);
			str = buf + strlen(ST_SPRITE_FRAME_WIDTH);
			w = atoi(str);
			/* frame height */
			freadln(fp, &bs);
			str = buf + strlen(ST_SPRITE_FRAME_HEIGHT);
			h = atoi(str);
			/* close */
			fclose(fp);

			/* add to canvas */
			sprites = _cvs->sprites;
			ht_set_or_insert(sprites, _name, result);
		}
	}

	return result;
}

void destroy_sprite(Canvas* _cvs, Sprite* _spr) {
	assert(_cvs);

	AGE_FREE(_spr->name);
	AGE_FREE(_spr);
}

void destroy_all_sprites(Canvas* _cvs) {
	assert(_cvs);

	ht_foreach(_cvs->sprites, _destroy_sprite);
	ht_clear(_cvs->sprites);
}

void fire_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
}

void post_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
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
