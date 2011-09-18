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
	SoundContext* context;
	DWORD thread_id;
	HANDLE thread_handle;
	union {
		CRITICAL_SECTION lock;
		LPCRITICAL_SECTION plock;
	};
	s32 level;
	bl loop;
	bl stop;
	Str sequence;
	s32 position;
	s32 time;
	struct ThreadInfo* shadow;
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

static void _copy_sound_thread_info(ThreadInfo* _src, ThreadInfo* _tgt) {
	_tgt->context = _src->context;
	_tgt->thread_id = _src->thread_id;
	_tgt->thread_handle = _src->thread_handle;
	_tgt->plock = &_src->lock;
	_tgt->level = _src->level;
	_tgt->loop = _src->loop;
	if(_tgt->sequence) {
		AGE_FREE(_tgt->sequence);
		_tgt->sequence = 0;
	}
	if(_src->sequence) {
		_tgt->sequence = copy_string(_src->sequence);
		AGE_FREE(_src->sequence);
		_src->sequence = 0;
	}
	_tgt->position = _src->position;
	_tgt->time = _src->time;
}

#define __PLAY_ONE_NOTE(__c) \
	if(_is_note(note) || _is_note(last)) { \
		if(!info->context->mute) { \
			_age_beep(info->level, note ? note : last); \
			note = __c; \
		} \
	}
static s32 WINAPI sound_proc(Ptr param) {
	DWORD result = 0;
	ThreadInfo* _info = (ThreadInfo*)param;
	ThreadInfo* info = 0;
	s32 len = 0;
	s8 last = '\0';
	s8 ch = '\0';
	s8 note = '\0';

	EnterCriticalSection(&_info->lock); {
		info = AGE_MALLOC(ThreadInfo);
		_info->shadow = info;
		_copy_sound_thread_info(_info, info);
		len = info->sequence ? strlen(info->sequence) : 0;
	} LeaveCriticalSection(&_info->lock);

_again:
	EnterCriticalSection(info->plock); {
		if(_info->sequence) {
			_copy_sound_thread_info(_info, info);
			len = info->sequence ? strlen(info->sequence) : 0;
		}
		if(_info->stop) {
			_info->stop = FALSE;
			AGE_FREE(info->sequence);
			info->sequence = 0;
			len = 0;
		}
	} LeaveCriticalSection(info->plock);
	if(info->sequence) {
		while(info->position < len) {
			ch = info->sequence[info->position++];
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
			info->position = 0;
			goto _again;
		} else {
			AGE_FREE(info->sequence);
			info->position = 0;
			goto _again;
		}
	} else {
		age_sleep(100);
		goto _again;
	}

	return result;
}

#undef __PLAY_ONE_NOTE

SoundContext* create_sound_context(void) {
	SoundContext* result = AGE_MALLOC(SoundContext);
	ThreadInfo* bgm = 0;
	ThreadInfo* sfx = 0;
	bgm = result->bgm = AGE_MALLOC(ThreadInfo);
	sfx = result->sfx = AGE_MALLOC(ThreadInfo);
	bgm->context = result;
	sfx->context = result;
	bgm->level = 1;
	sfx->level = 1;

	InitializeCriticalSection(&bgm->lock);
	InitializeCriticalSection(&sfx->lock);

	bgm->thread_handle = CreateThread(0, 0, sound_proc, bgm, 0, &bgm->thread_id);
	sfx->thread_handle = CreateThread(0, 0, sound_proc, sfx, 0, &sfx->thread_id);
	SetThreadPriority(bgm->thread_handle, THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(sfx->thread_handle, THREAD_PRIORITY_HIGHEST);
	age_sleep(1);

	return result;
}

void destroy_sound_context(SoundContext* _cnt) {
	ThreadInfo* bgm = 0;
	ThreadInfo* sfx = 0;
	bgm = (ThreadInfo*)_cnt->bgm;
	sfx = (ThreadInfo*)_cnt->sfx;

	stop_sound(_cnt, ST_BGM);
	stop_sound(_cnt, ST_SFX);

	TerminateThread(bgm->thread_handle, 1);
	WaitForSingleObject(bgm->thread_handle, 10);
	CloseHandle(bgm->thread_handle);
	bgm->thread_handle = 0;
	bgm->thread_id = 0;
	TerminateThread(sfx->thread_handle, 1);
	WaitForSingleObject(sfx->thread_handle, 10);
	CloseHandle(sfx->thread_handle);
	sfx->thread_handle = 0;
	sfx->thread_id = 0;

	DeleteCriticalSection(&bgm->lock);
	DeleteCriticalSection(&sfx->lock);

	if(bgm->shadow) {
		if(bgm->shadow->sequence) {
			AGE_FREE(bgm->shadow->sequence);
		}
		AGE_FREE(bgm->shadow);
	}
	if(sfx->shadow) {
		if(sfx->shadow->sequence) {
			AGE_FREE(sfx->shadow->sequence);
		}
		AGE_FREE(sfx->shadow);
	}
	if(bgm->sequence) {
		AGE_FREE(bgm->sequence);
	}
	if(sfx->sequence) {
		AGE_FREE(sfx->sequence);
	}
	AGE_FREE(_cnt->bgm);
	AGE_FREE(_cnt->sfx);
	AGE_FREE(_cnt);
}

void update_sound(SoundContext* _cnt, s32 _elapsedTime) {
	/* do nothing */
}

void set_sound_mute(SoundContext* _cnt, bl _mute) {
	_cnt->mute = _mute;
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
	EnterCriticalSection(&thread->lock); {
		thread->sequence = copy_string(_seq);
		thread->loop = _loop;
		thread->stop = FALSE;
	} LeaveCriticalSection(&thread->lock);
	age_sleep(1);
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
		if(thread->thread_handle) {
			EnterCriticalSection(&thread->lock); {
				thread->stop = TRUE;
				if(thread->sequence) {
					AGE_FREE(thread->sequence);
				}
				thread->level = 1;
				thread->position = 0;
			} LeaveCriticalSection(&thread->lock);
		}
	}
}
