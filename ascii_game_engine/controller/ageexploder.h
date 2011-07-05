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

#ifndef __AGE_EXPLODER_H__
#define __AGE_EXPLODER_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "../common/agelist.h"
#include "../common/agehashtable.h"

/**
 * @brief sprite explosion updating
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void update_sprite_ex(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);
/**
 * @brief fire sprite explosion rendering
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void fire_render_sprite_ex(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);
/**
 * @brief post sprite explosion rendering
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void post_render_sprite_ex(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);

#endif /* __AGE_EXPLODER_H__ */
