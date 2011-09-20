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
#include "animation.h"
#include "game.h"
#include "logic.h"
#include "renderer.h"
#include "script.h"
#include "state.h"

static AsciiHeroGame _game;

static void _on_exit(void) {
	s32 c = 0;
	s32 i = 0;
	static bl destroyed = FALSE;

	if(!destroyed) {
		for(i = 0; i < AHAT_COUNT; ++i) {
			if(game()->audio[i]) {
				AGE_FREE(game()->audio[i]);
				game()->audio[i] = 0;
			}
		}
		game()->destroy_score_boards();
		game()->clear_board();

		amb_save_data("data/save.bas");

		destroy_world();

		destroyed = TRUE;
	}

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
	Str audio[AHAT_COUNT];
	s8 fb = game()->foot_brush;
	memcpy(audio, game()->audio, sizeof(game()->audio));
	memset(game(), 0, sizeof(AsciiHeroGame));
	memcpy(game()->audio, audio, sizeof(game()->audio));
	game()->foot_brush = fb;
	init();
	game()->create_score_boards();
	AGE_CVS->store_params = TRUE;

	SetConsoleTitleA("ASCII HERO GO!");

	set_s32_param(AGE_CVS_PAR, "HIGH_SCORE", 0);
	fensure("data/save.bas");
	amb_load_data("data/save.bas");
}

AsciiHeroGame* game(void) {
	return &_game;
}

s32 main(s32 argc, Str argv[]) {
	_CrtSetBreakAlloc(0);

	atexit(_on_exit);

	create_world();
	register_game_script_interfaces();
	config_world("data/config.bas");
	_on_init();

	run_world();

	_on_exit();

	return 0;
}
