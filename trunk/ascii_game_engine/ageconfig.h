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

#ifndef __AGE_CONFIG_H__
#define __AGE_CONFIG_H__

#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <crtdbg.h>

#ifdef _MSC_VER
#	ifdef INTE_COMPILE
#		define AGE_API __declspec(dllexport)
#	else
#		define AGE_API __declspec(dllimport)
#	endif
#	define AGE_INTERNAL
#else
#	define AGE_API __attribute__ ((visibility("default")))
#	define AGE_INTERNAL __attribute__ ((visibility("hidden")))
#endif

#ifndef CANVAS_WIDTH
#	define CANVAS_WIDTH 80
#endif
#ifndef CANVAS_HEIGHT
#	define CANVAS_HEIGHT 25
#endif

#ifndef AGE_STR_LEN
#	define AGE_STR_LEN 256
#endif

#ifndef EXPECTED_FPS
#	define EXPECTED_FPS 60
#endif
#ifndef EXPECTED_FRAME_TIME
#	define EXPECTED_FRAME_TIME (1000 / EXPECTED_FPS)
#endif

#endif /* __AGE_CONFIG_H__ */
