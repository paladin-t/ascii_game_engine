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

#ifndef __AGE_SCRIPT_API_H__
#define __AGE_SCRIPT_API_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "my_basic/my_basic.h"

/**
 * @brief my-basic api: pronounce a beep
 */
AGE_INTERNAL int age_api_beep(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: register a key code mapping item
 */
AGE_INTERNAL int age_api_reg_key_code(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set the frame rate of a canvas
 */
AGE_INTERNAL int age_api_set_frame_rate(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: create a sprite object
 */
AGE_INTERNAL int age_api_create_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: destroy a sprite in a canvas
 */
AGE_INTERNAL int age_api_destroy_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: destroy all sprites in a canvas
 */
AGE_INTERNAL int age_api_destroy_all_sprites(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: set the position of a sprite object
 */
AGE_INTERNAL int age_api_set_sprite_position(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: get the x position of a sprite object
 */
AGE_INTERNAL int age_api_get_sprite_position_x(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: get the y position of a sprite object
 */
AGE_INTERNAL int age_api_get_sprite_position_y(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: play an animation of a time line
 */
AGE_INTERNAL int age_api_play_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief basic api: stop an animation of a time line
 */
AGE_INTERNAL int age_api_stop_sprite(mb_interpreter_t* s, void** l);

#endif /* __AGE_SCRIPT_API_H__ */
