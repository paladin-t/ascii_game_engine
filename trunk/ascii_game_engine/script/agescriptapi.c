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
#include <assert.h>

#include "agescriptapi.h"
#include "../input/ageinput.h"

int age_api_beep(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	assert(s && l);

	putchar('\a');

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
