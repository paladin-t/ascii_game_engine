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

#ifndef __AGE_CONTROLLER_H__
#define __AGE_CONTROLLER_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "../common/agelist.h"
#include "../common/agehashtable.h"

/**
 * @brief object controlling functor
 *
 * @param[in] _obj         - object to be controlled
 * @param[in] _name        - object name
 * @param[in] _elapsedTime - elapsed time since last frame
 * @param[in] _lparam      - first param
 * @param[in] _wparam      - second param
 * @param[in] _extra       - extra data
 * @return - execution status
 */
typedef s32 (* control_proc)(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra);

/**
 * @brief set a controller of a canvas
 *
 * @param[in] _obj  - canvas object
 * @param[in] _proc - controller
 */
AGE_API void set_canvas_controller(Ptr _obj, control_proc _proc);
/**
 * @brief get a controller of a canvas
 *
 * @param[in] _obj - canvas object
 * @return controller
 */
AGE_API control_proc get_canvas_controller(Ptr _obj);

/**
 * @brief set a controller of a sprite
 *
 * @param[in] _obj  - sprite object
 * @param[in] _proc - controller
 */
AGE_API void set_sprite_controller(Ptr _obj, control_proc _proc);
/**
 * @brief get a controller of a sprite
 *
 * @param[in] _obj - sprite object
 * @return controller
 */
AGE_API control_proc get_sprite_controller(Ptr _obj);

#endif /* __AGE_CONTROLLER_H__ */
