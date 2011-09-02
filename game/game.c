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

#include <windows.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

#include "age.h"
#include "game.h"
#include "logic.h"
#include "renderer.h"
#include "script.h"
#include "state.h"

static AsciiHeroGame _game;

static void _on_exit(void) {
	s32 c = 0;

	game()->destroy_score_boards();
	game()->clear_board();

	destroy_world();

	c = _CrtDumpMemoryLeaks();

	if(0 != c) {
#ifdef _DEBUG
		_asm int 3
#else
		printf("Memory leak detected.\n");
		system("pause");
#endif
	}
}

static void _on_init(void) {
	s8 fb = game()->footBrush;
	memset(game(), 0, sizeof(AsciiHeroGame));
	game()->footBrush = fb;
	init();
	game()->create_score_boards();
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

void _create_score_boards(void) {
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
		set_sprite_position(AGE_CVS, game()->scoreBoard[i], GAME_AREA_WIDTH + i * 4 + 2, 0);
		set_sprite_physics_mode(AGE_CVS, game()->scoreBoard[i], PHYSICS_MODE_NULL);
		set_sprite_visible(AGE_CVS, game()->scoreBoard[i], FALSE);
	}
}

void _destroy_score_boards(void) {
	s32 i = 0;
	for(i = 0; i < SCORE_BOARD_SIZE; ++i) {
		destroy_sprite(AGE_CVS, game()->scoreBoard[i]);
	}
	memset(game()->scoreBoard, 0, sizeof(game()->scoreBoard));
}

void _set_score_board_value(u32 score) {
	s32 i = 0;
	s32 n = 0;
	s8 sf[AGE_STR_LEN];
	s8 ef[AGE_STR_LEN];
	while(score) {
		n = score % 10;
		sprintf(sf, "s%d", n);
		sprintf(ef, "e%d", n);
		play_sprite(AGE_CVS, game()->scoreBoard[i], sf, ef, FALSE, 0);
		++i;
	}
}

void _set_score_board_visible(bl vis) {
	s32 i = 0;
	for(i = 0; i < SCORE_BOARD_SIZE; ++i) {
		set_sprite_visible(AGE_CVS, game()->scoreBoard[i], vis);
	}
}

AsciiHeroGame* game(void) {
	return &_game;
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

s32 main(s32 argc, Str argv[]) {
	_CrtSetBreakAlloc(0);

	atexit(_on_exit);

	/* initialize */
	create_world();
	register_game_script_interfaces();
	config_world("data/config.bas");
	_on_init();
	/* the main loop, go! */
	run_world();
	/* destroy everything */
	destroy_world();

	return 0;
}
