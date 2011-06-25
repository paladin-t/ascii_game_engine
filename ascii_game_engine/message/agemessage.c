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

#include "../common/ageallocator.h"
#include "../common/ageutil.h"
#include "../render/agerenderer.h"
#include "agemessage.h"

bl create_message_map_sprite(Ptr _obj) {
	bl result = TRUE;
	Sprite* spr = (Sprite*)_obj;

	assert(_obj);

	if(spr->messageMap.procMap) {
		result = FALSE;
	} else {
		spr->messageMap.procMap = ht_create(0, ht_cmp_ptr, ht_hash_ptr, 0);
	}

	return result;
}

bl destroy_message_map_sprite(Ptr _obj) {
	bl result = TRUE;
	Sprite* spr = (Sprite*)_obj;

	assert(_obj);

	if(!spr->messageMap.procMap) {
		result = FALSE;
	} else {
		ht_destroy(spr->messageMap.procMap);
		spr->messageMap.procMap = 0;
	}

	return result;
}

void register_message_proc_sprite(Ptr _obj, u32 _msg, MessageProc _proc) {
	Sprite* spr = (Sprite*)_obj;
	ht_node_t* pm = 0;
	union { Ptr ptr; u32 uint; } u;

	assert(_obj);

	if(_msg < MESSAGE_TABLE_SIZE) {
		spr->messageMap.fastTable[_msg] = _proc;
	} else {
		pm = spr->messageMap.procMap;
		u.uint = _msg;
		ht_set_or_insert(pm, u.ptr, _proc);
	}
}

MessageProc get_message_proc_sprite(Ptr _obj, u32 _msg) {
	MessageProc result = 0;
	Sprite* spr = (Sprite*)_obj;
	ht_node_t* pm = 0;
	union { Ptr ptr; u32 uint; } u;

	assert(_obj);

	if(_msg < MESSAGE_TABLE_SIZE) {
		result = spr->messageMap.fastTable[_msg];
	} else {
		pm = spr->messageMap.procMap;
		u.uint = _msg;
		ht_get(pm, u.ptr, (Ptr*)&result);
	}

	return result;
}

void unregister_message_proc_sprite(Ptr _obj, u32 _msg) {
	Sprite* spr = (Sprite*)_obj;
	ht_node_t* pm = 0;
	union { Ptr ptr; u32 uint; } u;

	assert(_obj);

	if(_msg < MESSAGE_TABLE_SIZE) {
		spr->messageMap.fastTable[_msg] = 0;
	} else {
		pm = spr->messageMap.procMap;
		u.uint = _msg;
		ht_set_or_insert(pm, u.ptr, 0);
	}
}

s32 send_message_to_sprite(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	MessageProc proc = 0;
	
	proc = get_message_proc_sprite(_receiver, _msg);
	if(proc) {
		proc(_sender, _msg, _lparam, _wparam, _extra);
	}

	return result;
}
