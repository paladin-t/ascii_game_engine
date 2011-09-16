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

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "age.h"

typedef struct AsciiHeroFsmTag {
	s32 index;
	struct {
		Str start_frame;
		Str end_frame;
	};
} AsciiHeroFsmTag;

typedef struct AsciiHeroFsmCmd {
	Str info;
} AsciiHeroFsmCmd;

void destroy_fsm_tag(Ptr _ptr);

Ptr normal_fsm_tag(void);
Ptr falling_fsm_tag(void);
Ptr walking_fsm_tag(void);
Ptr died_fsm_tag(void);

Ptr normal_fsm_cmd(void);
Ptr walking_fsm_cmd(void);
Ptr collide_fsm_cmd(void);
Ptr no_collide_fsm_cmd(void);
Ptr kill_fsm_cmd(void);

s32 fsm_tag_to_index(Ptr _obj);

s32 fsm_tag_to_command(Ptr _obj);

void fsm_step_handler(Ptr _src, Ptr _tgt);

void open_ascii_hero_animation_fsm(Fsm* _fsm);
void close_ascii_hero_animation_fsm(Fsm* _fsm);

#endif /* __ANIMATION_H__ */
