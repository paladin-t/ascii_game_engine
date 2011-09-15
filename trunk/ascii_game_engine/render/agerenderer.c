/*
** This source file is part of AGE
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

#if AGE_IMPL == AGE_IMPL_CONSOLE

#include "../common/ageallocator.h"
#include "../common/ageutil.h"
#include "../common/agestringtable.h"
#include "agerenderer.h"

static const Color COLOR_MAP[] = {
	0,   1,   2,   3,   4,   5,   6,   7,
	8,   9,   10,  11,  12,  13,  14,  15,
};

static Sprite* tobeRemoved = 0;

static void _destroy_sprite_impl(Canvas* _cvs, Sprite* _spr) {
	s32 k = 0;

	destroy_sprite_message_map(_spr);
	if(_spr->time_line.shape_file_name) {
		AGE_FREE(_spr->time_line.shape_file_name);
	}
	if(_spr->time_line.brush_file_name) {
		AGE_FREE(_spr->time_line.brush_file_name);
	}
	if(_spr->time_line.palete_file_name) {
		AGE_FREE(_spr->time_line.palete_file_name);
	}
	for(k = 0; k < _spr->time_line.frame_count; ++k) {
		AGE_FREE_N(_spr->time_line.frames[k].tex);
	}
	AGE_FREE_N(_spr->time_line.frames);
	ht_destroy(_spr->time_line.named_frames);
	if(_spr->time_line.begin_name) {
		AGE_FREE(_spr->time_line.begin_name);
	}
	if(_spr->time_line.end_name) {
		AGE_FREE(_spr->time_line.end_name);
	}
	if(_spr->userdata.destroy) {
		assert(_spr->userdata.data);
		_spr->userdata.destroy(_spr->userdata.data);
	}
	destroy_paramset(_spr->params);
	AGE_FREE(_spr->name);
	AGE_FREE(_spr);
}

static s32 _drop_sprite(Canvas* _cvs, Sprite* _spr) {
	s32 result = 0;

	++_cvs->dropped_sprites_count;
	if(_cvs->dropped_sprites_count > _cvs->dropped_sprites_size) {
		_cvs->dropped_sprites_size = _cvs->dropped_sprites_count + 8;
		_cvs->dropped_sprites = AGE_REALLOC_N(Sprite*, _cvs->dropped_sprites, _cvs->dropped_sprites_size);
	}
	_cvs->dropped_sprites[_cvs->dropped_sprites_count - 1] = _spr;

	return result;
}

static s32 _destroy_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	_drop_sprite(spr->owner, spr);

	return result;
}

static s32 _on_removing_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	if(spr != tobeRemoved) {
		if(spr->object_removed) {
			spr->object_removed(spr, tobeRemoved->owner, tobeRemoved);
		}
	}

	return result;
}

static s32 _destroy_string(Ptr _data, Ptr _extra) {
	s32 result = 0;
	s32 k = 0;

	Str fname = (Str)_extra;
	AGE_FREE(fname);

	return result;
}

static s32 _update_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->update) {
		spr->update(cvs, spr, cvs->context.last_elapsed_time);
	} else {
		update_sprite(cvs, spr, cvs->context.last_elapsed_time);
	}
	if(spr->control) {
		spr->control(
			spr,
			spr->name,
			cvs->context.last_elapsed_time,
			cvs->context.last_lparam,
			cvs->context.last_wparam,
			cvs->context.last_extra
		);
	}

	return result;
}

static s32 _fire_render_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->prev_render) {
		spr->prev_render(cvs, spr, cvs->context.last_elapsed_time);
	} else {
		prev_render_sprite(cvs, spr, cvs->context.last_elapsed_time);
	}

	return result;
}

static s32 _post_render_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->post_render) {
		spr->post_render(cvs, spr, cvs->context.last_elapsed_time);
	} else {
		post_render_sprite(cvs, spr, cvs->context.last_elapsed_time);
	}

	return result;
}

static s32 _collide_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	Canvas* cvs = (Canvas*)spr->owner;
	if(spr->collide) {
		spr->collide(cvs, spr, cvs->context.last_elapsed_time);
	} else {
		collide_sprite(cvs, spr, cvs->context.last_elapsed_time);
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
	Str fname = 0;
	union { Ptr ptr; s32 sint; } u;
	_spr->time_line.shape_file_name = copy_string(_shapeFile);
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
		_spr->time_line.frame_count = c;
		_spr->frame_size.w = w;
		_spr->frame_size.h = h;
		_spr->frame_rate = r;
		/* frames */
		_spr->time_line.frames = AGE_MALLOC_N(Frame, c);
		for(k = 0; k < c; ++k) {
			_spr->time_line.frames[k].parent = _spr;
			_spr->time_line.frames[k].tex = AGE_MALLOC_N(Pixel, w * h);
			for(j = 0; j < h; ++j) {
				freadln(fp, &bs);
				if(bs[0] == NAMED_FRAME_PREFIX) {
					fname = copy_substring(bs, 1, 0);
					u.sint = k;
					ht_set_or_insert(_spr->time_line.named_frames, fname, u.ptr);
					freadln(fp, &bs);
				}
				for(i = 0; i < w; ++i) {
					_spr->time_line.frames[k].tex[i + j * w].parent = &_spr->time_line.frames[k];
					_spr->time_line.frames[k].tex[i + j * w].shape = bs[i];
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
	_spr->time_line.brush_file_name = copy_string(_brushFile);
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
		assert(_spr->time_line.frame_count == c);
		assert(_spr->frame_size.w == w);
		assert(_spr->frame_size.h == h);
		/* frames */
		for(k = 0; k < c; ++k) {
			for(j = 0; j < h; ++j) {
				freadln(fp, &bs);
				for(i = 0; i < w; ++i) {
					_spr->time_line.frames[k].tex[i + j * w].brush = bs[i];
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
	_spr->time_line.palete_file_name = copy_string(_paleteFile);
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
		for(k = 0; k < _spr->time_line.frame_count; ++k) {
			for(j = 0; j < _spr->frame_size.h; ++j) {
				for(i = 0; i < _spr->frame_size.w; ++i) {
					b = (s32)_spr->time_line.frames[k].tex[i + j * _spr->frame_size.w].brush;
					_spr->time_line.frames[k].tex[i + j * _spr->frame_size.w].color = palete[b];
				}
			}
		}
	} else {
		result = FALSE;
	}

	return result;
}

static bl _try_fill_pixel_collision(Pixel* _pixelc, Pixel* _pixelf, s32 _px, s32 _py) {
	bl result = FALSE;
	Sprite* _sprf = 0;
	Sprite* _sprc = 0;
	u32 _pm = PHYSICS_MODE_NULL;
	s32 i = 0;

	assert(_pixelc && _pixelf);

	_sprf = _pixelf->parent->parent;
	_pm = get_sprite_physics_mode(_sprf->owner, _sprf);

	/* check */
	if((_pm & PHYSICS_MODE_CHECKER) != PHYSICS_MODE_NULL) {
		if(_pixelc->frame_count != 0) {
			if(_sprf->collided) {
				_sprf->collided(_sprf->owner, _sprf, _sprf->position.x + _px, _sprf->position.y + _py);
			}
		}
	}
	/* fill */
	if((_pm & PHYSICS_MODE_OBSTACLE) != PHYSICS_MODE_NULL) {
		if(_pixelc->frame_count < MAX_CACHED_FRAME_COUNT) { /* fill */
			_pixelc->owner_frames[
				_pixelc->frame_count++
			] = _pixelf->parent;
			result = TRUE;
		}
		for(i = 0; i < _pixelc->frame_count; ++i) { /* check */
			_sprc = _pixelc->owner_frames[i]->parent;
			if(_sprf != _sprc && _sprc->collided) {
				_sprc->collided(_sprc->owner, _sprc, _sprf->position.x + _px, _sprf->position.y + _py);
			}
		}
	}

	return result;
}

Canvas* create_canvas(const Str _name) {
	s32 count = CANVAS_WIDTH * CANVAS_HEIGHT;
	Str name = copy_string(_name);
	Canvas* result = AGE_MALLOC(Canvas);

	result->name = name;
	result->params = create_paramset();
	result->size.w = CANVAS_WIDTH;
	result->size.h = CANVAS_HEIGHT;
	result->pixels = AGE_MALLOC_N(Pixel, count);
	result->sprites = ht_create(0, ht_cmp_string, ht_hash_string, 0);
	result->context.last_color = ERASE_PIXEL_COLOR;
	create_canvas_message_map(result);

	return result;
}

void destroy_canvas(Canvas* _cvs) {
	s32 i = 0;
	Sprite* spr = 0;

	destroy_canvas_message_map(_cvs);
	destroy_paramset(_cvs->params);
	destroy_all_sprites(_cvs);

	for(i = 0; i < _cvs->dropped_sprites_count; ++i) {
		spr = _cvs->dropped_sprites[i];
		_destroy_sprite_impl(_cvs, spr);
	}
	AGE_FREE_N(_cvs->dropped_sprites);
	_cvs->dropped_sprites_size = 0;
	_cvs->dropped_sprites_count = 0;

	ht_destroy(_cvs->sprites);

	AGE_FREE_N(_cvs->pixels);
	AGE_FREE(_cvs->name);
	AGE_FREE(_cvs);
}

void set_frame_rate(Canvas* _cvs, s32 _rate) {
	assert(_cvs && _rate > 0);

	_cvs->frame_rate = _rate;
}

s32 get_frame_rate(Canvas* _cvs) {
	assert(_cvs);

	return _cvs->frame_rate;
}

void collide_canvas(Canvas* _cvs, s32 _elapsedTime) {
	ht_foreach(_cvs->sprites, _collide_sprite);
}

void update_canvas(Canvas* _cvs, s32 _elapsedTime) {
	control_proc ctrl = 0;

	_cvs->context.last_elapsed_time = _elapsedTime;
	_cvs->context.last_lparam = 0;
	_cvs->context.last_wparam = 0;
	_cvs->context.last_extra = 0;

	ctrl = get_canvas_controller(_cvs);
	if(ctrl) {
		ctrl(_cvs, _cvs->name, _elapsedTime, 0, 0, 0);
	}

	ht_foreach(_cvs->sprites, _update_sprite);
}

void tidy_canvas(Canvas* _cvs, s32 _elapsedTime) {
	Sprite* spr = 0;
	s32 i = 0;

	for(i = 0; i < _cvs->dropped_sprites_count; ++i) {
		spr = _cvs->dropped_sprites[i];
		_destroy_sprite_impl(_cvs, spr);
	}
	if(_cvs->dropped_sprites_size - _cvs->dropped_sprites_count > 16) {
		AGE_FREE_N(_cvs->dropped_sprites);
		_cvs->dropped_sprites_size = 0;
	}
	_cvs->dropped_sprites_count = 0;
}

void render_canvas(Canvas* _cvs, s32 _elapsedTime) {
	s32 x = 0;
	s32 y = 0;
	Pixel* pixelc = 0;

	/* fill frame buffer */
	if(_cvs->prev_render) {
		_cvs->prev_render(_cvs, _elapsedTime);
	}
	ht_foreach(_cvs->sprites, _fire_render_sprite);
	{
		s32 i = 0;
		s32 j = 0;
		for(i = 0; i < _cvs->size.w; ++i) {
			for(j = 0; j < _cvs->size.h; ++j) {
				Pixel* pc = &_cvs->pixels[i + j * _cvs->size.w];
				pc->frame_count = 0;
			}
		}
	}
	ht_foreach(_cvs->sprites, _post_render_sprite);
	if(_cvs->post_render) {
		_cvs->post_render(_cvs, _elapsedTime);
	}

	/* render frame buffer to target */
	for(y = 0; y < _cvs->size.h; ++y) {
		for(x = 0; x < _cvs->size.w; ++x) {
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			if(pixelc->color == ERASE_PIXEL_COLOR) {
				set_color(_cvs, get_mapped_color(0));
				goto_xy(_cvs, x, y);
				putch(ERASE_PIXEL_SHAPE);
				pixelc->color = 0;
			} else if(pixelc->shape) {
				set_color(_cvs, pixelc->color);
				goto_xy(_cvs, x, y);
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
		result->visibility = VISIBILITY_VISIBLE;
		result->params = create_paramset();
		result->owner = _cvs;
		result->time_line.named_frames = ht_create(0, ht_cmp_string, ht_hash_string, _destroy_string);

		_create_sprite_shape(_cvs, result, _shapeFile);
		_create_sprite_brush(_cvs, result, _brushFile);
		_create_sprite_palete(_cvs, result, _paleteFile);

		create_sprite_message_map(result);

		sprites = _cvs->sprites;
		ht_set_or_insert(sprites, result->name, result);
	}

	return result;
}

Sprite* clone_sprite(Canvas* _cvs, const Str _srcName, const Str _tgtName) {
	Sprite* result = 0;
	Sprite* src = 0;

	src = get_sprite_by_name(_cvs, _srcName);
	result = get_sprite_by_name(_cvs, _tgtName);
	assert(src && !result);
	if(src && !result) {
		/* TODO: copy from memory, not from file */
		result = create_sprite(
			_cvs,
			_tgtName,
			src->time_line.shape_file_name,
			src->time_line.brush_file_name,
			src->time_line.palete_file_name
		);
		result->physics_mode = src->physics_mode;
		result->collided = src->collided;
		result->control = src->control;
		result->object_removed = src->object_removed;
		result->update = src->update;
		result->prev_render = src->prev_render;
		result->post_render = src->post_render;
		copy_message_map(&src->message_map, &result->message_map);
	} else {
		result = 0;
	}

	return result;
}

void destroy_sprite(Canvas* _cvs, Sprite* _spr) {
	ls_node_t* spr = 0;

	assert(_cvs);

	spr = ht_find(_cvs->sprites, _spr->name);
	if(spr) {
		tobeRemoved = (Sprite*)spr->data;
		ht_foreach(_cvs->sprites, _on_removing_sprite);
		tobeRemoved = 0;

		ht_remove(_cvs->sprites, spr->extra);
		_drop_sprite(_cvs, _spr);
	}
}

void destroy_all_sprites(Canvas* _cvs) {
	assert(_cvs);

	ht_foreach(_cvs->sprites, _destroy_sprite);
	ht_clear(_cvs->sprites);
}

Color get_sprite_pixel_color(Canvas* _cvs, Sprite* _spr, s32 _frame, s32 _x, s32 _y) {
	Color result = ERASE_PIXEL_COLOR;
	Frame* frame = 0;

	assert(_cvs && _spr);

	if(_frame < 0 || _frame > _spr->time_line.frame_count - 1) {
		goto _exit;
	}
	frame = &_spr->time_line.frames[_frame];

	if(_x < 0 || _x > _spr->frame_size.w - 1 || _y < 0 || _y > _spr->frame_size.h - 1) {
		goto _exit;
	}

	result = frame->tex[_x + _y * _spr->frame_size.w].color;

_exit:
	return result;
}

void set_sprite_pixel_color(Canvas* _cvs, Sprite* _spr, s32 _frame, s32 _x, s32 _y, Color _col) {
	Frame* frame = 0;

	assert(_cvs && _spr);

	if(_frame < 0 || _frame > _spr->time_line.frame_count - 1) {
		goto _exit;
	}
	frame = &_spr->time_line.frames[_frame];

	if(_x < 0 || _x > _spr->frame_size.w - 1 || _y < 0 || _y > _spr->frame_size.h - 1) {
		goto _exit;
	}

	frame->tex[_x + _y * _spr->frame_size.w].color = _col;

_exit:
	return;
}

bl set_sprite_visible(Canvas* _cvs, Sprite* _spr, bl _vis) {
	bl result = TRUE;

	if(_vis) {
		_spr->visibility = VISIBILITY_VISIBLE;
	} else {
		if(_spr->visibility == VISIBILITY_VISIBLE) {
			_spr->visibility = VISIBILITY_DISAPPEARING;
		}
	}

	return result;
}

bl get_sprite_visible(Canvas* _cvs, Sprite* _spr, bl* _vis) {
	bl result = TRUE;

	if(_vis) {
		*_vis = _spr->visibility == VISIBILITY_VISIBLE;
	} else {
		result = FALSE;
	}

	return result;
}

bl set_sprite_position(Canvas* _cvs, Sprite* _spr, s32 _x, s32 _y) {
	bl result = TRUE;

	_spr->position.x = _x;
	_spr->position.y = _y;

	return result;
}

bl get_sprite_position(Canvas* _cvs, Sprite* _spr, s32* _x, s32* _y) {
	bl result = TRUE;

	if(_x) {
		*_x = _spr->position.x;
	}
	if(_y) {
		*_y = _spr->position.y;
	}

	return result;
}

s32 get_named_frame_index(Canvas* _cvs, Sprite* _spr, const Str _name) {
	s32 result = INVALID_FRAME_INDEX;
	ls_node_t* n = 0;
	union { Ptr ptr; s32 sint; } u;

	n = ht_find(_spr->time_line.named_frames, _name);
	if(n) {
		u.ptr = n->data;
		result = u.sint;
	}

	return result;
}

bl play_sprite(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, bl _loop, sprite_playing_callback_func _cb) {
	bl result = TRUE;

	if(_spr->time_line.begin_name) {
		AGE_FREE(_spr->time_line.begin_name);
	}
	if(_spr->time_line.end_name) {
		AGE_FREE(_spr->time_line.end_name);
	}
	_spr->time_line.begin_name = copy_string(_begin);
	_spr->time_line.end_name = copy_string(_end);
	_spr->time_line.begin_index = get_named_frame_index(_cvs, _spr, _begin);
	_spr->time_line.end_index = get_named_frame_index(_cvs, _spr, _end);
	_spr->time_line.loop = _loop;
	_spr->time_line.callback = _cb;
	_spr->time_line.current_frame = _spr->time_line.begin_index;

	return result;
}

bl pause_sprite(Canvas* _cvs, Sprite* _spr) {
	bl result = TRUE;

	assert(_cvs && _spr);

	_spr->time_line.pause = TRUE;

	return result;
}

bl resume_sprite(Canvas* _cvs, Sprite* _spr) {
	bl result = TRUE;

	assert(_cvs && _spr);

	_spr->time_line.pause = FALSE;

	return result;
}

bl stop_sprite(Canvas* _cvs, Sprite* _spr, s32 _stopAt) {
	bl result = TRUE;

	if((_stopAt == INVALID_FRAME_INDEX) || (_stopAt >= 0 && _stopAt < _spr->time_line.frame_count)) {
		if(_stopAt != INVALID_FRAME_INDEX) {
			_spr->time_line.current_frame = _stopAt;
		}
		if(_spr->time_line.begin_name) {
			AGE_FREE(_spr->time_line.begin_name);
		}
		if(_spr->time_line.end_name) {
			AGE_FREE(_spr->time_line.end_name);
		}
		_spr->time_line.begin_index = INVALID_FRAME_INDEX;
		_spr->time_line.end_index = INVALID_FRAME_INDEX;
	} else {
		result = FALSE;
	}

	return result;
}

void update_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	s32 tickableTime = 0;

	assert(_cvs && _spr);
	if(!_spr->time_line.pause) {
		tickableTime = (s32)(_cvs->frame_rate * _spr->frame_rate);
		_spr->frame_tick += _elapsedTime;
		if(_spr->frame_tick >= tickableTime) {
			_spr->frame_tick -= tickableTime;
			++_spr->time_line.current_frame;
			if(_spr->time_line.begin_name && _spr->time_line.end_name) {
				if(_spr->time_line.current_frame >= _spr->time_line.end_index) {
					if(_spr->time_line.callback) {
						_spr->time_line.callback(_cvs, _spr, _spr->time_line.begin_name, _spr->time_line.end_name, _spr->time_line.current_frame);
					}
					if(_spr->time_line.loop) {
						_spr->time_line.current_frame = _spr->time_line.begin_index;
					} else {
						--_spr->time_line.current_frame;
					}
				}
			} else {
				if(_spr->time_line.current_frame >= _spr->time_line.frame_count) {
					_spr->time_line.current_frame = 0;
				}
			}
		}
	}
}

void prev_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
	s32 x = 0;
	s32 y = 0;
	s32 s = 0;
	s32 itf = 0;
	s32 found = INVALID_FRAME_INDEX;
	Pixel* pixelf = 0;
	Pixel* pixelc = 0;

	if(_spr->visibility == VISIBILITY_HIDEN) {
		return;
	}
	if(_spr->visibility == VISIBILITY_DISAPPEARING) {
		_spr->visibility = VISIBILITY_HIDEN;
	}
	k = _spr->time_line.last_frame;
	for(j = 0; j < _spr->frame_size.h; ++j) {
		y = _spr->old_position.y + j;
		if(y < 0 || y >= _cvs->size.h) {
			continue;
		}
		for(i = 0; i < _spr->frame_size.w; ++i) {
			x = _spr->old_position.x + i;
			if(x < 0 || x >= _cvs->size.w) {
				continue;
			}
			pixelf = &_spr->time_line.frames[k].tex[i + j * _spr->frame_size.w];
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			s = (s32)pixelf->shape;
			if(s) {
				pixelc->shape = 0;
				pixelc->color = ERASE_PIXEL_COLOR;
				pixelc->zorder = DEFAULT_Z_ORDER;
				found = INVALID_FRAME_INDEX;
				for(itf = 0; itf < pixelc->frame_count; ++itf) {
					if(pixelc->owner_frames[itf] == pixelf->parent) {
						found = itf;
						pixelc->owner_frames[found] =
							pixelc->owner_frames[
								--pixelc->frame_count
							];
					}
				}
			}
		}
	}
	_spr->last_frame_position = _spr->old_position;
	_spr->old_position = _spr->position;
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

	if(_spr->visibility != VISIBILITY_VISIBLE) {
		return;
	}
	_spr->time_line.last_frame = _spr->time_line.current_frame;
	k = _spr->time_line.current_frame;
	for(j = 0; j < _spr->frame_size.h; ++j) {
		y = _spr->position.y + j;
		if(y < 0 || y >= _cvs->size.h) {
			continue;
		}
		for(i = 0; i < _spr->frame_size.w; ++i) {
			x = _spr->position.x + i;
			if(x < 0 || x >= _cvs->size.w) {
				continue;
			}
			pixelf = &_spr->time_line.frames[k].tex[i + j * _spr->frame_size.w];
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			s = (s32)pixelf->shape;
			if(s != ERASE_PIXEL_SHAPE) {
				if(pixelf->zorder <= pixelc->zorder) {
					pixelc->shape = (s8)s;
					pixelc->color = pixelf->color;
					pixelc->zorder = pixelf->zorder;
				}
			}
		}
	}
}

void collide_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	s32 i = 0;
	s32 j = 0;
	s32 k = 0;
	s32 x = 0;
	s32 y = 0;
	s32 s = 0;
	Pixel* pixelf = 0;
	Pixel* pixelc = 0;

	if(_spr->visibility != VISIBILITY_VISIBLE) {
		return;
	}
	_spr->time_line.last_frame = _spr->time_line.current_frame;
	k = _spr->time_line.current_frame;
	for(j = 0; j < _spr->frame_size.h; ++j) {
		y = _spr->position.y + j;
		if(y < 0 || y >= _cvs->size.h) {
			continue;
		}
		for(i = 0; i < _spr->frame_size.w; ++i) {
			x = _spr->position.x + i;
			if(x < 0 || x >= _cvs->size.w) {
				continue;
			}
			pixelf = &_spr->time_line.frames[k].tex[i + j * _spr->frame_size.w];
			pixelc = &_cvs->pixels[x + y * _cvs->size.w];
			s = (s32)pixelf->shape;
			if(s != ERASE_PIXEL_SHAPE) {
				_try_fill_pixel_collision(pixelc, pixelf, i, j);
			} else if(pixelf->brush != ERASE_PIXEL_SHAPE) {
				_try_fill_pixel_collision(pixelc, pixelf, i, j);
			}
		}
	}
}

u32 get_sprite_physics_mode(Canvas* _cvs, Sprite* _spr) {
	u32 result = PHYSICS_MODE_NULL;

	result = _spr->physics_mode;

	return result;
}

void set_sprite_physics_mode(Canvas* _cvs, Sprite* _spr, u32 _mode) {
	_spr->physics_mode = _mode;
}

void draw_string(Canvas* _cvs, Font* _font, s32 _x, s32 _y, const Str _text, ...) {
	s8 buf[AGE_TXT_LEN];
	Str pbuf = buf;
	va_list argptr;
	goto_xy(_cvs, _x, _y);
	if(_font) {
		set_color(_cvs, _font->color);
	} else {
		set_color(_cvs, get_mapped_color(15));
	}
	va_start(argptr, _text);
	vsprintf(pbuf, _text, argptr);
	va_end(argptr);
	printf(buf);
}

void put_char(Canvas* _cvs, Font* _font, s32 _x, s32 _y, s8 _ch) {
	goto_xy(_cvs, _x, _y);
	if(_font) {
		set_color(_cvs, _font->color);
	} else {
		set_color(_cvs, get_mapped_color(15));
	}
	putch(_ch);
}

Color get_mapped_color(s32 _index) {
	Color result = ERASE_PIXEL_COLOR;

	assert(_index >= 0 && _index < sizeof(COLOR_MAP));

	result = COLOR_MAP[_index];

	return result;
}

void set_cursor_visible(Canvas* _cvs, bl _vis) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = _vis;
	SetConsoleCursorInfo(hOut, &cci);
}

void goto_xy(Canvas* _cvs, s32 _x, s32 _y) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = _x;
	pos.Y = _y;
	SetConsoleCursorPosition(hOut, pos);
}

void set_color(Canvas* _cvs, Color _col) {
	HANDLE hConsole;
	if(_col != _cvs->context.last_color) {
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, get_mapped_color(_col));
		_cvs->context.last_color = _col;
	}
}

void clear_pixel(Canvas* _cvs, s32 _x, s32 _y) {
	Pixel* pixelc = 0;

	pixelc = &_cvs->pixels[_x + _y * _cvs->size.w];

	set_color(_cvs, get_mapped_color(0));
	goto_xy(_cvs, _x, _y);
	putch(ERASE_PIXEL_SHAPE);
	pixelc->shape = 0;
	pixelc->brush = ERASE_PIXEL_SHAPE;
	pixelc->color = 0;
	pixelc->frame_count = 0;
	pixelc->parent = 0;
	pixelc->zorder = DEFAULT_Z_ORDER;
}

void clear_screen(Canvas* _cvs) {
	s32 x = 0;
	s32 y = 0;

	for(y = 0; y < _cvs->size.h; ++y) {
		for(x = 0; x < _cvs->size.w; ++x) {
			clear_pixel(_cvs, x, y);
		}
	}

	system("cls");
}

#endif /* AGE_IMPL == AGE_IMPL_CONSOLE */
