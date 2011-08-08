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

static AsciiHeroGame _game;

static void _on_exit(void) {
	s32 c = 0;

	destroy_world();

	c = _CrtDumpMemoryLeaks();

	if(0 != c) {
		_asm int 3
	}
}

static void _on_init(void) {
	memset(game(), 0, sizeof(AsciiHeroGame));
	init();
}

static s32 _canvasControlProc(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	Point pos = { 0, 0 };
	get_sprite_position(AGE_CVS, game()->main, &pos.x, &pos.y);

	update_input_context(AGE_IPT);

	if(is_key_down(AGE_IPT, 0, KC_ESC)) {
		exit_world();
	}
	if(is_key_down(AGE_IPT, 0, KC_UP)) {
		--pos.y;
		set_sprite_position(AGE_CVS, game()->main, pos.x, pos.y);
	} else if(is_key_down(AGE_IPT, 0, KC_DOWN)) {
		++pos.y;
		set_sprite_position(AGE_CVS, game()->main, pos.x, pos.y);
	}
	if(is_key_down(AGE_IPT, 0, KC_LEFT)) {
		--pos.x;
		set_sprite_position(AGE_CVS, game()->main, pos.x, pos.y);
	} else if(is_key_down(AGE_IPT, 0, KC_RIGHT)) {
		++pos.x;
		set_sprite_position(AGE_CVS, game()->main, pos.x, pos.y);
	}

	return result;
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

	set_canvas_controller(AGE_CVS, state_show_logo);
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
