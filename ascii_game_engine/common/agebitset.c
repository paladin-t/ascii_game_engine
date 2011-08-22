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

#include "agebitset.h"

#ifndef _size_to_dword
#	define _size_to_dword(n) (((n) - 1) / 32 + 1)
#endif

#ifndef _size_to_byte
#	define _size_to_byte(n) (((n) - 1) / 8 + 1)
#endif

static void _getRawArrayPos(s32 _pos, s32* _index, s32* _offset) {
	*_index = _pos / 32;
	*_offset = _pos % 32;
}

Bitset* bs_create(s32 _size) {
	Bitset* result = 0;

	result = AGE_MALLOC(Bitset);
	result->bitCount = _size;
	result->dwordCount = _size_to_dword(_size);
	result->byteCount = result->dwordCount * sizeof(u32);
	result->raw = AGE_MALLOC_N(u32, result->dwordCount);

	return result;
}

void bs_destroy(Bitset* _bs) {
	AGE_FREE_N(_bs->raw);
	AGE_FREE(_bs);
}

void bs_clear(Bitset* _bs) {
	memset(_bs->raw, 0, _bs->byteCount);
}

void bs_set_bit(Bitset* _bs, s32 _pos, bl _val) {
	s32 index = 0;
	s32 offset = 0;
	_getRawArrayPos(_pos, &index, &offset);
	if(_val) {
		_bs->raw[index] |= (1 << offset);
	} else {
		_bs->raw[index] &= ~(1 << offset);
	}
}

void bs_set_all_bits(Bitset* _bs, Bitset* _other) {
	assert(_bs && _other);
	assert(_bs->bitCount == _other->bitCount);
	if(_bs->bitCount == _other->bitCount) {
		memcpy(_bs->raw, _other->raw, _bs->byteCount);
	}
}

bl bs_get_bit(Bitset* _bs, s32 _pos) {
	s32 index = 0;
	s32 offset = 0;
	_getRawArrayPos(_pos, &index, &offset);

	return (bl)((_bs->raw[index] | (1 << offset)) != 0);
}

bl bs_empty(Bitset* _bs) {
	s32 i = 0;
	for(i = 0; i < _bs->dwordCount; ++i) {
		if(_bs->raw[i] != 0) {
			return FALSE;
		}
	}

	return TRUE;
}

bl bs_equals(Bitset* _bs, Bitset* _other) {
	assert(_bs && _other);
	assert(_bs->bitCount == _other->bitCount);
	if(_bs->bitCount == _other->bitCount) {
		return (bl)(memcmp(_bs->raw, _other->raw, _bs->byteCount) == 0);
	} else {
		return FALSE;
	}
}

void bs_arithmetic_and(Bitset* _bs, Bitset* _other, Bitset* _result) {
	s32 i = 0;

	assert(_bs && _other && _result);
	assert(_bs->bitCount == _other->bitCount && _bs->bitCount == _result->bitCount);
	if(_bs->bitCount == _other->bitCount) {
		for(i = 0; i < _bs->dwordCount; ++i) {
			_result->raw[i] = _bs->raw[i] & _other->raw[i];
		}
	}
}

void bs_arithmetic_or(Bitset* _bs, Bitset* _other, Bitset* _result) {
	s32 i = 0;

	assert(_bs && _other && _result);
	assert(_bs->bitCount == _other->bitCount && _bs->bitCount == _result->bitCount);
	if(_bs->bitCount == _other->bitCount) {
		for(i = 0; i < _bs->dwordCount; ++i) {
			_result->raw[i] = _bs->raw[i] | _other->raw[i];
		}
	}
}

bl bs_logic_and(Bitset* _bs, Bitset* _other) {
	s32 i = 0;

	assert(_bs && _other);
	assert(_bs->bitCount == _other->bitCount);
	if(_bs->bitCount == _other->bitCount) {
		for(i = 0; i < _bs->dwordCount; ++i) {
			if((_bs->raw[i] & _other->raw[i]) != 0) {
				return TRUE;
			}
		}

		return FALSE;
	} else {
		return FALSE;
	}
}

bl bs_logic_or(Bitset* _bs, Bitset* _other) {
	s32 i = 0;

	assert(_bs && _other);
	assert(_bs->bitCount == _other->bitCount);
	if(_bs->bitCount == _other->bitCount) {
		for(i = 0; i < _bs->dwordCount; ++i) {
			if((_bs->raw[i] | _other->raw[i]) != 0) {
				return TRUE;
			}
		}

		return FALSE;
	} else {
		return FALSE;
	}
}
