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

#include "agelist.h"

#define safe_free(__p) { if(__p) { free(__p); __p = 0; } else { assert("Memory already released"); } }

s32 ls_cmp_data(Ptr node, Ptr info) {
	ls_node_t* n = (ls_node_t*)node;

	return (n->data == info) ? 0 : 1;
}

s32 ls_cmp_extra(Ptr node, Ptr info) {
	ls_node_t* n = (ls_node_t*)node;

	return (n->extra == info) ? 0 : 1;
}

ls_node_t* ls_create_node(Ptr data) {
	ls_node_t* result = 0;

	result = (ls_node_t*)malloc(sizeof(ls_node_t));
	assert(result);
	memset(result, 0, sizeof(ls_node_t));
	result->data = data;

	return result;
}

ls_node_t* ls_create(void) {
	ls_node_t* result = 0;

	result = ls_create_node(0);

	return result;
}

ls_node_t* ls_front(ls_node_t* node) {
	ls_node_t* result = node;

	result = result->next;

	return result;
}

ls_node_t* ls_back(ls_node_t* node) {
	ls_node_t* result = node;

	result = result->prev;

	return result;
}

ls_node_t* ls_at(ls_node_t* list, s32 pos) {
	ls_node_t* result = list;
	s32 i = 0;

	assert(result && pos >= 0);

	for(i = 0; i <= pos; ++i) {
		if(!result->next) {
			result = 0;
			break;
		} else {
			result = result->next;
		}
	}

	return result;
}

ls_node_t* ls_pushback(ls_node_t* list, Ptr data) {
	ls_node_t* result = 0;
	ls_node_t* tmp = 0;

	assert(list);

	result = ls_create_node(data);

	tmp = ls_back(list);
	if(!tmp) {
		tmp = list;
	}
	tmp->next = result;
	result->prev = tmp;
	list->prev = result;

	return result;
}

ls_node_t* ls_pushfront(ls_node_t* list, Ptr data) {
	ls_node_t* result = 0;
	ls_node_t* head = 0;

	assert(list);

	result = ls_create_node(data);

	head = list;
	list = ls_front(list);
	head->next = result;
	result->prev = head;
	if(list) {
		result->next = list;
		list->prev = result;
	}

	return result;
}

ls_node_t* ls_insert(ls_node_t* list, s32 pos, Ptr data) {
	ls_node_t* result = 0;
	ls_node_t* tmp = 0;

	assert(list && pos >= 0);

	list = ls_at(list, pos);
	assert(list);
	if(list) {
		result = ls_create_node(data);
		tmp = list->prev;

		tmp->next = result;
		result->prev = tmp;

		result->next = list;
		list->prev = result;
	}

	return result;
}

Ptr ls_popback(ls_node_t* list) {
	Ptr result = 0;
	ls_node_t* tmp = 0;

	assert(list);

	tmp = ls_back(list);
	if(tmp) {
		result = tmp->data;

		if(list != tmp->prev) {
			list->prev = tmp->prev;
		} else {
			list->prev = 0;
		}

		tmp->prev->next = 0;
		safe_free(tmp);
	}

	return result;
}

Ptr ls_popfront(ls_node_t* list) {
	Ptr result = 0;
	ls_node_t* tmp = 0;

	assert(list);

	tmp = ls_front(list);
	if(tmp) {
		result = tmp->data;

		if(!tmp->next) {
			list->prev = 0;
		}

		tmp->prev->next = tmp->next;
		if(tmp->next) {
			tmp->next->prev = tmp->prev;
		}
		safe_free(tmp);
	}

	return result;
}

u32 ls_remove(ls_node_t* list, s32 pos) {
	u32 result = 0;
	ls_node_t* tmp = 0;

	assert(list && pos >= 0);

	tmp = ls_at(list, pos);
	if(tmp) {
		if(tmp->prev) {
			tmp->prev->next = tmp->next;
		}
		if(tmp->next) {
			tmp->next->prev = tmp->prev;
		} else {
			list->prev = tmp->prev;
		}

		safe_free(tmp);

		++result;
	}

	return result;
}

u32 ls_try_remove(ls_node_t* list, Ptr info, ls_compare cmp) {
	u32 result = 0;
	ls_node_t* tmp = 0;

	assert(list && cmp);

	tmp = list->next;
	while(tmp) {
		if(cmp(tmp, info) == 0) {
			if(tmp->prev) {
				tmp->prev->next = tmp->next;
			}
			if(tmp->next) {
				tmp->next->prev = tmp->prev;
			}
			safe_free(tmp);
			++result;
			break;
		}
		tmp = tmp->next;
	}

	return result;
}

u32 ls_count(ls_node_t* list) {
	u32 result = 0;

	assert(list);

	while(list->next) {
		++result;
		list = list->next;
	}

	return result;
}

u32 ls_foreach(ls_node_t* list, ls_operation op) {
	u32 idx = 0;
	s32 opresult = ACL_OP_RESULT_NORMAL;
	ls_node_t* tmp = 0;

	assert(list && op);

	list = list->next;
	while(list) {
		opresult = (*op)(list->data, list->extra);
		++idx;
		tmp = list;
		list = list->next;

		if(ACL_OP_RESULT_NORMAL == opresult) {
			/* Do nothing */
		} else if(ACL_OP_RESULT_DEL_NODE == opresult) {
			tmp->prev->next = list;
			if(list) {
				list->prev = tmp->prev;
			}
			safe_free(tmp);
		} else {
			/* Do nothing */
		}
	}

	return idx;
}

bl ls_empty(ls_node_t* list) {
	bl result = FALSE;

	assert(list);

	result = 0 == list->next;

	return result;
}

void ls_clear(ls_node_t* list) {
	ls_node_t* tmp = 0;

	assert(list);

	tmp = list;
	list = list->next;
	tmp->next = 0;
	tmp->prev = 0;

	while(list) {
		tmp = list;
		list = list->next;
		safe_free(tmp);
	}
}

void ls_destroy(ls_node_t* list) {
	ls_clear(list);

	safe_free(list);
}

s32 ls_free_extra(Ptr data, Ptr extra) {
	s32 result = ACL_OP_RESULT_NORMAL;

	assert(extra);

	safe_free(extra);

	result = ACL_OP_RESULT_DEL_NODE;

	return result;
}
