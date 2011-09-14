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

#ifndef __AGE_BITFSM_H__
#define __AGE_BITFSM_H__

#include "../common/agebitset.h"
#include "../common/agelist.h"

/**
 * @brief status structure in bitfsm
 */
typedef struct FsmStatus {
	Bitset* status; /**< internal status data */
	s32 index;      /**< index of this status */
} FsmStatus;

/**
 * @brief step structure in bitfsm
 */
typedef struct FsmStep {
	Bitset* condition; /**< status transition condition */
	s32 next;          /**< next status */
	bl exact;          /**< detect the condition exactly */
} FsmStep;

/**
 * @brief rule step structure
 */
typedef struct FsmRuleStep {
	s32 index;        /**< rule step index */
	FsmStep**  steps; /**< steps in this piece of rule */
	s32 steps_count;  /**< count of steps */
	Ptr tag;          /**< tagged rule step data */
} FsmRuleStep;

/**
 * @brief object to index convertion functor
 *
 * @param _obj - object
 * @return - converted index
 */
typedef s32 (* ObjToIndexFunc)(Ptr _obj);
/**
 * @brief object to command convertion functor
 *
 * @param _obj - object
 * @return - converted command
 */
typedef s32 (* ObjToCommandFunc)(Ptr _obj);

/**
 * @brief stepping callback handler using integer
 *
 * @param[in] _src - source step
 * @param[in] _tgt - target step
 */
typedef void (* IntStepHendlerFunc)(s32 _src, s32 _tgt);
/**
 * @brief stepping callback handler using object data
 *
 * @param[in] _src - source step
 * @param[in] _tgt - target step
 */
typedef void (* ObjStepHandlerFunc)(Ptr _src, Ptr _tgt);

/**
 * @brief fsm structure
 */
typedef struct Fsm {
	FsmStatus* current;              /**< current status */
	FsmRuleStep* rule_steps;         /**< transition rules */
	s32 rule_steps_count;            /**< transition rules count */
	s32 terminal_index;              /**< terminal index of this fsm */
	ObjToIndexFunc obj_to_index;     /**< object to index convertion functor */
	ObjToCommandFunc obj_to_command; /**< object to command convertion functor */
	IntStepHendlerFunc int_handler;  /**< stepping callback handler using integer */
	ObjStepHandlerFunc obj_handler;  /**< stepping callback handler using object data */
	destroyer tag_destructor;        /**< tag destructor functor */
} Fsm;

/**
 * @brief create a fsm status object
 *
 * @param[in] _bitsCount - size of bitset
 * @return - created fsm status object
 */
AGE_API FsmStatus* create_fsm_status(s32 _bitsCount);
/**
 * @brief destroy a fsm status object
 *
 * @param[in] _obj - fsm status object to be destroyed
 */
AGE_API void destroy_fsm_status(FsmStatus* _obj);

/**
 * @brief create a fsm step object
 *
 * @param[in] _bitsCount - size of bitset
 * @return - created fsm step object
 */
AGE_API FsmStep* create_fsm_step(s32 _bitsCount);
/**
 * @brief destroy a fsm step object
 *
 * @param[in] _obj - fsm step object to be destroyed
 */
AGE_API void destroy_fsm_step(FsmStep* _obj);

/**
 * @brief create a fsm rule step object
 *
 * @param[in] _count - count of rule steps to be allocated
 * @return - created fsm rule step object
 */
AGE_API FsmRuleStep* create_fsm_rule_step(s32 _count);
/**
 * @brief destroy a fsm rule step object
 *
 * @param[in] _obj - fsm rule step object to be destroyed
 */
AGE_API void destroy_fsm_rule_step(FsmRuleStep* _obj);
/**
 * @brief append a fsm step to a rule step object
 *
 * @param[in] _ruleStep - rule step object
 * @param[in] _step     - step object
 */
AGE_API void append_fsm_rule_step(FsmRuleStep* _ruleStep, FsmStep* _step);
/**
 * @brief remove a fsm step from a rule step object
 *
 * @param[in] _index - the index of a rule step to be removed
 */
AGE_API void remove_fsm_rule_step(FsmRuleStep* _ruleStep, s32 _index);

/**
 * @brief try to walk a step
 *
 * @param[in] _ruleStep - rule step object to be operated
 * @param[in] _curr     - current fsm status
 * @param[in] _status   - transition status data as a command
 * @param[in] _exact    - check transition rules exactly
 * @return - whether the transition is taken or not
 */
AGE_API bl walk_rule_step(FsmRuleStep* _ruleStep, FsmStatus* _curr, Bitset* _status, bl _exact);

/**
 * @brief create a bitfsm object
 *
 * @param[in] _statusCount   - status count
 * @param[in] _commandCount  - transition command count
 * @param[in] _objToIndex    - object to index convertion functor
 * @param[in] _objToCommand  - object to command convertion functor
 * @param[in] _intHandler    - integer stepping event handler
 * @param[in] _objHandler    - tag stepping event handler
 * @param[in] _tagDestructor - tag destructor functor
 * @return - created bitfsm object
 */
AGE_API Fsm* create_bitfsm(s32 _statusCount, s32 _commandCount, ObjToIndexFunc _objToIndex, ObjToCommandFunc _objToCommand, IntStepHendlerFunc _intHandler, ObjStepHandlerFunc _objHandler, destroyer _tagDestructor);
/**
 * @brief destroy a bitfsm object
 *
 * @param[in] _fsm - bitfsm object to be destroyed
 */
AGE_API void destroy_bitfsm(Fsm* _fsm);

/**
 * @brief reset a bitfsm to initialized status
 *
 * @param[in] _fsm - bitfsm object
 */
AGE_API void reset_bitfsm(Fsm* _fsm);
/**
 * @brief clear all data of a bitfsm
 *
 * @param[in] _fsm - bitfsm object
 */
AGE_API void clear_bitfsm(Fsm* _fsm);
/**
 * @brief set the current step of a bitfsm by index
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - current step index
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_bitfsm_current_step_index(Fsm* _fsm, s32 _index);
/**
 * @brief set the current step of a bitfsm by tag
 *
 * @param[in] _fsm - bitfsm object
 * @param[in] _obj - current step tag
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_bitfsm_current_step_tag(Fsm* _fsm, Ptr _obj);
/**
 * @brief set the terminal step of a bitfsm by index
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - terminal step index
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_bitfsm_terminal_step_index(Fsm* _fsm, s32 _index);
/**
 * @brief set the terminal step of a bitfsm by tag
 *
 * @param[in] _fsm - bitfsm object
 * @param[in] _obj - terminal step tag
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_bitfsm_terminal_step_tag(Fsm* _fsm, Ptr _obj);
/**
 * @brief set the step handler(s) of a bitfsm
 *
 * @param[in] _fsm    - bitfsm object
 * @param[in] _intHdl - integer handler functor
 * @param[in] _objHdl - tag handler functor
 */
AGE_API void set_bitfsm_step_handler(Fsm* _fsm, IntStepHendlerFunc _intHdl, ObjStepHandlerFunc _objHdl);
/**
 * @brief register a index to type rule of a rule step of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - index of a rule step
 * @param[in] _tag   - tag of a rule step
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl register_bitfsm_rule_step_index_to_tag(Fsm* _fsm, s32 _index, Ptr _tag);
/**
 * @brief register a rule step of a bitfsm by tag
 *
 * @param[in] _fsm - bitfsm object
 * @param[in] _obj - tag of a rule step
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl register_bitfsm_rule_step_tag(Fsm* _fsm, Ptr _obj);
/**
 * @brief add a rule step of a bitfsm by index
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - index of a rule step
 * @param[in] _cond  - transition condition
 * @param[in] _next  - next bitfsm status of this rule
 * @param[in] _exact - take this transition exactly
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl add_bitfsm_rule_step_by_index(Fsm* _fsm, s32 _index, Bitset* _cond, s32 _next, bl _exact);
/**
 * @brief add a rule step of a bitfsm by tag
 *
 * @param[in] _fsm      - bitfsm object
 * @param[in] _indexObj - tag of a rule step
 * @param[in] _cond     - transition condition
 * @param[in] _next     - next bitfsm status of this rule
 * @param[in] _exact    - take this transition exactly
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl add_bitfsm_rule_step_by_tag(Fsm* _fsm, Ptr _indexObj, Bitset* _cond, Ptr _nextObj, bl _exact);
/**
 * @brief add a rule step of a bitfsm by tag and conditon parameters
 *
 * @param[in] _fsm      - bitfsm object
 * @param[in] _indexObj - tag of a rule step
 * @param[in] _cond     - transition condition parameters
 * @param[in] _next     - next bitfsm status of this rule
 * @param[in] _exact    - take this transition exactly
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl add_bitfsm_rule_step_by_tag_params(Fsm* _fsm, Ptr _indexObj, ls_node_t* _cond, Ptr _nextObj, bl _exact);
/**
 * @brief remove a rule step of a bitfsm by index
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - index of the rule step to be removed
 * @param[in] _cond  - condition of the rule step to be removed
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl remove_bitfsm_rule_step_by_index(Fsm* _fsm, s32 _index, Bitset* _cond);
/**
 * @brief remove a rule step of a bitfsm by tag
 *
 * @param[in] _fsm      - bitfsm object
 * @param[in] _indexObj - tag of the rule step to be removed
 * @param[in] _cond     - condition of the rule step to be removed
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl remove_bitfsm_rule_step_by_tag(Fsm* _fsm, Ptr _indexObj, Bitset* _cond);
/**
 * @brief remove a rule step of a bitfsm by tag and condition parameters
 *
 * @param[in] _fsm      - bitfsm object
 * @param[in] _indexObj - tag of the rule step to be removed
 * @param[in] _cond     - condition parameters of the rule step to be removed
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl remove_bitfsm_rule_step_ty_tag_params(Fsm* _fsm, Ptr _indexObj, ls_node_t* _cond);
/**
 * @brief clear a rule step of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - index of a rule step to be removed
 */
AGE_API void clear_bitfsm_rule_step(Fsm* _fsm, s32 _index);
/**
 * @brief clear all rule steps of a bitfsm
 *
 * @param[in] _fsm - bitfsm object
 */
AGE_API void clear_bitfsm_all_rule_steps(Fsm* _fsm);
/**
 * @brief get the current step index of a bitfsm
 *
 * @param[in] _fsm - bitfsm object
 * @return - the current step index
 */
AGE_API s32 get_bitfsm_current_step(Fsm* _fsm);
/**
 * @brief get the current status of a bitfsm
 *
 * @param[in] _fsm - bitfsm object
 * @return - the current status
 */
AGE_API Bitset* get_bitfsm_current_status(Fsm* _fsm);
/**
 * @brief walk one step on a bitfsm with an integer as command
 *
 * @param[in] _fsm    - bitfsm object
 * @param[in] _status - status command index
 * @param[in] _exact  - treat this command exactly
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl walk_bitfsm_with_int(Fsm* _fsm, s32 _status, bl _exact);
/**
 * @brief walk one step on a bitfsm with a tag as command
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _obj   - status command tag
 * @param[in] _exact - treat this command exactly
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl walk_bitfsm_with_tag(Fsm* _fsm, Ptr _obj, bl _exact);
/**
 * @brief detect whether a bitfsm is at it's terminated status
 *
 * @param[in] _fsm - bitfsm object
 * @return - return TRUE if terminated, or FALSE if not
 */
AGE_API bl terminated_bitfsm(Fsm* _fsm);
/**
 * @brief get status count of a bitfsm
 *
 * @param[in] _fsm - bitfsm object
 * @return - status count
 */
AGE_API s32 get_bitfsm_status_count(Fsm* _fsm);
/**
 * @brief get a tag of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - the desired tag index
 * @return - tag data
 */
AGE_API Ptr get_bitfsm_status_tag(Fsm* _fsm, s32 _index);
/**
 * @brief get transition command count of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - the desired command index
 * @return - transiton command count
 */
AGE_API s32 get_bitfsm_command_count(Fsm* _fsm, s32 _index);
/**
 * @brief get a transition condition of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - the desired condition index
 * @param[in] _step  - the desired step index
 * @return - transiton condition
 */
AGE_API Bitset* get_bitfsm_step_command_condition(Fsm* _fsm, s32 _index, s32 _step);
/**
 * @brief get a next step index of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - the desired index
 * @param[in] _step  - the desired step index
 * @return - a next step index
 */
AGE_API s32 get_bitfsm_step_command_next(Fsm* _fsm, s32 _index, s32 _step);
/**
 * @brief get whether a condition transition rule is exactly mode of a bitfsm
 *
 * @param[in] _fsm   - bitfsm object
 * @param[in] _index - the desired conditon index
 * @param[in] _step  - the desired step index
 * @return - return TRUE if exactly, or FALSE if not
 */
AGE_API bl get_bitfsm_step_exact(Fsm* _fsm, s32 _index, s32 _step);

#endif /* __AGE_BITFSM_H__ */
