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
	s32 k = 0;

	Sprite* spr = (Sprite*)_data;
	for(k = 0; k < spr->frameCount; ++k) {
		AGE_FREE_N(spr->timeLine.frames[k].tex);
	}
	AGE_FREE_N(spr->timeLine.frames);
	destroy_sprite((Canvas*)spr->owner, spr);

	return result;
}

static s32 _update_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->update) {
		spr->update(cvs, spr, cvs->context.lastElapsedTime);
	} else {
		update_sprite(cvs, spr, cvs->context.lastElapsedTime);
	}
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
	if(spr->fireRender) {
		spr->fireRender(cvs, spr, cvs->context.lastElapsedTime);
	} else {
		fire_render_sprite(cvs, spr, cvs->context.lastElapsedTime);
	}

	return result;
}

static s32 _post_render_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->postRender) {
		spr->postRender(cvs, spr, cvs->context.lastElapsedTime);
	} else {
		post_render_sprite(cvs, spr, cvs->context.lastElapsedTime);
	}

	return result;
}

static bl _create_sprite_shape(Canvas* _cvs, Sprite* _spr, const Str _shapeFile) {
	bl result = TRUE;
	FILE* fp = 0;
	s8 buf[AGE_STR_LEN];
	Str str = 0;
	Str bs = buf;
	s32 c = 0;
	s32 w = 0;
	s32 h = 0;
	f32 r = 1.0f;
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
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
		/* frame rate */
		freadln(fp, &bs);
		str = buf + strlen(ST_SPRITE_FRAME_RATE);
		r = (f32)atof(str);
		fskipln(fp);
		/* assignment */
		_spr->frameCount = c;
		_spr->frameSize.w = w;
		_spr->frameSize.h = h;
		_spr->frameRate = r;
		/* frames */
		_spr->timeLine.frames = AGE_MALLOC_N(Frame, c);
		for(k = 0; k < c; ++k) {
			_spr->timeLine.frames[k].tex = AGE_MALLOC_N(Pixel, w * h);
			for(j = 0; j < h; ++j) {
				freadln(fp, &bs);
				for(i = 0; i < w; ++i) {
					_spr->timeLine.frames[k].tex[i + j * w].parent = &_spr->timeLine.frames[k];
					_spr->timeLine.frames[k].tex[i + j * w].shape = bs[i];
				}
			}
			fskipln(fp);
		}
		/* close */
		fclose(fp);
	} else {
		result = FALSE;
	}

	return result;
}

static bl _create_sprite_brush(Canvas* _cvs, Sprite* _spr, const Str _brushFile) {
	bl result = TRUE;
	FILE* fp = 0;
	s8 buf[AGE_STR_LEN];
	Str str = 0;
	Str bs = buf;
	s32 c = 0;
	s32 w = 0;
	s32 h = 0;
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
	fp = fopen(_brushFile, "rb+");
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
		fskipln(fp);
		/* checking */
		assert(_spr->frameCount == c);
		assert(_spr->frameSize.w == w);
		assert(_spr->frameSize.h == h);
		/* frames */
		for(k = 0; k < c; ++k) {
			for(j = 0; j < h; ++j) {
				freadln(fp, &bs);
				for(i = 0; i < w; ++i) {
					_spr->timeLine.frames[k].tex[i + j * w].brush = bs[i];
				}
			}
			fskipln(fp);
		}
		/* close */
		fclose(fp);
	} else {
		result = FALSE;
	}

	return result;
}

static bl _create_sprite_palete(Canvas* _cvs, Sprite* _spr, const Str _paleteFile) {
	bl result = TRUE;
	FILE* fp = 0;
	s8 buf[AGE_STR_LEN];
	Str str = 0;
	Str bs = buf;
	s32 c = 0;
	Color palete[256];
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
	s32 b = 0;
	memset(palete, 0, sizeof(palete));
	fp = fopen(_paleteFile, "rb+");
	if(fp != 0) {
		while(!feof(fp)) {
			freadln(fp, &bs);
			if(strlen(bs) == 0) {
				break;
			}
			str = buf + strlen("x: ");
			c = atoi(str);
			palete[(s32)buf[0]] = (Color)c;
		}
		/* close */
		fclose(fp);
		/* paint */
		for(k = 0; k < _spr->frameCount; ++k) {
			for(j = 0; j < _spr->frameSize.h; ++j) {
				for(i = 0; i < _spr->frameSize.w; ++i) {
					b = (s32)_spr->timeLine.frames[k].tex[i + j * _spr->frameSize.w].brush;
					_spr->timeLine.frames[k].tex[i + j * _spr->frameSize.w].color = palete[b];
				}
			}
		}
	} else {
		result = FALSE;
	}

	return result;
}

Canvas* create_canvas(const Str _name) {
	int count = CANVAS_WIDTH * CANVAS_HEIGHT;
	Str name = copy_string(_name);
	Canvas* result = AGE_MALLOC(Canvas);

	result->name = name;
	result->size.w = CANVAS_WIDTH;
	result->size.h = CANVAS_HEIGHT;
	result->pixels = AGE_MALLOC_N(Pixel, count);
	result->sprites = ht_create(0, ht_cmp_string, ht_hash_string, 0);

	return result;
}

void destroy_canvas(Canvas* _cvs) {
	destroy_all_sprites(_cvs);
	ht_destroy(_cvs->sprites);
	AGE_FREE_N(_cvs->pixels);
	AGE_FREE(_cvs->name);
	AGE_FREE(_cvs);
}

void set_frame_rate(Canvas* _cvs, s32 _rate) {
	assert(_cvs && _rate > 0);

	_cvs->frameRate = _rate;
}

s32 get_frame_rate(Canvas* _cvs) {
	assert(_cvs);

	return _cvs->frameRate;
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
	s32 x = 0;
	s32 y = 0;
	Pixel* pixelc = 0;

	/* fill frame buffer */
	ht_foreach(_cvs->sprites, _fire_render_sprite);
	ht_foreach(_cvs->sprites, _post_render_sprite);

	/* render frame buffer to target */
	for(y = 0; y < _cvs->size.h; ++y) {
		for(x = 0; x < _cvs->size.w; ++x) {
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			if(pixelc->color == ERASE_PIXEL_COLOR) {
				set_color(_cvs, 0);
				goto_xy(x, y);
				putch(ERASE_PIXEL_SHAPE);
				pixelc->color = 0;
			} else if(pixelc->shape) {
				set_color(_cvs, pixelc->color);
				goto_xy(x, y);
				putch(pixelc->shape);
			}
		}
	}
}

Sprite* get_sprite_by_name(Canvas* _cvs, const Str _name) {
	Sprite* result = 0;

	assert(_cvs);

	ht_get(_cvs->sprites, _name, &result);

	return result;
}

Sprite* create_sprite(Canvas* _cvs, const Str _name, const Str _shapeFile, const Str _brushFile, const Str _paleteFile) {
	Sprite* result = 0;
	ht_node_t* sprites = 0;

	assert(_cvs);

	if(!get_sprite_by_name(_cvs, _name)) {
		result = AGE_MALLOC(Sprite);
		result->name = copy_string(_name);
		result->owner = _cvs;
		result->timeLine.namedFrames = ht_create(0, ht_cmp_string, ht_hash_string, 0);

		_create_sprite_shape(_cvs, result, _shapeFile);
		_create_sprite_brush(_cvs, result, _brushFile);
		_create_sprite_palete(_cvs, result, _paleteFile);

		create_message_map_sprite(result);

		sprites = _cvs->sprites;
		ht_set_or_insert(sprites, _name, result);
	}

	return result;
}

void destroy_sprite(Canvas* _cvs, Sprite* _spr) {
	assert(_cvs);

	destroy_message_map_sprite(_spr);
	ht_destroy(_spr->timeLine.namedFrames);
	AGE_FREE(_spr->name);
	AGE_FREE(_spr);
}

void destroy_all_sprites(Canvas* _cvs) {
	assert(_cvs);

	ht_foreach(_cvs->sprites, _destroy_sprite);
	ht_clear(_cvs->sprites);
}

bl set_position_sprite(Canvas* _cvs, Sprite* _spr, s32 _x, s32 _y) {
	bl result = TRUE;

	_spr->position.x = _x;
	_spr->position.y = _y;

	return result;
}

bl get_position_sprite(Canvas* _cvs, Sprite* _spr, s32* _x, s32* _y) {
	bl result = TRUE;

	if(_x) {
		*_x = _spr->position.x;
	}
	if(_y) {
		*_y = _spr->position.y;
	}

	return result;
}

bl play_sprite(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, bl _loop, SpritePlayingCallbackFunc _cb) {
	bl result = TRUE;

	// TODO

	return result;
}

bl stop_sprite(Canvas* _cvs, Sprite* _spr, s32 _stopAt) {
	bl result = TRUE;

	// TODO

	return result;
}

void update_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	s32 tickableTime = 0;

	tickableTime = (s32)(_cvs->frameRate * _spr->frameRate);
	_spr->frameTick += _elapsedTime;
	if(_spr->frameTick >= tickableTime) {
		_spr->frameTick -= tickableTime;
		++_spr->currentFrame;
		if(_spr->timeLine.begin && _spr->timeLine.end) {
			// TODO
		} else {
			if(_spr->currentFrame >= _spr->frameCount) {
				_spr->currentFrame = 0;
			}
		}
	}
}

void fire_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
	s32 x = 0;
	s32 y = 0;
	s32 s = 0;
	s32 itf = 0;
	s32 found = -1;
	Pixel* pixelf = 0;
	Pixel* pixelc = 0;

	k = _spr->currentFrame;
	for(j = 0; j < _spr->frameSize.h; ++j) {
		y = _spr->oldPosition.y + j;
		if(y < 0 || y >= _cvs->size.h) {
			continue;
		}
		for(i = 0; i < _spr->frameSize.w; ++i) {
			x = _spr->oldPosition.x + i;
			if(x < 0 || x >= _cvs->size.w) {
				continue;
			}
			pixelf = &_spr->timeLine.frames[k].tex[i + j * _spr->frameSize.w];
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			s = (s32)pixelf->shape;
			if(s) {
				pixelc->shape = 0;
				pixelc->color = ERASE_PIXEL_COLOR;
				pixelc->zorder = DEFAULT_Z_ORDER;
				found = -1;
				for(itf = 0; itf < pixelc->frameCount; ++itf) {
					if(pixelc->ownerFrames[itf] == pixelf->parent) {
						found = itf;
						break;
					}
				}
				if(found != -1) {
					pixelc->ownerFrames[found] =
						pixelc->ownerFrames[
							--pixelc->frameCount
						];
				}
			}
		}
	}
	_spr->oldPosition = _spr->position;
}

void post_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
	s32 x = 0;
	s32 y = 0;
	s32 s = 0;
	Pixel* pixelf = 0;
	Pixel* pixelc = 0;

	k = _spr->currentFrame;
	for(j = 0; j < _spr->frameSize.h; ++j) {
		y = _spr->position.y + j;
		if(y < 0 || y >= _cvs->size.h) {
			continue;
		}
		for(i = 0; i < _spr->frameSize.w; ++i) {
			x = _spr->position.x + i;
			if(x < 0 || x >= _cvs->size.w) {
				continue;
			}
			pixelf = &_spr->timeLine.frames[k].tex[i + j * _spr->frameSize.w];
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			s = (s32)pixelf->shape;
			if(s && pixelf->zorder <= pixelc->zorder
				&& pixelc->frameCount < MAX_CACHED_FRAME_COUNT) {
				pixelc->shape = (s8)s;
				pixelc->color = pixelf->color;
				pixelc->zorder = pixelf->zorder;
				pixelc->ownerFrames[
					pixelc->frameCount++
				] = pixelf->parent;
			}
		}
	}
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

void set_color(Canvas* _cvs, Color _col) {
	HANDLE hConsole;
	if(_col != _cvs->context.lastColor) {
		assert(_col >= 0 && _col < _countof(COLOR_MAP));
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, COLOR_MAP[_col]);
		_cvs->context.lastColor = _col;
	}
}
