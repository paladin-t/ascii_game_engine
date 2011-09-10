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

#include "script.h"
#include "game.h"

static int _set_foot_brush(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	Str b = 0;

	assert(s && l);

	amb_attempt_open_bracket(s, l);
	amb_pop_string(s, l, &b);
	amb_attempt_close_bracket(s, l);

	game()->footBrush = b[0];

	return result;
}

static int _set_audio_string(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	s32 t = 0;
	Str b = 0;

	assert(s && l);

	amb_attempt_open_bracket(s, l);
	amb_pop_int(s, l, &t);
	amb_pop_string(s, l, &b);
	amb_attempt_close_bracket(s, l);

	assert(t >= 0 && t < AHAT_COUNT);
	if(t >= 0 && t < AHAT_COUNT) {
		game()->audio[t] = AGE_REALLOC(s8, game()->audio[t], strlen(b) + 1);
		strcpy(game()->audio[t], b);
	}

	return result;
}

void register_game_script_interfaces(void) {
	amb_register_func(get_world()->script, "SET_FOOT_BRUSH", _set_foot_brush);
	amb_register_func(get_world()->script, "SET_AUDIO_STRING", _set_audio_string);
}
