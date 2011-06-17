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

#include <stdio.h>

#include "age.h"

#define _VER_MAJOR 1
#define _VER_MINOR 0
#define _VER_REVISION 1
#define _AGE_VERSION ((_VER_MAJOR << 24) | (_VER_MINOR << 16) | (_VER_REVISION))
#define _AGE_VERSION_STRING "1.0.0001"

static World* _gWorld = 0;

static void _on_error(mb_interpreter_t* s, mb_error_e e, char* m, int p);
static bl _open_script(mb_interpreter_t** s);
static bl _close_script(mb_interpreter_t** s);

void _on_error(mb_interpreter_t* s, mb_error_e e, char* m, int p) {
	if(SE_NO_ERR != e) {
		printf("Error : [POS] %d, [CODE] %d, [MESSAGE] %s\n", p, e, m);
	}
}

bl _open_script(mb_interpreter_t** s) {
	bl result = TRUE;

	mb_open(s);
	mb_set_error_handler(*s, _on_error);
	mb_register_func(*s, "BEEP", age_api_beep);
	mb_register_func(*s, "REG_KEY", age_api_reg_key_code);

	return result;
}

bl _close_script(mb_interpreter_t** s) {
	bl result = TRUE;

	mb_close(s);

	return result;
}

u32 get_ver(void) {
	return _AGE_VERSION;
}

const s8* get_ver_string(void) {
	return _AGE_VERSION_STRING;
}

World* create_world(void) {
	World* result = 0;

	assert(!_gWorld);

	result = AGE_MALLOC(World);
	result->canvas = create_canvas();

	mb_init();
	_open_script(&result->script);

	_gWorld = result;

	return result;
}

World* get_world(void) {
	return _gWorld;
}

void destroy_world(World* _wld) {
	assert(_gWorld);

	_close_script(&_wld->script);
	mb_dispose();

	destroy_canvas(_wld->canvas);
	AGE_FREE(_wld);

	_gWorld = 0;
}

bl config_world(const s8* _cfgFile) {
	bl result = TRUE;

	assert(_gWorld);

	mb_load_file(_gWorld->script, _cfgFile);
	mb_run(_gWorld->script);

	return result;
}

bl run_world_script(const s8* _sptFile) {
	bl result = TRUE;

	assert(_gWorld);

	mb_load_file(_gWorld->script, _sptFile);
	mb_run(_gWorld->script);

	return result;
}

bl run_new_script(const s8* _sptFile) {
	bl result = TRUE;
	mb_interpreter_t* script = 0;

	_open_script(&script);
	mb_load_file(script, _sptFile);
	mb_run(script);
	_close_script(&script);

	return result;
}
