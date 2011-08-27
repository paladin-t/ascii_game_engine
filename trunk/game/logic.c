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

#include "logic.h"
#include "game.h"
#include "state.h"
#include "renderer.h"

typedef struct MessageContext {
	Ptr receiver;
	Ptr sender;
	u32 msg;
	u32 lparam;
	u32 wparam;
	Ptr extra;
} MessageContext;

static MessageContext lastMsg;

static s32 _broadcast_message_to_sprite(Ptr _data, Ptr _extra) {
	s32 result = 0;

	Sprite* spr = (Sprite*)_data;
	send_message_to_sprite(spr, lastMsg.receiver, lastMsg.msg, lastMsg.lparam, lastMsg.wparam, lastMsg.extra);

	return result;
}

BoardUserdata* create_board_userdata(void) {
	BoardUserdata* result = 0;

	result = AGE_MALLOC(BoardUserdata);

	return result;
}

void destroy_board_userdata(Ptr _ptr) {
	BoardUserdata* ud = (BoardUserdata*)_ptr;
	assert(ud);
	AGE_FREE(ud);
}

PlayerUserdata* create_player_userdata(void) {
	PlayerUserdata* result = 0;

	result = AGE_MALLOC(PlayerUserdata);

	return result;
}

void destroy_player_userdata(Ptr _ptr) {
	PlayerUserdata* ud = (PlayerUserdata*)_ptr;
	assert(ud);
	AGE_FREE(ud);
}

s32 on_ctrl_for_sprite_main_player(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 on_ctrl_for_sprite_board(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 on_msg_proc_for_canvas(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	Canvas* cvs = (Canvas*)_receiver;
	BoardUserdata* ud = 0;
	Sprite* b = 0;
	s32 i = 0;

	lastMsg.receiver = _receiver;
	lastMsg.sender = _sender;
	lastMsg.msg = _msg;
	lastMsg.lparam = _lparam;
	lastMsg.wparam = _wparam;
	lastMsg.extra = _extra;

	assert(cvs && cvs->sprites);

	switch(_msg) {
		case MSG_BOARD_UP:
			ht_foreach(cvs->sprites, _broadcast_message_to_sprite);
			for(i = 0; i < game()->boardCount; ++i) {
				b = game()->boardPool[i];
				ud = (BoardUserdata*)b->userdata.data;
				assert(ud);
				if(ud->drop) {
					game()->remove_board(b);
				}
			}
			break;
	}

	return result;
}

s32 on_msg_proc_for_sprite_main_player(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 on_msg_proc_for_sprite_board(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 on_playing_for_sprite_main_player(struct Canvas* _cvs, struct Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx) {
	s32 result = 0;

	return result;
}

s32 on_playing_for_sprite_board(struct Canvas* _cvs, struct Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx) {
	s32 result = 0;

	return result;
}

void on_removing_for_sprite_main_player(Ptr _handlerObj, struct Canvas* _cvs, struct Sprite* _spr) {
}

void on_removing_for_sprite_board(Ptr _handlerObj, struct Canvas* _cvs, struct Sprite* _spr) {
}

void on_collide_for_sprite_main_player(struct Canvas* _cvs, struct Sprite* _spr, s32 _px, s32 _py) {
}

void on_collide_for_sprite_board(struct Canvas* _cvs, struct Sprite* _spr, s32 _px, s32 _py) {
}

void on_update_for_sprite_main_player(struct Canvas* _cvs, struct Sprite* _spr, s32 _elapsedTime) {
	PlayerUserdata* ud = 0;
	s32 x = 0;
	s32 y = 0;

	assert(_cvs && _spr);

	ud = (PlayerUserdata*)(_spr->userdata.data);
	assert(ud);
	ud->time += _elapsedTime;
	if(ud->time >= ud->fallTime) {
		ud->time -= ud->fallTime;
		get_sprite_position(_cvs, _spr, &x, &y);
		++y;
		set_sprite_position(_cvs, _spr, x, y);
		if(y > GAME_AREA_BOTTOM) {
			// TODO
		}
	}
}

void on_update_for_sprite_board(struct Canvas* _cvs, struct Sprite* _spr, s32 _elapsedTime) {
}

s32 on_msg_proc_for_sprite_board_up(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	Sprite* spr = (Sprite*)_receiver;
	Canvas* cvs = (Canvas*)_sender;
	s32 x = 0;
	s32 y = 0;

	assert(_msg == MSG_BOARD_UP);

	get_sprite_position(cvs, spr, &x, &y);
	--y;
	if(y < -spr->frameSize.h) {
		game()->drop_board(spr);
	} else {
		set_sprite_position(cvs, spr, x, y);
	}

	return result;
}

s32 on_msg_proc_for_sprite_main_player_move(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	Sprite* spr = (Sprite*)_receiver;
	s32 x = 0;
	s32 y = 0;

	assert(_msg == MSG_MOVE);

	get_sprite_position(spr->owner, spr, &x, &y);
	switch(_lparam) {
		case DIR_LEFT:
			break;
		case DIR_RIGHT:
			break;
		default:
			assert("Unknow direction");
			break;
	}
	// TODO

	return result;
}
