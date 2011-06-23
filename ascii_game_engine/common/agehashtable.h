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

#ifndef __AGE_HASH_TABLE_H__
#define __AGE_HASH_TABLE_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "agelist.h"

u32 ht_hash_string(Ptr ht, Ptr d);
u32 ht_hash_int(Ptr ht, Ptr d);
u32 ht_hash_real(Ptr ht, Ptr d);
u32 ht_hash_ptr(Ptr ht, Ptr d);

s32 ht_cmp_string(Ptr d1, Ptr d2);
s32 ht_cmp_int(Ptr d1, Ptr d2);
s32 ht_cmp_real(Ptr d1, Ptr d2);
s32 ht_cmp_ptr(Ptr d1, Ptr d2);

ht_node_t* ht_create(u32 size, ht_compare cmp, ht_hash hs, ls_operation freeextra);
ls_node_t* ht_find(ht_node_t* ht, Ptr key);
u32 ht_count(ht_node_t* ht);
u32 ht_get(ht_node_t* ht, Ptr key, Ptr* value);
u32 ht_set(ht_node_t* ht, Ptr key, Ptr value);
u32 ht_set_or_insert(ht_node_t* ht, Ptr key, Ptr value);
u32 ht_remove(ht_node_t* ht, Ptr key);
u32 ht_foreach(ht_node_t* ht, ht_operation op);
bl ht_empty(ht_node_t* ht);
void ht_clear(ht_node_t* ht);
void ht_destroy(ht_node_t* ht);

#endif /* __AGE_HASH_TABLE_H__ */
