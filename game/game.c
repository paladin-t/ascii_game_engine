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

#include <windows.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

#include "age.h"
#include "game.h"
#include "state.h"
#include "logic.h"

static AsciiHeroGame _game;

static void _on_exit(void) {
	s32 c = 0;

	if(game()->clear_board) {
		game()->clear_board();
	}
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
	memset(game(), 0, sizeof(AsciiHeroGame));
	init();
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
		AGE_REALLOC_N(Sprite*, game()->boardPool, game()->boardCount);
	}
	sprintf(newName, "board_%u", boardId++);
	result = clone_sprite(AGE_CVS, game()->boardTemplate->name, newName);
	game()->boardPool[game()->boardCount - 1] = result;
	play_sprite(
		AGE_CVS,
		result,
		BOARD_INFO[_type].startFrame,
		BOARD_INFO[_type].endFrame,
		FALSE,
		on_playing_for_sprite_board
	);
	pause_sprite(AGE_CVS, result);

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
		AGE_FREE_N(game()->boardPool);
	}

	return result;
}

AsciiHeroGame* game(void) {
	return &_game;
}

void init(void) {
	game()->levelDistance = DEFAULT_LEVEL_DISTANCE;

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

	set_canvas_controller(AGE_CVS, state_show_logo);

	game()->generate_board_type = _generate_board_type;
	game()->add_board_by_type = _add_board_by_type;
	game()->remove_board = _remove_board;
	game()->clear_board = _clear_board;

	game()->time = 0;
	game()->lineUpTime = DEFAULT_LINE_UP_TIME;
}

s32 main(s32 argc, Str argv[]) {
	_CrtSetBreakAlloc(0);

	atexit(_on_exit);

	/* initialize */
	create_world();
	config_world("data/config.bas");
	_on_init();
	/* the main loop, go! */
	run_world();
	/* destroy everything */
	destroy_world();

	return 0;
}
