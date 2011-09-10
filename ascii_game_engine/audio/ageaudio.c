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

#include <math.h>

#include "../common/ageutil.h"
#include "ageaudio.h"

static u32 _FREQ[3][7] = {
	{ 262,  294,  330,  349,  392,  440,  494 },
	{ 523,  578,  659,  698,  784,  880,  988 },
	{ 1046, 1175, 1318, 1397, 1568, 1760, 1976 }
};

typedef struct ThreadInfo {
	DWORD threadId;
	HANDLE threadHandle;
	CRITICAL_SECTION lock;
	s32 level;
	bl loop;
} ThreadInfo;

static void _lock(Ptr _info) {
	ThreadInfo* info = (ThreadInfo*)_info;
	EnterCriticalSection(&(info->lock));
}

static void _unlock(Ptr _info) {
	ThreadInfo* info = (ThreadInfo*)_info;
	LeaveCriticalSection(&(info->lock));
}

static bl _is_note(s8 _ch) {
	return ((_ch >= 'A' && _ch <= 'G') || _ch == 'P');
}

static void _age_beep(s32 _level, s8 _note) {
	if(_note == 'P') {
		age_sleep(100);
	} else {
		if(_note < 'C') {
			_note -= 'A';
			_note += 5;
		} else {
			_note -= 'C';
		}
		Beep(_FREQ[_level][_note], 100);
	}
}

#define __PLAY_ONE_NOTE(__c) \
	if(_is_note(note) || _is_note(last)) { \
		_age_beep(info->level, note ? note : last); \
		note = __c; \
	}
static s32 WINAPI sound_proc(Ptr param) {
	DWORD result = 0;
	SoundContext* cnt = (SoundContext*)param;
	s32 len = strlen(cnt->sequence);
	ThreadInfo* info = 0;
	s8 last = '\0';
	s8 ch = '\0';
	s8 note = '\0';

	if(cnt->bgm) {
		info = (ThreadInfo*)(cnt->bgm);
	} else if(cnt->sfx) {
		info = (ThreadInfo*)(cnt->sfx);
	} else {
		return result;
	}

_again:
	while(cnt->position < len) {
		ch = cnt->sequence[cnt->position++];
		if(ch == '>') {
			__PLAY_ONE_NOTE('\0');
			if(info->level < 2) {
				++info->level;
			}
		} else if(ch == '<') {
			__PLAY_ONE_NOTE('\0');
			if(info->level > 0) {
				--info->level;
			}
		} else if(_is_note(ch) || ch == '\0') {
			__PLAY_ONE_NOTE(ch);
		}
		last = ch;
		age_sleep(80);
	}
	if(info->loop) {
		cnt->position = 0;
		goto _again;
	}

	return result;
}

#undef __PLAY_ONE_NOTE

SoundContext* create_sound_context(void) {
	SoundContext* result = AGE_MALLOC(SoundContext);
	result->bgm = AGE_MALLOC(ThreadInfo);
	result->sfx = AGE_MALLOC(ThreadInfo);
	((ThreadInfo*)(result->bgm))->level = 1;
	((ThreadInfo*)(result->sfx))->level = 1;
	InitializeCriticalSection(&((ThreadInfo*)(result->bgm))->lock);
	InitializeCriticalSection(&((ThreadInfo*)(result->sfx))->lock);

	return result;
}

void destroy_sound_context(SoundContext* _cnt) {
	stop_sound(_cnt, ST_BGM);
	stop_sound(_cnt, ST_SFX);
	DeleteCriticalSection(&((ThreadInfo*)(_cnt->bgm))->lock);
	DeleteCriticalSection(&((ThreadInfo*)(_cnt->sfx))->lock);
	AGE_FREE(_cnt->bgm);
	AGE_FREE(_cnt->sfx);
	if(_cnt->sequence) {
		AGE_FREE(_cnt->sequence);
	}
	AGE_FREE(_cnt);
}

void update_sound(SoundContext* _cnt, s32 _elapsedTime) {
	/* do nothing */
}

void play_sound_string(SoundContext* _cnt, const Str _seq, SoundType _type, bl _loop) {
	ThreadInfo* thread = 0;
	stop_sound(_cnt, _type);
	if(_type == ST_BGM) {
		thread = (ThreadInfo*)(_cnt->bgm);
	} else if(_type == ST_SFX) {
		thread = (ThreadInfo*)(_cnt->sfx);
	} else {
		assert(0 && "Unknown sound type");
	}
	_cnt->sequence = copy_string(_seq);
	thread->loop = _loop;
	thread->threadHandle = CreateThread(0, 0, sound_proc, _cnt, 0, &thread->threadId);
	SetThreadPriority(thread->threadHandle, THREAD_PRIORITY_HIGHEST);
	Sleep(1);
}

void stop_sound(SoundContext* _cnt, SoundType _type) {
	ThreadInfo* thread = 0;
	if(_type == ST_BGM) {
		thread = (ThreadInfo*)(_cnt->bgm);
	} else if(_type == ST_SFX) {
		thread = (ThreadInfo*)(_cnt->sfx);
	} else {
		assert(0 && "Unknown sound type");
	}
	if(thread) {
		thread->level = 1;
		if(thread->threadHandle) {
			TerminateThread(thread->threadHandle, 1);
			WaitForSingleObject(thread->threadHandle, 10);
			CloseHandle(thread->threadHandle);
			thread->threadHandle = 0;
			thread->threadId = 0;
			if(_cnt->sequence) {
				AGE_FREE(_cnt->sequence);
			}
			_cnt->position = 0;
		}
	}
}
