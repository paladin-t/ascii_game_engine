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

s32 ctrl_for_sprite_main_player(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 ctrl_for_sprite_board(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 common_msg_proc_for_canvas(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 common_msg_proc_for_sprite_main_player(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}

s32 common_msg_proc_for_sprite_board(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
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
}

void on_update_for_sprite_board(struct Canvas* _cvs, struct Sprite* _spr, s32 _elapsedTime) {
}
