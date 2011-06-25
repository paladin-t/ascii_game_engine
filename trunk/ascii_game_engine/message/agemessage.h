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

typedef enum {
	MSG_GET_PROP,
	MSG_SET_PROP,

	MSG_USER,
} MESSAGES;

typedef s32 (* MessageProc)(Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

typedef struct {
	MessageProc fastTable[MESSAGE_TABLE_SIZE];
	ht_node_t* procMap;
} MessageMap;

AGE_API bl create_message_map_sprite(Ptr _obj);
AGE_API bl destroy_message_map_sprite(Ptr _obj);

AGE_API void register_message_proc_sprite(Ptr _obj, u32 _msg, MessageProc _proc);
AGE_API MessageProc get_message_proc_sprite(Ptr _obj, u32 _msg);
AGE_API void unregister_message_proc_sprite(Ptr _obj, u32 _msg);

AGE_API s32 send_message_to_sprite(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

#endif /* __AGE_MESSAGE_H__ */
