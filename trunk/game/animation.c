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

#include "animation.h"
#include "game.h"

static AsciiHeroFsmTag NORMAL_FSM_TAG = { 0, "normal", "end_normal" };
static AsciiHeroFsmTag FALLING_FSM_TAG = { 0, "walk", "end_walk" };
static AsciiHeroFsmTag WALKING_FSM_TAG = { 0, "walk", "end_walk" };

void destroy_fsm_tag(Ptr _ptr) {
	/* do nothing */
}

Ptr normal_fsm_tag(void) {
	return &NORMAL_FSM_TAG;
}

Ptr falling_fsm_tag(void) {
	return &FALLING_FSM_TAG;
}

Ptr walking_fsm_tag(void) {
	return &WALKING_FSM_TAG;
}

s32 fsm_tag_to_index(Ptr _obj) {
	AsciiHeroFsmTag* tag = (AsciiHeroFsmTag*)_obj;

	assert(_obj);

	return tag->index;
}

s32 fsm_tag_to_command(Ptr _obj) {
	s32 result = 0;

	// TODO

	return result;
}

void fsm_step_handler(Ptr _src, Ptr _tgt) {
	// TODO
}
