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

#include "ageaudio.h"

typedef struct ThreadInfo {
	DWORD threadId;
	HANDLE threadHandle;
	CRITICAL_SECTION lock;
} ThreadInfo;

static void lock(Ptr _info) {
	ThreadInfo* info = (ThreadInfo*)_info;
	EnterCriticalSection(&(info->lock));
}

static void unlock(Ptr _info) {
	ThreadInfo* info = (ThreadInfo*)_info;
	LeaveCriticalSection(&(info->lock));
}

static s32 WINAPI sound_proc(Ptr param) {
	DWORD result = 0;
	SoundContext* cnt = (SoundContext*)param;

	// TODO

	return result;
}

SoundContext* create_sound_context(void) {
	SoundContext* result = AGE_MALLOC(SoundContext);
	result->bgm = AGE_MALLOC(ThreadInfo);
	result->sfx = AGE_MALLOC(ThreadInfo);
	InitializeCriticalSection(&((ThreadInfo*)(result->bgm))->lock);
	InitializeCriticalSection(&((ThreadInfo*)(result->sfx))->lock);

	return result;
}

void destroy_sound_context(SoundContext* _cnt) {
	DeleteCriticalSection(&((ThreadInfo*)(_cnt->bgm))->lock);
	DeleteCriticalSection(&((ThreadInfo*)(_cnt->sfx))->lock);
	AGE_FREE(_cnt->bgm);
	AGE_FREE(_cnt->sfx);
	AGE_FREE(_cnt);
}

void age_sound_update(SoundContext* _cnt, s32 _elapsedTime) {
	// TODO
}

void age_play_sound(SoundContext* _cnt, const Str _seq, SoundType _type) {
	ThreadInfo* thread = 0;
	age_stop_sound(_cnt, _type);
	if(_type == ST_BGM) {
		thread = (ThreadInfo*)(_cnt->bgm);
	} else if(_type == ST_SFX) {
		thread = (ThreadInfo*)(_cnt->sfx);
	} else {
		assert("Unknown sound type");
	}
	thread->threadHandle = CreateThread(0, 0, sound_proc, _cnt, 0, &thread->threadId);
}

void age_stop_sound(SoundContext* _cnt, SoundType _type) {
	ThreadInfo* thread = 0;
	if(_type == ST_BGM) {
		thread = (ThreadInfo*)(_cnt->bgm);
	} else if(_type == ST_SFX) {
		thread = (ThreadInfo*)(_cnt->sfx);
	} else {
		assert("Unknown sound type");
	}
	TerminateThread(thread->threadHandle, 1);
	WaitForSingleObject(thread->threadHandle, 10);
	CloseHandle(thread->threadHandle);
	thread->threadHandle = 0;
	thread->threadId = 0;
}
