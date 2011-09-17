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

#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "age.h"

#define DEFAULT_LINE_UP_TIME 500
#define DEFAULT_LEVEL_DISTANCE 6
#define DEFAULT_FALL_TIME 100

#define SCORE_BOARD_SIZE 4

#define DEFAULT_JUMP_LINE 3
#define DEFAULT_JUMP_TIME 50

typedef enum AsciiHeroBoardType {
	AHBT_SOLID,
	AHBT_SPRING,
	AHBT_SERRATION,
	AHBT_OVERTURN,
	AHBT_L_SCROLL,
	AHBT_R_SCROLL,
	AHBT_COUNT,
} AsciiHeroBoardType;

typedef enum AsciiHeroAudioTypes {
	AHAT_LOGO,
	AHAT_TICK,
	AHAT_COLLIDE,
	AHAT_GAME_OVER,
	AHAT_COUNT,
} AsciiHeroAudioTypes;

typedef struct AsciiHeroGame {
	Str audio[AHAT_COUNT];
	Sprite* main;
	Sprite* subsidiary;
	Sprite* board_template;
	Sprite** board_pool;
	s32 board_pool_size;
	s32 board_count;
	Sprite* score_board[SCORE_BOARD_SIZE];
	bl game_over;
	s32 time;
	s32 line_up_time;
	u32 line_count;
	u32 level_distance;
	u32 level_count;
	bl level_generated;
	s8 foot_brush;
	AsciiHeroBoardType (* generate_board_type)(void);
	Sprite* (* add_board_by_type)(AsciiHeroBoardType _type);
	s32 (* drop_board)(Sprite* _spr);
	s32 (* remove_board)(Sprite* _spr);
	s32 (* clear_board)(void);
	void (* create_score_boards)(void);
	void (* destroy_score_boards)(void);
	void (* set_score_board_value)(u32 score);
	void (* set_score_board_visible)(bl vis);
} AsciiHeroGame;

typedef enum Direction {
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
} Direction;

typedef enum UserMessages {
	MSG_USER_BEGIN = MSG_USER + 1,
	MSG_BOARD_UP,
	MSG_MOVE,
	MSG_JUMP,
}UserMessages;

typedef struct BoardUserdata {
	u32 type;
	bl drop;
	bl collition;
	s32 time;
} BoardUserdata;

typedef struct PlayerUserdata {
	s32 time;
	s32 fall_time;
	s32 jump_line;
	s32 jump_time;
	s8 on_board[AGE_STR_LEN];
	s32 collition_direction;
	u32 hold_step_count;
	Fsm* fsm;
} PlayerUserdata;

typedef void (* BoardAction)(Canvas* _cvs, Sprite* _spr);
typedef struct BoardInfo {
	const Str start_frame;
	const Str end_frame;
	struct {
		s32 prob_min;
		s32 prob_max;
		AsciiHeroBoardType type;
	};
	BoardAction action;
} BoardInfo;

void solid_board_action(Canvas* _cvs, Sprite* _spr);
void spring_board_action(Canvas* _cvs, Sprite* _spr);
void serration_board_action(Canvas* _cvs, Sprite* _spr);
void overturn_board_action(Canvas* _cvs, Sprite* _spr);
void lscroll_board_action(Canvas* _cvs, Sprite* _spr);
void rscroll_board_action(Canvas* _cvs, Sprite* _spr);

static const BoardInfo BOARD_INFO[AHBT_COUNT] = {
	{ "solid", "end_solid", 0, 29, AHBT_SOLID, solid_board_action },
	{ "spring", "end_spring", 30, 39, AHBT_SPRING, spring_board_action },
	{ "serration", "end_serration", 40, 54, AHBT_SERRATION, serration_board_action },
	{ "overturn", "end_overturn", 55, 69, AHBT_OVERTURN, overturn_board_action },
	{ "l-scroll", "end_l-scroll", 70, 79, AHBT_L_SCROLL, lscroll_board_action },
	{ "r-scroll", "end_r-scroll", 80, 89, AHBT_R_SCROLL, rscroll_board_action },
};

void init(void);

BoardUserdata* create_board_userdata(void);
void destroy_board_userdata(Ptr _ptr);

PlayerUserdata* create_player_userdata(void);
void destroy_player_userdata(Ptr _ptr);

s32 on_ctrl_for_sprite_main_player(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_ctrl_for_sprite_board(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_msg_proc_for_canvas(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_msg_proc_for_sprite_main_player(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_msg_proc_for_sprite_board(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_playing_for_sprite_main_player(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx);

s32 on_playing_for_sprite_board(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx);

void on_removing_for_sprite_main_player(Ptr _handlerObj, Canvas* _cvs, Sprite* _spr);

void on_removing_for_sprite_board(Ptr _handlerObj, Canvas* _cvs, Sprite* _spr);

void on_collide_for_sprite_main_player(Canvas* _cvs, Sprite* _spr, s32 _px, s32 _py);

void on_collide_for_sprite_board(Canvas* _cvs, Sprite* _spr, s32 _px, s32 _py);

void on_update_for_sprite_main_player(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);

void on_update_for_sprite_board(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);

s32 on_msg_proc_for_sprite_board_up(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_msg_proc_for_sprite_main_player_move(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

s32 on_msg_proc_for_sprite_main_player_jump(Ptr _receiver, Ptr _sender, u32 _msg, u32 _lparam, u32 _wparam, Ptr _extra);

#endif /* __LOGIC_H__ */
