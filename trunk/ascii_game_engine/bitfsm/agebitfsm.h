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
	s32 stepsCount;   /**< count of steps */
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
	FsmStatus* current;            /**< current status */
	FsmRuleStep* ruleSteps;        /**< transition rules */
	s32 ruleStepsCount;            /**< transition rules count */
	s32 terminalIndex;             /**< terminal index of this fsm */
	ObjToIndexFunc objToIndex;     /**< object to index convertion functor */
	ObjToCommandFunc objToCommand; /**< object to command convertion functor */
	IntStepHendlerFunc intHandler; /**< stepping callback handler using integer */
	ObjStepHandlerFunc objHandler; /**< stepping callback handler using object data */
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
 * @return - created fsm rule step object
 */
AGE_API FsmRuleStep* create_fsm_rule_step(void);
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
 * @param[in] _statusCount  - status count
 * @param[in] _commandCount - transition command count
 * @param[in] _objToIndex   - object to index convertion functor
 * @param[in] _objToCommand - object to command convertion functor
 * @param[in] _intHandler   - integer stepping event handler
 * @param[in] _objHandler   - tag stepping event handler
 * @return - created bitfsm object
 */
AGE_API Fsm* create_bitfsm(s32 _statusCount, s32 _commandCount, ObjToIndexFunc _objToIndex, ObjToCommandFunc _objToCommand, IntStepHendlerFunc _intHandler, ObjStepHandlerFunc _objHandler);
/**
 * @brief destroy a bitfsm object
 *
 * @param[in] _fsm - bitfsm object to be destroyed
 */
AGE_API void destroy_bitfsm(Fsm* _fsm);

#endif /* __AGE_BITFSM_H__ */
