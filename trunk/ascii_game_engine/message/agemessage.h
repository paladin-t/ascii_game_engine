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

#ifndef __AGE_MESSAGE_H__
#define __AGE_MESSAGE_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "../common/agelist.h"
#include "../common/agehashtable.h"

#define MESSAGE_TABLE_SIZE 8

/**
 * @brief enum messages
 */
typedef enum Messages {
	MSG_GET_PROP,         /**< get a property from an object */
	MSG_SET_PROP,         /**< set a property from an object */

	MSG_USER,             /**< beginning of user defined message */
} Messages;

/**
 * @brief message processing functor
 *
 * @param[in] _sender      - the sender of this message
 * @param[in] _msg         - message type
 * @param[in] _lparam      - first param
 * @param[in] _wparam      - second param
 * @param[in] _extra       - extra data
 * @return - execution status
 */
typedef s32 (* MessageProc)(Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

/**
 * @brief message processing functor mapping structure
 */
typedef struct MessageMap {
	MessageProc fastTable[MESSAGE_TABLE_SIZE]; /**< functors cached in an array for fast accessing */
	ht_node_t* procMap;                        /**< functors the array above cannot hold */
} MessageMap;

/**
 * @brief create a message map of a sprite
 *
 * @param[in] _obj - sprite object
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl create_message_map_sprite(Ptr _obj);
/**
 * @brief destroy a message map of a sprite
 *
 * @param[in] _obj - sprite object
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl destroy_message_map_sprite(Ptr _obj);

/**
 * @brief register a message processing functor to a sprite object
 *
 * @param[in] _obj  - sprite object
 * @param[in] _msg  - message type to be registered
 * @param[in] _proc - processing functor
 */
AGE_API void register_message_proc_sprite(Ptr _obj, u32 _msg, MessageProc _proc);
/**
 * @brief get a registered message processing functor of a sprite object
 *
 * @param[in] _obj  - sprite object
 * @param[in] _msg  - message type to be registered
 * @return - processing functor
 */
AGE_API MessageProc get_message_proc_sprite(Ptr _obj, u32 _msg);
/**
 * @brief unregister a message processing functor of a sprite object
 *
 * @param[in] _obj  - sprite object
 * @param[in] _msg  - message type to be registered
 */
AGE_API void unregister_message_proc_sprite(Ptr _obj, u32 _msg);

/**
 * @brief send a message to a sprite object
 *
 * @param[in] _receiver - target sprite object
 * @param[in] _sender      - the sender of the message
 * @param[in] _msg         - message type
 * @param[in] _lparam      - first param
 * @param[in] _wparam      - second param
 * @param[in] _extra       - extra data
 * @return - message sending status
 */
AGE_API s32 send_message_to_sprite(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

#endif /* __AGE_MESSAGE_H__ */
