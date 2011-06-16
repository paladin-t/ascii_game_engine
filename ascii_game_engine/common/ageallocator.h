/*
** This source file is part of MY-BASIC
**
** For the latest info, see http://code.google.com/p/my-basic/
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

AGE_API Ptr age_malloc(s32 _size);
AGE_API void age_free(Ptr _ptr);

#ifndef AGE_MALLOC
#	define AGE_MALLOC(_type) ((_type*)age_malloc(sizeof(_type)))
#endif
#ifndef AGE_FREE
#	define AGE_FREE(_ptr) age_free(_ptr)
#endif

#endif /* __AGE_ALLOCATOR_H__ */
