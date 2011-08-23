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

#ifndef __AGE_LIST_H__
#define __AGE_LIST_H__

#include "../ageconfig.h"
#include "../common/agetype.h"

typedef s32 (* acl_common_compare)(Ptr, Ptr);

#define ACL_OP_RESULT_NORMAL 0
#define ACL_OP_RESULT_DEL_NODE -1
typedef s32 (* acl_common_operation)(Ptr, Ptr);

typedef acl_common_compare ls_compare;
typedef acl_common_operation ls_operation;

typedef struct ls_node_t {
	Ptr data;
	struct ls_node_t* prev;
	struct ls_node_t* next;
	Ptr extra;
} ls_node_t;

AGE_API s32 ls_cmp_data(Ptr node, Ptr info);
AGE_API s32 ls_cmp_extra(Ptr node, Ptr info);

AGE_API ls_node_t* ls_create_node(Ptr data);
AGE_API ls_node_t* ls_create(void);
AGE_API ls_node_t* ls_front(ls_node_t* node);
AGE_API ls_node_t* ls_back(ls_node_t* node);
AGE_API ls_node_t* ls_at(ls_node_t* list, s32 pos);
AGE_API ls_node_t* ls_pushback(ls_node_t* list, Ptr data);
AGE_API ls_node_t* ls_pushfront(ls_node_t* list, Ptr data);
AGE_API ls_node_t* ls_insert(ls_node_t* list, s32 pos, Ptr data);
AGE_API Ptr ls_popback(ls_node_t* list);
AGE_API Ptr ls_popfront(ls_node_t* list);
AGE_API u32 ls_remove(ls_node_t* list, s32 pos);
AGE_API u32 ls_try_remove(ls_node_t* list, Ptr info, ls_compare cmp);
AGE_API u32 ls_count(ls_node_t* list);
AGE_API u32 ls_foreach(ls_node_t* list, ls_operation op);
AGE_API bl ls_empty(ls_node_t* list);
AGE_API void ls_clear(ls_node_t* list);
AGE_API void ls_destroy(ls_node_t* list);
AGE_API s32 ls_free_extra(Ptr data, Ptr extra);

#endif /* __AGE_LIST_H__ */
