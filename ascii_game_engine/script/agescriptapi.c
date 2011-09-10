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

#include "../common/ageutil.h"
#include "../input/ageinput.h"
#include "../render/agerenderer.h"
#include "../age.h"
#include "agescriptapi.h"

static FILE* dataFile = 0;

static s32 _save_cvs_param(Ptr _data, Ptr _extra) {
	s32 result = 0;
	Str name = 0;
	AgeParam* par = 0;
	s8 buf[AGE_STR_LEN];

	par = (AgeParam*)_data;
	name = (Str)_extra;

	switch(par->type) {
		case APT_S32:
			sprintf(buf, "SET_CVS_S32_PARAM(\"%s\", %d)\r\n", name, par->s32);
			break;
		case APT_U32:
			sprintf(buf, "SET_CVS_U32_PARAM(\"%s\", %d)\r\n", name, par->u32);
			break;
		case APT_F32:
			sprintf(buf, "SET_CVS_F32_PARAM(\"%s\", %f)\r\n", name, par->f32);
			break;
		case APT_STR:
			sprintf(buf, "SET_CVS_STR_PARAM(\"%s\", %s)\r\n", name, par->str);
			break;
		default:
			assert(0 && "Unknown parameter type");
			break;
	}

	fwrite(buf, 1, strlen(buf), dataFile);

	return result;
}

bl amb_register_func(mb_interpreter_t* s, const Str n, mb_func_t f) {
	bl result = TRUE;

	result = !!mb_register_func(s, n, f);

	return result;
}

bl amb_unregister_func(mb_interpreter_t* s, const Str n) {
	bl result = TRUE;

	result = !!mb_remove_func(s, n);

	return result;
}

int amb_attempt_func_begin(mb_interpreter_t* s, void** l) {
	return mb_attempt_func_begin(s, l);
}

int amb_attempt_open_bracket(mb_interpreter_t* s, void** l) {
	return mb_attempt_open_bracket(s, l);
}

int amb_attempt_close_bracket(mb_interpreter_t* s, void** l) {
	return mb_attempt_close_bracket(s, l);
}

int amb_pop_int(mb_interpreter_t* s, void** l, int_t* val) {
	return mb_pop_int(s, l, val);
}

int amb_pop_real(mb_interpreter_t* s, void** l, real_t* val) {
	return mb_pop_real(s, l, val);
}

int amb_pop_string(mb_interpreter_t* s, void** l, char** val) {
	return mb_pop_string(s, l, val);
}

int amb_pop_value(mb_interpreter_t* s, void** l, mb_value_t* val) {
	return mb_pop_value(s, l, val);
}

int amb_push_int(mb_interpreter_t* s, void** l, int_t val) {
	return mb_push_int(s, l, val);
}

int amb_push_real(mb_interpreter_t* s, void** l, real_t val) {
	return mb_push_real(s, l, val);
}

int amb_push_string(mb_interpreter_t* s, void** l, char* val) {
	return mb_push_string(s, l, val);
}

int amb_push_value(mb_interpreter_t* s, void** l, mb_value_t val) {
	return mb_push_value(s, l, val);
}

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

int age_api_get_cvs_s32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	s32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	get_s32_param(AGE_CVS_PAR, name, &val);
	mb_push_int(s, l, val);

	return result;
}

int age_api_set_cvs_s32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	s32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_int(s, l, &val);
	mb_attempt_close_bracket(s, l);

	set_s32_param(AGE_CVS_PAR, name, val);

	return result;
}

int age_api_get_cvs_u32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	u32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	get_u32_param(AGE_CVS_PAR, name, &val);
	mb_push_int(s, l, (int)val);

	return result;
}

int age_api_set_cvs_u32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	s32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_int(s, l, &val);
	mb_attempt_close_bracket(s, l);

	set_u32_param(AGE_CVS_PAR, name, (u32)val);

	return result;
}

int age_api_get_cvs_f32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	f32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	get_f32_param(AGE_CVS_PAR, name, &val);
	mb_push_real(s, l, val);

	return result;
}

int age_api_set_cvs_f32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	f32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_real(s, l, &val);
	mb_attempt_close_bracket(s, l);

	set_f32_param(AGE_CVS_PAR, name, val);

	return result;
}

int age_api_get_cvs_str_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Str val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	get_str_param(AGE_CVS_PAR, name, &val);
	mb_push_string(s, l, val);

	return result;
}

int age_api_set_cvs_str_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str name = 0;
	Str val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &name);
	mb_pop_string(s, l, &val);
	mb_attempt_close_bracket(s, l);

	set_str_param(AGE_CVS_PAR, name, val);

	return result;
}

int age_api_get_spr_s32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	s32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	get_s32_param(spr->params, name, &val);
	mb_push_int(s, l, val);

	return result;
}

int age_api_set_spr_s32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	s32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_pop_int(s, l, &val);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	set_s32_param(spr->params, name, val);

	return result;
}

int age_api_get_spr_u32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	u32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	get_u32_param(spr->params, name, &val);
	mb_push_int(s, l, (int)val);

	return result;
}

int age_api_set_spr_u32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	s32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_pop_int(s, l, &val);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	set_u32_param(spr->params, name, (u32)val);

	return result;
}

int age_api_get_spr_f32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	f32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	get_f32_param(spr->params, name, &val);
	mb_push_real(s, l, val);

	return result;
}

int age_api_set_spr_f32_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	f32 val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_pop_real(s, l, &val);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	set_f32_param(spr->params, name, val);

	return result;
}

int age_api_get_spr_str_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	Str val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	get_str_param(spr->params, name, &val);
	mb_push_string(s, l, val);

	return result;
}

int age_api_set_spr_str_param(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str sn = 0;
	Sprite* spr = 0;
	Str name = 0;
	Str val = 0;

	assert(s && l);

	mb_attempt_open_bracket(s, l);
	mb_pop_string(s, l, &sn);
	mb_pop_string(s, l, &name);
	mb_pop_string(s, l, &val);
	mb_attempt_close_bracket(s, l);

	spr = get_sprite_by_name(AGE_CVS, sn);
	assert(spr);
	set_str_param(spr->params, name, val);

	return result;
}

void amb_load_data(const Str file) {
	run_new_script(file);
}

void amb_save_data(const Str file) {
	dataFile = fopen(file, "wb+");
	if(AGE_CVS->storeParams && dataFile) {
		ht_foreach(AGE_CVS->params, _save_cvs_param);
	}
	if(dataFile) {
		fclose(dataFile);
		dataFile = 0;
	}
}
