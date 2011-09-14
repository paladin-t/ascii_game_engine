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

#include "../common/ageallocator.h"
#include "../common/ageutil.h"
#include "../render/agerenderer.h"
#include "agemessage.h"

static MessageMap* _tempMsgMap = 0;

static s32 _ht_cmp_msg(Ptr d1, Ptr d2) {
	u32 i1 = (u32)d1;
	u32 i2 = (u32)d2;
	s32 i = (s64)i1 - (s64)i2;
	s32 result = 0;
	if(i < 0) {
		result = -1;
	} else if(i > 0) {
		result = 1;
	}

	return result;
}

static s32 _copy_message_map(Ptr _data, Ptr _extra) {
	s32 result = 0;
	message_proc proc = 0;
	Str str = _extra;
	union { Ptr ptr; u32 uint; } u;

	assert(_tempMsgMap);

	proc = (message_proc)_data;
	u.ptr = _extra;
	register_message_proc(_tempMsgMap, u.uint, proc);

	return result;
}

bl create_sprite_message_map(Ptr _obj) {
	bl result = TRUE;
	Sprite* spr = (Sprite*)_obj;

	assert(_obj);

	if(spr->message_map.proc_map) {
		result = FALSE;
	} else {
		spr->message_map.proc_map = ht_create(0, _ht_cmp_msg, ht_hash_ptr, 0);
	}

	return result;
}

bl create_canvas_message_map(Ptr _obj) {
	bl result = TRUE;
	Canvas* cvs = (Canvas*)_obj;

	assert(_obj);

	if(cvs->message_map.proc_map) {
		result = FALSE;
	} else {
		cvs->message_map.proc_map = ht_create(0, _ht_cmp_msg, ht_hash_ptr, 0);
	}

	return result;
}

bl destroy_sprite_message_map(Ptr _obj) {
	bl result = TRUE;
	Sprite* spr = (Sprite*)_obj;

	assert(_obj);

	if(!spr->message_map.proc_map) {
		result = FALSE;
	} else {
		ht_destroy(spr->message_map.proc_map);
		spr->message_map.proc_map = 0;
	}

	return result;
}

bl destroy_canvas_message_map(Ptr _obj) {
	bl result = TRUE;
	Canvas* cvs = (Canvas*)_obj;

	assert(_obj);

	if(!cvs->message_map.proc_map) {
		result = FALSE;
	} else {
		ht_destroy(cvs->message_map.proc_map);
		cvs->message_map.proc_map = 0;
	}

	return result;
}

message_proc get_message_map_message_proc(MessageMap* _msgMap, u32 _msg) {
	message_proc result = 0;
	ht_node_t* pm = 0;
	union { Ptr ptr; u32 uint; } u;

	assert(_msgMap);

	if(_msg < MESSAGE_TABLE_SIZE) {
		result = _msgMap->fast_table[_msg];
	} else {
		pm = _msgMap->proc_map;
		u.uint = _msg;
		ht_get(pm, u.ptr, (Ptr*)&result);
	}

	return result;
}

message_proc get_sprite_message_proc(Ptr _obj, u32 _msg) {
	message_proc result = 0;
	Sprite* spr = (Sprite*)_obj;

	assert(_obj);

	result = get_message_map_message_proc(&spr->message_map, _msg);

	return result;
}

message_proc get_canvas_message_proc(Ptr _obj, u32 _msg) {
	message_proc result = 0;
	Canvas* cvs = (Canvas*)_obj;

	assert(_obj);

	result = get_message_map_message_proc(&cvs->message_map, _msg);

	return result;
}

void register_message_proc(MessageMap* _map, u32 _msg, message_proc _proc) {
	ht_node_t* pm = 0;
	union { Ptr ptr; u32 uint; } u;

	assert(_map);

	if(_msg < MESSAGE_TABLE_SIZE) {
		_map->fast_table[_msg] = _proc;
	} else {
		pm = _map->proc_map;
		u.uint = _msg;
		ht_set_or_insert(pm, u.ptr, _proc);
	}
}

void unregister_message_proc(MessageMap* _map, u32 _msg) {
	ht_node_t* pm = 0;
	ls_node_t* p = 0;
	union { Ptr ptr; u32 uint; } u;

	assert(_map);

	if(_msg < MESSAGE_TABLE_SIZE) {
		_map->fast_table[_msg] = 0;
	} else {
		pm = _map->proc_map;
		u.uint = _msg;
		p = ht_find(pm, u.ptr);
		if(p) {
			ht_remove(pm, p->extra);
		}
	}
}

void copy_message_map(MessageMap* _src, MessageMap* _tgt) {
	memcpy(_tgt->fast_table, _src->fast_table, sizeof(_src->fast_table));
	_tempMsgMap = _tgt;
	ht_foreach(_src->proc_map, _copy_message_map);
	_tempMsgMap = 0;
}

s32 send_message_to_proc(message_proc _func, Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	result = _func(_receiver, _sender, _msg, _lparam, _wparam, _extra);

	return result;
}

s32 send_message_to_object(MessageReceiver* _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	message_proc proc = 0;

	assert(_receiver && _receiver->receiver && _receiver->message_map);

	proc = get_message_map_message_proc(_receiver->message_map, _msg);
	if(proc) {
		result = proc(_receiver->receiver, _sender, _msg, _lparam, _wparam, _extra);
	}

	return result;
}

s32 send_message_to_sprite(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	message_proc proc = 0;
	
	proc = get_sprite_message_proc(_receiver, _msg);
	if(proc) {
		result = proc(_receiver, _sender, _msg, _lparam, _wparam, _extra);
	}

	return result;
}

s32 send_message_to_canvas(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	message_proc proc = 0;
	
	proc = get_canvas_message_proc(_receiver, _msg);
	if(proc) {
		result = proc(_receiver, _sender, _msg, _lparam, _wparam, _extra);
	}

	return result;
}
