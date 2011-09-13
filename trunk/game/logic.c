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
#include "animation.h"
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

static AsciiHeroBoardType _generate_board_type(void) {
	AsciiHeroBoardType result = AHBT_SOLID;
	s32 probMax = 0;
	s32 prob = 0;
	s32 i = 0;

	probMax = BOARD_INFO[_countof(BOARD_INFO) - 1].probMax;
	prob = age_rand(0, probMax);
	for(i = 0; i < _countof(BOARD_INFO); ++i) {
		if(prob >= BOARD_INFO[i].probMin && prob <= BOARD_INFO[i].probMax) {
			result = BOARD_INFO[i].type;
			break;
		}
	}

	return result;
}

static Sprite* _add_board_by_type(AsciiHeroBoardType _type) {
	Sprite* result = 0;
	static u32 boardId = 0;
	s8 newName[AGE_STR_LEN];

	assert(_type >= 0 && _type < AHBT_COUNT);
	++game()->boardCount;
	if(game()->boardCount > game()->boardPoolSize) {
		game()->boardPoolSize = game()->boardCount + 8;
		game()->boardPool = AGE_REALLOC_N(Sprite*, game()->boardPool, game()->boardPoolSize);
	}
	sprintf(newName, "board_%u", boardId++);
	result = clone_sprite(AGE_CVS, game()->boardTemplate->name, newName);
	result->userdata.data = create_board_userdata();
	result->userdata.destroy = destroy_board_userdata;
	game()->boardPool[game()->boardCount - 1] = result;
	register_message_proc(&result->messageMap, MSG_BOARD_UP, on_msg_proc_for_sprite_board_up);
	play_sprite(
		AGE_CVS,
		result,
		BOARD_INFO[_type].startFrame,
		BOARD_INFO[_type].endFrame,
		TRUE,
		on_playing_for_sprite_board
	);
	pause_sprite(AGE_CVS, result);

	return result;
}

static s32 _drop_board(Sprite* _spr) {
	s32 result = 0;

	BoardUserdata* ud = (BoardUserdata*)_spr->userdata.data;
	assert(ud);
	ud->drop = TRUE;

	return result;
}

static s32 _remove_board(Sprite* _spr) {
	s32 result = 0;
	s32 i = 0;
	Sprite* b = 0;

	for(i = 0; i < game()->boardCount; ++i) {
		b = game()->boardPool[i];
		if(b == _spr) {
			--game()->boardCount;
			if(game()->boardCount) {
				game()->boardPool[i] = game()->boardPool[game()->boardCount];
			} else {
				AGE_FREE_N(game()->boardPool);
			}
			destroy_sprite(AGE_CVS, b);
			++result;
			break;
		}
	}

	return result;
}

static s32 _clear_board(void) {
	s32 result = 0;
	s32 i = 0;
	Sprite* b = 0;

	for(i = 0; i < game()->boardCount; ++i) {
		b = game()->boardPool[i];
		game()->boardPool[i] = 0;
		destroy_sprite(AGE_CVS, b);
		++result;
	}
	if(game()->boardPool) {
		game()->boardCount = 0;
		game()->boardPoolSize = 0;
		AGE_FREE_N(game()->boardPool);
	}

	return result;
}

static void _create_score_boards(void) {
	s32 i = 0;
	s8 n[AGE_STR_LEN];
	for(i = 0; i < SCORE_BOARD_SIZE; ++i) {
		sprintf(n, "score_cell_%d", i);
		game()->scoreBoard[i] = create_sprite(
			AGE_CVS,
			n,
			"data/ui/number_shape.txt",
			"data/ui/number_brush.txt",
			"data/ui/number_palete.txt"
		);
		play_sprite(AGE_CVS, game()->scoreBoard[i], "s0", "e0", FALSE, 0);
		set_sprite_position(AGE_CVS, game()->scoreBoard[i], GAME_AREA_WIDTH + (SCORE_BOARD_SIZE - 1 - i) * 4 + 2, 1);
		set_sprite_physics_mode(AGE_CVS, game()->scoreBoard[i], PHYSICS_MODE_NULL);
		set_sprite_visible(AGE_CVS, game()->scoreBoard[i], FALSE);
	}
}

static void _destroy_score_boards(void) {
	s32 i = 0;
	for(i = 0; i < SCORE_BOARD_SIZE; ++i) {
		destroy_sprite(AGE_CVS, game()->scoreBoard[i]);
	}
	memset(game()->scoreBoard, 0, sizeof(game()->scoreBoard));
}

static void _set_score_board_value(u32 score) {
	s32 i = 0;
	s32 j = 0;
	s32 n = 0;
	s8 sf[AGE_STR_LEN];
	s8 ef[AGE_STR_LEN];
	while(score) {
		n = score % 10;
		sprintf(sf, "s%d", n);
		sprintf(ef, "e%d", n);
		assert(i >= 0 && i < _countof(game()->scoreBoard));
		play_sprite(AGE_CVS, game()->scoreBoard[i], sf, ef, FALSE, 0);
		++i;
		score /= 10;
	}
	for(j = i; j < SCORE_BOARD_SIZE; ++j) {
		assert(j >= 0 && j < _countof(game()->scoreBoard));
		play_sprite(AGE_CVS, game()->scoreBoard[j], "s0", "e0", FALSE, 0);
	}
}

static void _set_score_board_visible(bl vis) {
	s32 i = 0;
	for(i = 0; i < SCORE_BOARD_SIZE; ++i) {
		set_sprite_visible(AGE_CVS, game()->scoreBoard[i], vis);
	}
}

void solid_board_action(Canvas* _cvs, Sprite* _spr) {
	/* do nothing */
}

void spring_board_action(Canvas* _cvs, Sprite* _spr) {
	assert(_cvs && _spr);

	send_message_to_sprite(game()->main, _spr, MSG_JUMP, 0, 0, 0);
}

void serration_board_action(Canvas* _cvs, Sprite* _spr) {
	game()->game_over = TRUE;
}

void overturn_board_action(Canvas* _cvs, Sprite* _spr) {
	BoardUserdata* bu = 0;
	static const s32 OVERTURN_TIME = 300;

	assert(_cvs && _spr);

	bu = (BoardUserdata*)(_spr->userdata.data);
	if(bu->time >= OVERTURN_TIME) {
		set_sprite_physics_mode(_cvs, _spr, PHYSICS_MODE_NULL);
	}
}

void lscroll_board_action(Canvas* _cvs, Sprite* _spr) {
	BoardUserdata* bu = 0;
	static const s32 SCROLL_TIME = 100;

	assert(_cvs && _spr);

	bu = (BoardUserdata*)(_spr->userdata.data);
	if(bu->time >= SCROLL_TIME) {
		bu->time -= SCROLL_TIME;
		send_message_to_sprite(game()->main, 0, MSG_MOVE, DIR_LEFT, 0, 0);
	}
}

void rscroll_board_action(Canvas* _cvs, Sprite* _spr) {
	BoardUserdata* bu = 0;
	static const s32 SCROLL_TIME = 100;

	assert(_cvs && _spr);

	bu = (BoardUserdata*)(_spr->userdata.data);
	bu = (BoardUserdata*)(_spr->userdata.data);
	if(bu->time >= SCROLL_TIME) {
		bu->time -= SCROLL_TIME;
		send_message_to_sprite(game()->main, 0, MSG_MOVE, DIR_RIGHT, 0, 0);
	}
}

void init(void) {
	game()->main = create_sprite(
		AGE_CVS,
		"ascii_hero_go_logo",
		"data/ui/logo_shape.txt",
		"data/ui/logo_brush.txt",
		"data/ui/logo_palete.txt"
	);
	game()->subsidiary = create_sprite(
		AGE_CVS,
		"copyright",
		"data/ui/copyright_shape.txt",
		"data/ui/copyright_brush.txt",
		"data/ui/copyright_palete.txt"
	);
	set_sprite_visible(AGE_CVS, game()->main, FALSE);
	set_sprite_visible(AGE_CVS, game()->subsidiary, FALSE);

	set_canvas_controller(AGE_CVS, state_show_splash);

	game()->generate_board_type = _generate_board_type;
	game()->add_board_by_type = _add_board_by_type;
	game()->drop_board = _drop_board;
	game()->remove_board = _remove_board;
	game()->clear_board = _clear_board;
	game()->create_score_boards = _create_score_boards;
	game()->destroy_score_boards = _destroy_score_boards;
	game()->set_score_board_value = _set_score_board_value;
	game()->set_score_board_visible = _set_score_board_visible;

	game()->time = 0;
	game()->lineUpTime = DEFAULT_LINE_UP_TIME;
	game()->lineCount = 0;
	game()->levelDistance = DEFAULT_LEVEL_DISTANCE;
	game()->levelCount = 0;
	game()->levelGenerated = FALSE;

	register_message_proc(&AGE_CVS->messageMap, MSG_BOARD_UP, on_msg_proc_for_canvas);
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
	result->fsm = create_bitfsm(3, 4, fsm_tag_to_index, fsm_tag_to_command, 0, fsm_step_handler, destroy_fsm_tag);

	return result;
}

void destroy_player_userdata(Ptr _ptr) {
	PlayerUserdata* ud = (PlayerUserdata*)_ptr;
	assert(ud);
	destroy_bitfsm(ud->fsm);
	AGE_FREE(ud);
}

s32 on_ctrl_for_sprite_main_player(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	game()->main->direction = 0;
	if(is_key_down(AGE_IPT, 0, KC_LEFT)) {
		send_message_to_sprite(game()->main, 0, MSG_MOVE, DIR_LEFT, 0, 0);
	} else if(is_key_down(AGE_IPT, 0, KC_RIGHT)) {
		send_message_to_sprite(game()->main, 0, MSG_MOVE, DIR_RIGHT, 0, 0);
	}

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

s32 on_playing_for_sprite_main_player(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx) {
	s32 result = 0;

	return result;
}

s32 on_playing_for_sprite_board(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx) {
	s32 result = 0;

	return result;
}

void on_removing_for_sprite_main_player(Ptr _handlerObj, Canvas* _cvs, Sprite* _spr) {
}

void on_removing_for_sprite_board(Ptr _handlerObj, Canvas* _cvs, Sprite* _spr) {
}

void on_collide_for_sprite_main_player(Canvas* _cvs, Sprite* _spr, s32 _px, s32 _py) {
	Pixel* pixelc = 0;
	PlayerUserdata* ud = 0;
	Sprite* bd = 0;
	s32 i = 0;
	s32 k = 0;
	s32 x = 0;
	s32 y = 0;
	s32 fx = 0;
	s32 fy = 0;
	s8 b = 0;
	bl ob = FALSE;

	assert(_px >= 0 && _px < CANVAS_WIDTH && _py >= 0 && _py < CANVAS_HEIGHT);

	fx = _px - _spr->position.x;
	fy = _py - _spr->position.y;
	k = _spr->timeLine.currentFrame;
	b = _spr->timeLine.frames[k].tex[fx + fy * _spr->frameSize.w].brush;

	pixelc = &_cvs->pixels[_px + _py * _cvs->size.w];
	ud = (PlayerUserdata*)(_spr->userdata.data);
	if(ud->onBoard[0]) {
		return;
	}
	for(i = 0; i < pixelc->frameCount; ++i) {
		bd = pixelc->ownerFrames[i]->parent;
		if(bd != _spr) {
			if(b == game()->footBrush) {
				assert(strlen(_spr->name) + 1 < _countof(ud->onBoard));
				sprintf(ud->onBoard, bd->name);
				ob = TRUE;
				if(_px + bd->position.x <= _spr->position.x) {
					ud->collitionDirection = -1;
				} else if(_px >= _spr->position.x + _spr->frameSize.w) {
					ud->collitionDirection = 1;
				} else {
					ud->collitionDirection = 0;
				}
				break;
			}
		} else {
			if(pixelc->frameCount == 1) {
				ob = TRUE;
			}
		}
	}

	if(!ob) {
		if(ud->collitionDirection && _spr->direction && ud->collitionDirection == _spr->direction) {
			set_sprite_position(_cvs, _spr, _spr->lastFramePosition.x, _spr->position.y);
		}
	}
}

void on_collide_for_sprite_board(Canvas* _cvs, Sprite* _spr, s32 _px, s32 _py) {
	Pixel* pixelc = 0;
	BoardUserdata* bu = 0;
	Sprite* bd = 0;
	s32 i = 0;

	assert(_cvs && _spr);

	pixelc = &_cvs->pixels[_px + _py * _cvs->size.w];
	for(i = 0; i < pixelc->frameCount; ++i) {
		bd = pixelc->ownerFrames[i]->parent;
		if(bd != _spr) {
			bu = (BoardUserdata*)(_spr->userdata.data);
			if(_spr->timeLine.pause) {
				bu->collition = TRUE;
				resume_sprite(_cvs, _spr);
				break;
			}
		}
	}
}

void on_update_for_sprite_main_player(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	PlayerUserdata* ud = 0;
	Sprite* bd = 0;
	s32 x = 0;
	s32 y = 0;
	s32 bx = 0;
	s32 by = 0;
	s32 l = 0;

	assert(_cvs && _spr);

	get_sprite_position(_cvs, _spr, &x, &y);
	ud = (PlayerUserdata*)(_spr->userdata.data);
	assert(ud);

	l = game()->lineCount - GAME_AREA_HEIGHT + y + _spr->frameSize.h;
	if(l > 0) {
		l /= game()->levelDistance;
		if(l > (s32)game()->levelCount) {
			game()->levelCount = l;
			game()->set_score_board_value(l);
		}
	}

	if(ud->jumpLine) {
		ud->jumpTime += _elapsedTime;
		if(ud->jumpTime >= DEFAULT_JUMP_TIME) {
			ud->jumpTime -= DEFAULT_JUMP_TIME;
			--ud->jumpLine;
			--y;
			set_sprite_position(_cvs, _spr, x, y);
		}
	} else {
		ud->time += _elapsedTime;
		if(ud->time >= ud->fallTime) {
			ud->time -= ud->fallTime;
			if(!ud->onBoard[0]) {
				++y;
				set_sprite_position(_cvs, _spr, x, y);
				if(y > GAME_AREA_BOTTOM) {
					game()->game_over = TRUE;
				}
			}
		}

		if(ud->onBoard[0]) {
			bd = get_sprite_by_name(_cvs, ud->onBoard);
			if(bd) {
				get_sprite_position(_cvs, bd, &bx, &by);
				by = by - _spr->frameSize.h + 1;
				set_sprite_position(_cvs, _spr, x, by);
				if(by + _spr->frameSize.h <= GAME_AREA_TOP) {
					game()->game_over = TRUE;
				}
			}
		}
	}
	ud->onBoard[0] = '\0';

	update_sprite(_cvs, _spr, _elapsedTime);
}

void on_update_for_sprite_board(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime) {
	BoardUserdata* bu = 0;

	assert(_cvs && _spr);

	bu = (BoardUserdata*)(_spr->userdata.data);
	if(bu) {
		if(bu->collition) {
			bu->collition = FALSE;
			bu->time += _elapsedTime;
			if(BOARD_INFO[bu->type].action) {
				BOARD_INFO[bu->type].action(_cvs, _spr);
			}
		} else if(!_spr->timeLine.pause) {
			pause_sprite(_cvs, _spr);
		}
	}

	update_sprite(_cvs, _spr, _elapsedTime);
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
			spr->direction = -1;
			--x;
			if(x <= GAME_AREA_LEFT) {
				x = GAME_AREA_LEFT + 1;
			}
			break;
		case DIR_RIGHT:
			spr->direction = 1;
			++x;
			if(x + spr->frameSize.w - 1 >= GAME_AREA_RIGHT) {
				x = GAME_AREA_RIGHT - spr->frameSize.w;
			}
			break;
		default:
			assert(0 && "Unknown direction");
			break;
	}
	set_sprite_position(spr->owner, spr, x, y);

	return result;
}

s32 on_msg_proc_for_sprite_main_player_jump(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	Sprite* spr = 0;
	PlayerUserdata* ud = 0;

	spr = (Sprite*)_receiver;
	ud = (PlayerUserdata*)(spr->userdata.data);
	ud->jumpLine = DEFAULT_JUMP_LINE;
	ud->jumpTime = 0;

	return result;
}
