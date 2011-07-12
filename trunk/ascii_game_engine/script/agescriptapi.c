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

#include "../common/ageutil.h"
#include "../input/ageinput.h"
#include "../render/agerenderer.h"
#include "../age.h"
#include "agescriptapi.h"

int age_api_beep(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	assert(s && l);

	age_beep();

	return result;
}

int age_api_reg_key_code(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	s32 _ply = 0;
	s32 _idx = 0;
	s32 _cod = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_int(s, l, &_ply);
	mb_pop_int(s, l, &_idx);
	mb_pop_int(s, l, &_cod);
	mb_attempt_close_bracket(s, l);

	register_key_map(_ply, _idx, _cod);

	return result;
}

int age_api_set_frame_rate(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	s32 rate = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_int(s, l, &rate);
	mb_attempt_close_bracket(s, l);

	set_frame_rate(AGE_CVS, rate);

	return result;
}

int age_api_create_sprite(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Str shapeFile = 0;
	Str brushFile = 0;
	Str paleteFile = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_string(s, l, &shapeFile);
	mb_pop_string(s, l, &brushFile);
	mb_pop_string(s, l, &paleteFile);
	mb_attempt_close_bracket(s, l);

	create_sprite(AGE_CVS, name, shapeFile, brushFile, paleteFile);

	return result;
}

int age_api_destroy_sprite(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Sprite* spr = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, name);
	if(spr) {
		destroy_sprite(AGE_CVS, spr);
	}

	return result;
}

int age_api_destroy_all_sprites(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_attempt_close_bracket(s, l);

	destroy_all_sprites(AGE_CVS);

	return result;
}

int age_api_set_sprite_position(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Sprite* spr = 0;
	s32 x = 0;
	s32 y = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_int(s, l, &x);
	mb_pop_int(s, l, &y);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, name);
	if(spr) {
		set_sprite_position(AGE_CVS, spr, x, y);
	}

	return result;
}

int age_api_get_sprite_position_x(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Sprite* spr = 0;
	s32 p = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, name);
	if(spr) {
		get_sprite_position(AGE_CVS, spr, &p, 0);
		mb_push_int(s, l, p);
	}

	return result;
}

int age_api_get_sprite_position_y(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Sprite* spr = 0;
	s32 p = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, name);
	if(spr) {
		get_sprite_position(AGE_CVS, spr, 0, &p);
		mb_push_int(s, l, p);
	}

	return result;
}

int age_api_play_sprite(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Str begin = 0;
	Str end = 0;
	s32 loop = 0;
	Sprite* spr = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_string(s, l, &begin);
	mb_pop_string(s, l, &end);
	mb_pop_int(s, l, &loop);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, name);
	if(spr) {
		play_sprite(AGE_CVS, spr, begin, end, loop != 0, 0);
	}

	return result;
}

int age_api_stop_sprite(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	s32 stopAt = 0;
	Sprite* spr = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_int(s, l, &stopAt);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, name);
	if(spr) {
		stop_sprite(AGE_CVS, spr, stopAt);
	}

	return result;
}
