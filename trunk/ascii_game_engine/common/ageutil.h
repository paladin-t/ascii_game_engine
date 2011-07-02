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

#ifndef __AGE_UTIL_H__
#define __AGE_UTIL_H__

#include "../ageconfig.h"
#include "../common/agetype.h"

#ifndef _countof
#	define _countof(_a) (sizeof(_a) / sizeof(_a[0]))
#endif

/**
 * @brief get the system tick count
 *
 * @return - the system tick count, in millisecond
 */
AGE_INTERNAL u32 get_tick_count(void);
/**
 * @brief sleep for a while
 *
 * @return - time to sleep, in millisecond
 */
AGE_INTERNAL void sys_sleep(s32 _time);

/**
 * @brief read a line from a file to a string buffer
 *
 * @param[in] _fp    - file pointer
 * @param[out] _buf  - buffer to be filled
 * @return - read characters count
 */
AGE_INTERNAL s32 freadln(FILE* _fp, Str* _buf);
/**
 * @brief skip a line from a file
 *
 * @param[in] _fp    - file pointer
 * @return - skipped characters count
 */
AGE_INTERNAL s32 fskipln(FILE* _fp);

/**
 * @brief create a new string and copy the given content to it
 *
 * @param[in] _str - source string
 * @return - new created string, need to be freed manually
 */
AGE_INTERNAL Str copy_string(const Str _str);

#endif /* __AGE_UTIL_H__ */