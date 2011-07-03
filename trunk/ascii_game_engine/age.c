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

#include "age.h"

#define _VER_MAJOR 1
#define _VER_MINOR 0
#define _VER_REVISION 1
#define _AGE_VERSION ((_VER_MAJOR << 24) | (_VER_MINOR << 16) | (_VER_REVISION))
#define _AGE_VERSION_STRING "1.0.0001"

static World* _gWorld = 0;

static void _on_error(mb_interpreter_t* s, mb_error_e e, char* m, int p) {
	if(SE_NO_ERR != e) {
		printf("Error : [POS] %d, [CODE] %d, [MESSAGE] %s\n", p, e, m);
	}
}

static bl _register_apis(mb_interpreter_t* s) {
	bl result = TRUE;

	mb_register_func(s, "BEEP", age_api_beep);
	mb_register_func(s, "REG_KEY", age_api_reg_key_code);
	mb_register_func(s, "SET_FRAME_RATE", age_api_set_frame_rate);

	return result;
}

static bl _open_script(mb_interpreter_t** s) {
	bl result = TRUE;

	mb_open(s);
	mb_set_error_handler(*s, _on_error);
	_register_apis(*s);

	return result;
}

static bl _close_script(mb_interpreter_t** s) {
	bl result = TRUE;

	mb_close(s);

	return result;
}

u32 get_ver(void) {
	return _AGE_VERSION;
}

const Str get_ver_string(void) {
	return _AGE_VERSION_STRING;
}

World* create_world(void) {
	World* result = 0;

	assert(!_gWorld);

	open_input();
	result = AGE_MALLOC(World);
	result->input = create_input_context();
	result->canvas = create_canvas(ST_DEFAULT_CANVAS_NAME);
	set_cursor_visible(FALSE);

	mb_init();
	_open_script(&result->script);

	_gWorld = result;

	return result;
}

World* get_world(void) {
	return _gWorld;
}

void destroy_world(void) {
	if(!_gWorld) {
		return;
	}

	_close_script(&_gWorld->script);
	mb_dispose();

	destroy_canvas(_gWorld->canvas);
	destroy_input_context(_gWorld->input);
	AGE_FREE(_gWorld);

	_gWorld = 0;

	close_input();
}

bl config_world(const Str _cfgFile) {
	bl result = TRUE;

	assert(_gWorld);

	mb_reset(&_gWorld->script, FALSE);
	mb_load_file(_gWorld->script, _cfgFile);
	mb_run(_gWorld->script);

	return result;
}

s32 run_world(void) {
	s32 result = 0;
	s32 now = 0;
	s32 old = get_tick_count();
	s32 elapsed = 0;
	s32 delay = 0;

	assert(_gWorld);

	_gWorld->running = TRUE;
	while(_gWorld->running) {
		now = get_tick_count();
		elapsed = now - old;
		old = now;
		update_canvas(AGE_CVS, elapsed);
		render_canvas(AGE_CVS, elapsed);
		delay = EXPECTED_FRAME_TIME - (get_tick_count() - old);
		if(delay > 0) {
			sys_sleep(delay);
		}
	}

	return result;
}

void exit_world(void) {
	assert(_gWorld);

	_gWorld->running = FALSE;
}

bl run_world_script(const Str _sptFile) {
	bl result = TRUE;

	assert(_gWorld);

	mb_reset(&_gWorld->script, FALSE);
	mb_load_file(_gWorld->script, _sptFile);
	mb_run(_gWorld->script);

	return result;
}

bl run_new_script(const Str _sptFile) {
	bl result = TRUE;
	mb_interpreter_t* script = 0;

	_open_script(&script);
	mb_load_file(script, _sptFile);
	mb_run(script);
	_close_script(&script);

	return result;
}
