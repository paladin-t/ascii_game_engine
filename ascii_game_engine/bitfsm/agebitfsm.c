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

#include "agebitfsm.h"

FsmStatus* create_fsm_status(s32 _bitsCount) {
	FsmStatus* result = AGE_MALLOC(FsmStatus);
	result->status = bs_create(_bitsCount);
	result->index = -1;

	return result;
}

void destroy_fsm_status(FsmStatus* _obj) {
	bs_destroy(_obj->status);
	AGE_FREE(_obj);
}

FsmStep* create_fsm_step(s32 _bitsCount) {
	FsmStep* result = AGE_MALLOC(FsmStep);
	result->condition = bs_create(_bitsCount);
	result->next = -1;
	result->exact = FALSE;

	return result;
}

void destroy_fsm_step(FsmStep* _obj) {
	bs_destroy(_obj->condition);
	AGE_FREE(_obj);
}

FsmRuleStep* create_fsm_rule_step(void) {
	FsmRuleStep* result = AGE_MALLOC(FsmRuleStep);
	result->index = -1;
	result->steps = 0;
	result->stepsCount = 0;
	result->tag = 0;

	return result;
}

void destroy_fsm_rule_step(FsmRuleStep* _obj) {
	s32 i = 0;

	if(_obj->steps) {
		for(i = 0; i < _obj->stepsCount; ++i) {
			AGE_FREE(_obj->steps[i]);
		}
		AGE_FREE(_obj->steps);
	}
	AGE_FREE(_obj);
}

void append_fsm_rule_step(FsmRuleStep* _ruleStep, FsmStep* _step) {
	assert(_ruleStep && _step);

	_ruleStep->steps = AGE_REALLOC(FsmStep*, _ruleStep->steps, ++_ruleStep->stepsCount);
	_ruleStep->steps[_ruleStep->stepsCount - 1] = _step;
}

bl walk_rule_step(FsmRuleStep* _ruleStep, FsmStatus* _curr, Bitset* _status, bl _exact) {
	Bitset* _s = 0;
	s32 i = 0;
	FsmStep* _ck = 0;

	if(_ruleStep->index == -1) {
		return FALSE;
	}

	_s = bs_create(_status->bitCount);
	bs_arithmetic_or(_curr->status, _status, _s);
	for(i = 0; i < _ruleStep->stepsCount; ++i) {
		_ck = _ruleStep->steps[i];
		if((_ck->exact && bs_equals(_ck->condition, _s)) || (!_ck->exact && (bs_logic_and(_ck->condition, _s)))) {
			_curr->index = _ck->next;
			bs_set_all_bits(_curr->status, _exact ? _status : _s);

			return TRUE;
		}
	}
	bs_destroy(_s);

	return FALSE;
}

Fsm* create_bitfsm(s32 _statusCount, s32 _commandCount, ObjToIndexFunc _objToIndex, ObjToCommandFunc _objToCommand, IntStepHendlerFunc _intHandler, ObjStepHandlerFunc _objHandler) {
	Fsm* result = 0;

	result = AGE_MALLOC(Fsm);
	result->current = create_fsm_status(_commandCount);
	result->ruleSteps = create_fsm_rule_step();
	result->objToIndex = _objToIndex;
	result->objToCommand = _objToCommand;
	result->intHandler = _intHandler;
	result->objHandler = _objHandler;

	return result;
}

void destroy_bitfsm(Fsm* _fsm) {
	destroy_fsm_rule_step(_fsm->ruleSteps);
	destroy_fsm_status(_fsm->current);
	AGE_FREE(_fsm);
}

void reset_bitfsm(Fsm* _fsm) {
	bs_clear(_fsm->current->status);
	_fsm->current->index = -1;
	_fsm->terminalIndex = -1;
}

void clear_bitfsm(Fsm* _fsm) {
	reset_bitfsm(_fsm);
	clear_bitfsm_all_rule_steps(_fsm);
}

bl set_bitfsm_current_step_index(Fsm* _fsm, s32 _index) {
	// TODO
	return FALSE;
}

bl set_bitfsm_current_step_tag(Fsm* _fsm, Ptr _obj) {
	// TODO
	return FALSE;
}

bl set_bitfsm_terminal_step_index(Fsm* _fsm, s32 _index) {
	// TODO
	return FALSE;
}

bl set_bitfsm_terminal_step_tag(Fsm* _fsm, Ptr _obj) {
	// TODO
	return FALSE;
}

void set_bitfsm_step_handler(Fsm* _fsm, IntStepHendlerFunc _intHdl, ObjStepHandlerFunc _objHdl) {
	// TODO
}

bl register_bitfsm_rule_step_index_to_tag(Fsm* _fsm, s32 _index, Ptr _tag) {
	// TODO
	return FALSE;
}

bl register_bitfsm_rule_step_tag(Fsm* _fsm, Ptr _obj) {
	// TODO
	return FALSE;
}

bl add_bitfsm_rule_step_by_index(Fsm* _fsm, s32 _index, Bitset* _cond, s32 _next, bl _exact) {
	// TODO
	return FALSE;
}

bl add_bitfsm_rule_step_by_tag(Fsm* _fsm, Ptr _indexObj, Bitset* _cond, Ptr _next, bl _exact) {
	// TODO
	return FALSE;
}

bl add_bitfsm_rule_step_by_tag_params(Fsm* _fsm, Ptr _indexObj, ls_node_t* _cond, Ptr _next, bl _exact) {
	// TODO
	return FALSE;
}

bl remove_bitfsm_rule_step_by_index(Fsm* _fsm, s32 _index, Bitset* _cond) {
	// TODO
	return FALSE;
}

bl remove_bitfsm_rule_step_by_tag(Fsm* _fsm, Ptr _indexObj, Bitset* _cond) {
	// TODO
	return FALSE;
}

bl remove_bitfsm_rule_step_ty_tag_params(Fsm* _fsm, Ptr _indexObj, ls_node_t* _cond) {
	// TODO
	return FALSE;
}

void clear_bitfsm_rule_step(Fsm* _fsm, s32 _index) {
	// TODO
}

void clear_bitfsm_all_rule_steps(Fsm* _fsm) {
	// TODO
}

s32 get_bitfsm_current_step(Fsm* _fsm) {
	// TODO
	return 0;
}

Bitset* get_bitfsm_current_status(Fsm* _fsm) {
	// TODO
	return 0;
}

bl walk_bitfsm_with_int(Fsm* _fsm, s32 _status, bl _exact) {
	// TODO
	return FALSE;
}

bl walk_bitfsm_with_tag(Fsm* _fsm, Ptr _obj, bl _exact) {
	// TODO
	return FALSE;
}

bl terminated_bitfsm(Fsm* _fsm) {
	// TODO
	return FALSE;
}

s32 get_bitfsm_status_count(Fsm* _fsm) {
	// TODO
	return 0;
}

Ptr get_bitfsm_status_tag(Fsm* _fsm, s32 _index) {
	// TODO
	return 0;
}

s32 get_bitfsm_command_count(Fsm* _fsm) {
	// TODO
	return 0;
}

Bitset* get_bitfsm_step_command_condition(Fsm* _fsm, s32 _index, s32 _step) {
	// TODO
	return 0;
}

s32 get_bitfsm_step_command_next(Fsm* _fsm, s32 _index, s32 _step) {
	// TODO
	return 0;
}

bl get_bitfsm_step_exact(Fsm* _fsm, s32 _index, s32 _step) {
	// TODO
	return FALSE;
}
