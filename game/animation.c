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
static AsciiHeroFsmTag FALLING_FSM_TAG = { 1, "fall", "end_fall" };
static AsciiHeroFsmTag WALKING_FSM_TAG = { 2, "walk", "end_walk" };
static AsciiHeroFsmTag DIED_FSM_TAG = { 3, "died", "end_died" };

static AsciiHeroFsmCmd NORMAL_FSM_CMD = { "normal" };
static AsciiHeroFsmCmd WALKING_FSM_CMD = { "walk" };
static AsciiHeroFsmCmd COLLIDE_FSM_CMD = { "collide" };
static AsciiHeroFsmCmd NO_COLLIDE_FSM_CMD = { "no_collide" };
static AsciiHeroFsmCmd KILL_FSM_CMD = { "kill" };

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

Ptr died_fsm_tag(void) {
	return &DIED_FSM_TAG;
}

Ptr normal_fsm_cmd(void) {
	return &NORMAL_FSM_CMD;
}

Ptr walking_fsm_cmd(void) {
	return &WALKING_FSM_CMD;
}

Ptr collide_fsm_cmd(void) {
	return &COLLIDE_FSM_CMD;
}

Ptr no_collide_fsm_cmd(void) {
	return &NO_COLLIDE_FSM_CMD;
}

Ptr kill_fsm_cmd(void) {
	return &KILL_FSM_CMD;
}

void open_ascii_hero_animation_fsm(Fsm* _fsm) {
	Bitset* bs = 0;

	register_bitfsm_rule_step_tag(_fsm, normal_fsm_tag());
	register_bitfsm_rule_step_tag(_fsm, falling_fsm_tag());
	register_bitfsm_rule_step_tag(_fsm, walking_fsm_tag());
	register_bitfsm_rule_step_tag(_fsm, died_fsm_tag());

	bs = bs_create(5);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(no_collide_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, normal_fsm_tag(), bs, falling_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(collide_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, falling_fsm_tag(), bs, normal_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(walking_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, normal_fsm_tag(), bs, walking_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(normal_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, walking_fsm_tag(), bs, normal_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(no_collide_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, walking_fsm_tag(), bs, falling_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(kill_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, normal_fsm_tag(), bs, died_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(kill_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, falling_fsm_tag(), bs, died_fsm_tag(), TRUE);
	bs_clear(bs);
	bs_set_bit(bs, fsm_tag_to_command(kill_fsm_cmd()), TRUE);
	add_bitfsm_rule_step_by_tag(_fsm, walking_fsm_tag(), bs, died_fsm_tag(), TRUE);
	bs_destroy(bs);

	set_bitfsm_current_step_tag(_fsm, normal_fsm_tag());
	walk_bitfsm_with_tag(_fsm, no_collide_fsm_cmd(), TRUE);
}

void close_ascii_hero_animation_fsm(Fsm* _fsm) {
	clear_bitfsm(_fsm);
}

s32 fsm_tag_to_index(Ptr _obj) {
	AsciiHeroFsmTag* tag = (AsciiHeroFsmTag*)_obj;

	assert(_obj);

	return tag->index;
}

s32 fsm_tag_to_command(Ptr _obj) {
	s32 result = 0;
	AsciiHeroFsmCmd* cmd = (AsciiHeroFsmCmd*)_obj;

	assert(_obj);
	if(cmd == &NORMAL_FSM_CMD) {
		result = 0;
	} else if(cmd == &WALKING_FSM_CMD) {
		result = 1;
	} else if(cmd == &COLLIDE_FSM_CMD) {
		result = 2;
	} else if(cmd == &NO_COLLIDE_FSM_CMD) {
		result = 3;
	} else if(cmd == &KILL_FSM_CMD) {
		result = 4;
	} else {
		assert(0 && "Unknown FSM command");
	}

	return result;
}

void fsm_step_handler(Ptr _src, Ptr _tgt) {
	s8 buf[AGE_STR_LEN];
	AsciiHeroFsmTag* t1 = (AsciiHeroFsmTag*)_src;
	AsciiHeroFsmTag* t2 = (AsciiHeroFsmTag*)_tgt;
	sprintf(buf, "from %d: %s, to %d: %s\n", t1->index, t1->start_frame, t2->index, t2->start_frame);
#ifdef _DEBUG
	OutputDebugStringA(buf);
#endif

	play_sprite(AGE_CVS, game()->main,
		t2->start_frame, t2->end_frame,
		TRUE,
		on_playing_for_sprite_main_player
	);
	resume_sprite(AGE_CVS, game()->main);
}
