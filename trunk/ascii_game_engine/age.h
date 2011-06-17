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

#ifndef __AGE_H__
#define __AGE_H__

#include <assert.h>

#include "ageconfig.h"
#include "script/my_basic/my_basic.h"
#include "script/agescriptapi.h"
#include "common/agetype.h"
#include "common/ageallocator.h"
#include "input/ageinput.h"
#include "render/agerenderer.h"

typedef struct {
	Canvas* canvas;
	mb_interpreter_t* script;
} World;

AGE_API u32 get_ver(void);
AGE_API const Str get_ver_string(void);

AGE_API World* create_world(void);
AGE_API World* get_world(void);
AGE_API void destroy_world(World* _wld);

AGE_API bl config_world(const Str _cfgFile);

AGE_API bl run_world_script(const Str _sptFile);
AGE_API bl run_new_script(const Str _sptFile);

#endif /* __AGE_H__ */
