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

#ifndef __AGE_BITSET_H__
#define __AGE_BITSET_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "../common/ageallocator.h"

typedef struct Bitset {
	s32 bitCount;
	s32 dwordCount;
	s32 byteCount;
	u32* raw;
} Bitset;

AGE_INTERNAL Bitset* bs_create(s32 _size);
AGE_INTERNAL void bs_destroy(Bitset* _bs);
AGE_INTERNAL void bs_clear(Bitset* _bs);
AGE_INTERNAL void bs_set_bit(Bitset* _bs, s32 _pos, bl _val);
AGE_INTERNAL void bs_set_all_bits(Bitset* _bs, Bitset* _other);
AGE_INTERNAL bl bs_get_bit(Bitset* _bs, s32 _pos);
AGE_INTERNAL bl bs_empty(Bitset* _bs);
AGE_INTERNAL bl bs_equals(Bitset* _bs, Bitset* _other);
AGE_INTERNAL void bs_arithmetic_and(Bitset* _bs, Bitset* _other, Bitset* _result);
AGE_INTERNAL void bs_arithmetic_or(Bitset* _bs, Bitset* _other, Bitset* _result);
AGE_INTERNAL bl bs_logic_and(Bitset* _bs, Bitset* _other);
AGE_INTERNAL bl bs_logic_or(Bitset* _bs, Bitset* _other);

#endif /* __AGE_BITSET_H__ */
