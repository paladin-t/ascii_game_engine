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

#ifndef __AGE_ALLOCATOR_H__
#define __AGE_ALLOCATOR_H__

#include "../ageconfig.h"
#include "../common/agetype.h"

/**
 * @brief memory allocator functor
 *
 * @param[in] _size - size to be allocated in bytes
 * @return - allocated memory
 */
typedef Ptr (* creator)(s32 _size);
/**
 * @brief memory deallocator functor
 *
 * @param[in] _ptr - pointer of a piece of memory to be deallocated
 */
typedef void (* destroyer)(Ptr _ptr);

#ifdef _DEBUG
/**
 * @brief malloc a piece of space with debug information
 *
 * @param[in] _size - bytes count to be malloced
 * @param[in] _file - caller file name
 * @param[in] _line - caller line number
 * @return - pointer to the malloced space
 */
AGE_API Ptr age_malloc_dbg(s32 _size, const Str _file, s32 _line);
#endif
/**
 * @brief malloc a piece of space
 *
 * @param[in] _size - bytes count to be malloced
 * @return - pointer to the malloced space
 */
AGE_API Ptr age_malloc(s32 _size);
/**
 * @brief realloc a piece of space
 *
 * @param[in] _ori  - original memory
 * @param[in] _size - new size in byts
 * @return - pointer to the realloced spece
 */
AGE_API Ptr age_realloc(Ptr _ori, s32 _size);
/**
 * @brief free a piece of space
 *
 * @param[in] _ptr - pointer to the malloced space
 */
AGE_API void age_free(Ptr _ptr);

#ifndef _age_malloc
#	ifdef _DEBUG
#		define _age_malloc(_type, _size) ((_type*)age_malloc_dbg(_size, __FILE__, __LINE__))
#	else
#		define _age_malloc(_type, _size) ((_type*)age_malloc(_size))
#	endif
#endif
#ifndef AGE_MALLOC
#	define AGE_MALLOC(_type) (_age_malloc(_type, sizeof(_type)))
#endif
#ifndef AGE_MALLOC_N
#	define AGE_MALLOC_N(_type, _count) (_age_malloc(_type, sizeof(_type) * _count))
#endif
#ifndef AGE_FREE
#	define AGE_FREE(_ptr) { age_free(_ptr); (_ptr) = 0; }
#endif
#ifndef AGE_FREE_N
#	define AGE_FREE_N(_ptr) { age_free(_ptr); (_ptr) = 0; }
#endif
#ifndef AGE_REALLOC
#	define AGE_REALLOC(_type, _ptr, _size) ((_type*)age_realloc(_ptr, _size))
#endif
#ifndef AGE_REALLOC_N
#	define AGE_REALLOC_N(_type, _ptr, _count) ((_type*)age_realloc(_ptr, sizeof(_type) * _count))
#endif

#endif /* __AGE_ALLOCATOR_H__ */
