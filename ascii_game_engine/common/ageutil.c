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

#include "ageallocator.h"
#include "ageutil.h"

#pragma comment(lib, "winmm.lib")

void sys_beep(void) {
	putchar('\a');
}

u32 sys_tick_count(void) {
	return timeGetTime();
}

void sys_sleep(s32 _time) {
	Sleep(_time);
}

s32 freadln(FILE* _fp, Str* _buf) {
	s32 result = 0;
	s8 ch = 0;
	long ft = 0;

	assert(_fp && _buf);

	ch = (s8)fgetc(_fp);
	while(ch != EOF && ch != '\r' && ch != '\n') {
		if(ch != EOF) {
			(*_buf)[result++] = ch;
		}
		ch = (s8)fgetc(_fp);
	}
	(*_buf)[result] = '\0';

	if(ch != EOF) {
		ft = ftell(_fp);
		ch = (s8)fgetc(_fp);
		if(ch != '\n') {
			fseek(_fp, ft, 0);
		}
	}

	return result;
}

s32 fskipln(FILE* _fp) {
	s32 result = 0;
	s8 buf[AGE_STR_LEN];
	Str str = buf;
	result = freadln(_fp, &str);

	return result;
}

Str copy_string(const Str _str) {
	Str result = 0;
	s32 l = (s32)strlen(_str);

	assert(_str);

	result = (Str)age_malloc(l + 1);
	strcpy(result, _str);

	return result;
}

Str copy_substring(const Str _str, s32 _start, s32 _count) {
	Str result = 0;
	s32 l = (s32)strlen(_str);
	s32 i = 0;

	assert(_str);

	if(!_count) {
		_count = l - _start;
	}
	result = (Str)age_malloc(l + 1);
	for(i = 0; i < l && i < _count; ++i) {
		result[i] = _str[_start + i];
	}
	result[i] = '\0';

	return result;
}

s32 age_cmp_ptr(const Ptr _left, const Ptr _right) {
	s32 result = 0;
	if(_left > _right) {
		result = 1;
	} else if(_left < _right) {
		result = -1;
	}

	return result;
}
