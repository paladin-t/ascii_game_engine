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

#ifndef __AGE_INPUT_H__
#define __AGE_INPUT_H__

#include "../ageconfig.h"
#include "../common/agetype.h"

#define MAX_PLAYER_COUNT 2

/**
 * @brief enum of key map
 */
typedef enum KeyIndex {
	KC_UP,    /**< up key */
	KC_DOWN,  /**< down key */
	KC_LEFT,  /**< left key */
	KC_RIGHT, /**< right key */
	KC_OK,    /**< ok/confirm/fire/enter key */
	KC_ESC,   /**< escape/cancel key */
	KC_P,     /**< 'p' key */

	KC_COUNT  /**< mapped keys count */
} KeyIndex;

/**
 * @brief open input system
 *
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl open_input(void);
/**
 * @brief close input system
 *
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl close_input(void);

/**
 * @brief create an input context
 *
 * @return - created input context
 */
AGE_API Ptr create_input_context(void);
/**
 * @brief destroy an input context
 *
 * @param[in] _ctx - input context to be destroyd
 */
AGE_API void destroy_input_context(Ptr _ctx);

/**
 * @brief update an input context
 *
 * @param[in] _ctx - input context to be updated
 */
AGE_API void update_input_context(Ptr _ctx);

/**
 * @brief register a key code as an index in the map
 *
 * @param[in] _player  - player index
 * @param[in] _keyIdx  - key index
 * @param[in] _keyCode - key code
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl register_key_map(s32 _player, KeyIndex _keyIdx, s32 _keyCode);

/**
 * @brief determine whether the given index key of a player is down
 *
 * @param[in] _ctx - input context to be detected
 * @param[in] _player  - player index
 * @param[in] _keyIdx  - key index
 * @return - return TRUE if down
 */
AGE_API bl is_key_down(Ptr _ctx, s32 _player, KeyIndex _keyIdx);

#endif /* __AGE_INPUT_H__ */
