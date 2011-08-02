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

#include "agehashtable.h"

#define safe_free(__p) { if(__p) { free(__p); __p = 0; } else { assert("Memory already released"); } }

u32 ht_hash_string(Ptr ht, Ptr d) {
	u32 result = 0;
	ht_node_t* self = (ht_node_t*)ht;
	Str s = (Str)d;
	u32 h = 0;

	assert(ht);

	for( ; *s; ++s) {
		h = 5 * h + *s;
	}

	result = h % self->array_size;

	return result;
}

u32 ht_hash_int(Ptr ht, Ptr d) {
	u32 result = 0;
	ht_node_t* self = (ht_node_t*)ht;
	s32 i = *(s32*)d;

	assert(ht);

	result = (u32)i;
	result %= self->array_size;

	return result;
}

u32 ht_hash_real(Ptr ht, Ptr d) {
	f32 r = *(f32*)d;
	union {
		f32 r;
		s32 i;
	} u;
	u.r = r;

	return ht_hash_int(ht, &u.i);
}

u32 ht_hash_ptr(Ptr ht, Ptr d) {
	union {
		s32 i;
		Ptr p;
	} u;
	u.p = d;

	return ht_hash_int(ht, &u.i);
}

s32 ht_cmp_string(Ptr d1, Ptr d2) {
	Str s1 = (Str)d1;
	Str s2 = (Str)d2;

	return strcmp(s1, s2);
}

s32 ht_cmp_int(Ptr d1, Ptr d2) {
	s32 i1 = *(s32*)d1;
	s32 i2 = *(s32*)d2;
	s32 i = i1 - i2;
	s32 result = 0;
	if(i < 0) {
		result = -1;
	} else if(i > 0) {
		result = 1;
	}

	return result;
}

s32 ht_cmp_real(Ptr d1, Ptr d2) {
	f32 r1 = *(f32*)d1;
	f32 r2 = *(f32*)d2;
	f32 r = r1 - r2;
	s32 result = 0;
	if(r < 0.0f) {
		result = -1;
	} else if(r > 0.0f) {
		result = 1;
	}

	return result;
}

s32 ht_cmp_ptr(Ptr d1, Ptr d2) {
	s32 i1 = *(s32*)d1;
	s32 i2 = *(s32*)d2;
	s32 i = i1 - i2;
	s32 result = 0;
	if(i < 0) {
		result = -1;
	} else if(i > 0) {
		result = 1;
	}

	return result;
}

ht_node_t* ht_create(u32 size, ht_compare cmp, ht_hash hs, ls_operation freeextra) {
	const u32 array_size = size ? size : HT_ARRAY_SIZE_DEFAULT;
	ht_node_t* result = 0;
	u32 ul = 0;

	if(!cmp) {
		cmp = ht_cmp_int;
	}
	if(!hs) {
		hs = ht_hash_int;
	}

	result = (ht_node_t*)malloc(sizeof(ht_node_t));
	result->free_extra = freeextra;
	result->compare = cmp;
	result->hash = hs;
	result->array_size = array_size;
	result->count = 0;
	result->array = (ls_node_t**)malloc(sizeof(ls_node_t*) * result->array_size);
	for(ul = 0; ul < result->array_size; ++ul) {
		result->array[ul] = ls_create();
	}

	return result;
}

ls_node_t* ht_find(ht_node_t* ht, Ptr key) {
	ls_node_t* result = 0;
	ls_node_t* bucket = 0;
	u32 hash_code = 0;

	assert(ht && key);

	hash_code = ht->hash(ht, key);
	bucket = ht->array[hash_code];
	bucket = bucket->next;
	while(bucket) {
		if(ht->compare(bucket->extra, key) == 0) {
			result = bucket;
			break;
		}
		bucket = bucket->next;
	}

	return result;
}

u32 ht_count(ht_node_t* ht) {
	u32 result = 0;

	assert(ht);

	result = ht->count;

	return result;
}

u32 ht_get(ht_node_t* ht, Ptr key, Ptr* value) {
	u32 result = 0;
	ls_node_t* bucket = 0;

	assert(ht && key && value);

	bucket = ht_find(ht, key);
	if(bucket) {
		*value = bucket->data;
		++result;
	}

	return result;
}

u32 ht_set(ht_node_t* ht, Ptr key, Ptr value) {
	u32 result = 0;
	ls_node_t* bucket = 0;

	assert(ht && key);

	bucket = ht_find(ht, key);
	if(bucket) {
		bucket->data = value;
		++result;
	}

	return result;
}

u32 ht_set_or_insert(ht_node_t* ht, Ptr key, Ptr value) {
	u32 result = 0;
	ls_node_t* bucket = 0;
	u32 hash_code = 0;

	assert(ht && key);

	bucket = ht_find(ht, key);
	if(bucket) { /* Update */
		bucket->data = value;
		++result;
	} else { /* Insert */
		hash_code = ht->hash(ht, key);
		bucket = ht->array[hash_code];
		bucket = ls_pushback(bucket, value);
		assert(bucket);
		bucket->extra = key;
		++result;
	}

	return result;
}

u32 ht_remove(ht_node_t* ht, Ptr key) {
	u32 result = 0;
	u32 hash_code = 0;
	ls_node_t* bucket = 0;

	assert(ht && key);

	bucket = ht_find(ht, key);
	hash_code = ht->hash(ht, key);
	bucket = ht->array[hash_code];
	result = ls_try_remove(bucket, key, ls_cmp_extra);

	return result;
}

u32 ht_foreach(ht_node_t* ht, ht_operation op) {
	u32 result = 0;
	ls_node_t* bucket = 0;
	u32 ul = 0;

	for(ul = 0; ul < ht->array_size; ++ul) {
		bucket = ht->array[ul];
		if(bucket) {
			result += ls_foreach(bucket, op);
		}
	}

	return result;
}

bl ht_empty(ht_node_t* ht) {
	return 0 == ht_count(ht);
}

void ht_clear(ht_node_t* ht) {
	u32 ul = 0;

	assert(ht && ht->array);

	for(ul = 0; ul < ht->array_size; ++ul) {
		ls_clear(ht->array[ul]);
	}
}

void ht_destroy(ht_node_t* ht) {
	u32 ul = 0;

	assert(ht && ht->array);

	if(ht->free_extra) {
		ht_foreach(ht, ht->free_extra);
	}

	for(ul = 0; ul < ht->array_size; ++ul) {
		ls_destroy(ht->array[ul]);
	}
	safe_free(ht->array);
	safe_free(ht);
}
