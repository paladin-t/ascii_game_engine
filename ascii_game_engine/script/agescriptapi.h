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

#ifndef __AGE_SCRIPT_API_H__
#define __AGE_SCRIPT_API_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "my_basic/my_basic.h"

/**
 * @brief my-basic interface adapter
 */
AGE_API bl amb_register_func(mb_interpreter_t* s, const Str n, mb_func_t f);
/**
 * @brief my-basic interface adapter
 */
AGE_API bl amb_unregister_func(mb_interpreter_t* s, const Str n);

/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_attempt_func_begin(mb_interpreter_t* s, void** l);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_attempt_open_bracket(mb_interpreter_t* s, void** l);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_attempt_close_bracket(mb_interpreter_t* s, void** l);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_pop_int(mb_interpreter_t* s, void** l, int_t* val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_pop_real(mb_interpreter_t* s, void** l, real_t* val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_pop_string(mb_interpreter_t* s, void** l, char** val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_pop_value(mb_interpreter_t* s, void** l, mb_value_t* val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_push_int(mb_interpreter_t* s, void** l, int_t val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_push_real(mb_interpreter_t* s, void** l, real_t val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_push_string(mb_interpreter_t* s, void** l, char* val);
/**
 * @brief my-basic interface adapter
 */
AGE_API int amb_push_value(mb_interpreter_t* s, void** l, mb_value_t val);

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
 * @brief my-basic api: create a sprite object
 */
AGE_INTERNAL int age_api_create_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: destroy a sprite in a canvas
 */
AGE_INTERNAL int age_api_destroy_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: destroy all sprites in a canvas
 */
AGE_INTERNAL int age_api_destroy_all_sprites(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set the position of a sprite object
 */
AGE_INTERNAL int age_api_set_sprite_position(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get the x position of a sprite object
 */
AGE_INTERNAL int age_api_get_sprite_position_x(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get the y position of a sprite object
 */
AGE_INTERNAL int age_api_get_sprite_position_y(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: play an animation of a time line
 */
AGE_INTERNAL int age_api_play_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: stop an animation of a time line
 */
AGE_INTERNAL int age_api_stop_sprite(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a s32 parameter of the canvas
 */
AGE_INTERNAL int age_api_get_cvs_s32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a s32 parameter of the canvas
 */
AGE_INTERNAL int age_api_set_cvs_s32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a u32 parameter of the canvas
 */
AGE_INTERNAL int age_api_get_cvs_u32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a u32 parameter of the canvas
 */
AGE_INTERNAL int age_api_set_cvs_u32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a f32 parameter of the canvas
 */
AGE_INTERNAL int age_api_get_cvs_f32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a f32 parameter of the canvas
 */
AGE_INTERNAL int age_api_set_cvs_f32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a string parameter of the canvas
 */
AGE_INTERNAL int age_api_get_cvs_str_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a string parameter of the canvas
 */
AGE_INTERNAL int age_api_set_cvs_str_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a s32 parameter of a sprite
 */
AGE_INTERNAL int age_api_get_spr_s32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a s32 parameter of a sprite
 */
AGE_INTERNAL int age_api_set_spr_s32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a u32 parameter of a sprite
 */
AGE_INTERNAL int age_api_get_spr_u32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a u32 parameter of a sprite
 */
AGE_INTERNAL int age_api_set_spr_u32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a f32 parameter of a sprite
 */
AGE_INTERNAL int age_api_get_spr_f32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a f32 parameter of a sprite
 */
AGE_INTERNAL int age_api_set_spr_f32_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: get a string parameter of a sprite
 */
AGE_INTERNAL int age_api_get_spr_str_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: set a string parameter of a sprite
 */
AGE_INTERNAL int age_api_set_spr_str_param(mb_interpreter_t* s, void** l);

/**
 * @brief my-basic api: load saved data
 */
AGE_API void amb_load_data(const Str file);
/**
 * @brief my-basic api: save data
 */
AGE_API void amb_save_data(const Str file);

#endif /* __AGE_SCRIPT_API_H__ */
