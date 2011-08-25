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

#include "state.h"
#include "logic.h"
#include "renderer.h"
#include "game.h"

static void _draw_logo(s32 _time) {
	s32 i = 0;
	s32 j = 0;
	s32 __w = game()->main->frameSize.w;
	s32 __h = game()->main->frameSize.h;
	f32 __p = (_time % 3000) / 1000.0f;
	s32 __c = __w + __h;
	__c = (s32)(__c * __p);
	for(j = 0; j < __h; ++j) {
		for(i = 0; i < __w; ++i) {
			set_sprite_pixel_color(AGE_CVS, game()->main, 0, i, j, get_mapped_color(8));
		}
	}
	j = 0;
	for(i = __c; i >= 0; --i) {
		set_sprite_pixel_color(AGE_CVS, game()->main, 0, i, j, get_mapped_color(15));
		++j;
	}
}

s32 state_show_splash(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
#define _S_DEFAULT 0
#define _S_TATE_1 1
#define _S_TATE_2 2
#define _S_TATE_3 3
#define _S_TATE_4 4
	static const Str _SPLASH = "A Tony's Toy Game";
	static const Color _COLORS[] = { 0, 7, 8, 15 };
	static const s32 _PHASE_TIME = 800;
	static Font f;

	s32 result = 0;
	static s32 state = _S_DEFAULT;
	static s32 time = 0;
	static s32 ci = 0;

	time += _elapsedTime;

	switch(state) {
		case _S_DEFAULT:
			{
				if(time >= _PHASE_TIME) {
					ci = 1;
					++state;
				} else {
					if(time % (s32)(_PHASE_TIME / 2.0f) < (s32)(_PHASE_TIME / 3.0f)) {
						ci = age_rand(0, _countof(_COLORS));
					}
				}
			}
			break;
		case _S_TATE_1:
			{
				if(time >= _PHASE_TIME * 2) {
					++state;
				}
			}
			break;
		case _S_TATE_2:
			{
				if(time >= _PHASE_TIME * 3) {
					++state;
				} else {
					if(time % (s32)(_PHASE_TIME / 3.0f) < (s32)(_PHASE_TIME / 5.0f)) {
						ci = 3;
					}
				}
			}
			break;
		case _S_TATE_3:
			{
				if(time >= _PHASE_TIME * 4) {
					ci = 0;
					++state;
				} else {
					if(time % (s32)(_PHASE_TIME / 2.0f) < (s32)(_PHASE_TIME / 3.0f)) {
						ci = age_rand(0, _countof(_COLORS));
					}
				}
			}
			break;
		case _S_TATE_4:
			{
				if(time >= _PHASE_TIME * 5) {
					state = _S_DEFAULT;
					set_canvas_controller(AGE_CVS, state_show_logo);
				}
			}
			break;
	}

	f.color = get_mapped_color(_COLORS[ci]);
	draw_string(AGE_CVS, &f, 31, 11, _SPLASH);

	return result;
#undef _S_DEFAULT
#undef _S_TATE_1
#undef _S_TATE_2
#undef _S_TATE_3
#undef _S_TATE_4
}

s32 state_show_logo(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
#define _S_DEFAULT 0
#define _S_WAITING 1
#define _S_MENU 2
#define _RAISING_RAIT 128
	static const Str _MENU_TEXT[] = {
		"    P L A Y    ",
		"    R A N K    ",
		"S E T T I N G S",
		"   A B O U T   ",
		"    E X I T    "
	};

	s32 result = 0;
	s32 _x = 17;
	static s32 state = _S_DEFAULT;

	static Font _f1, _f2;

	s32 i = 0; s32 j = 0;

	static s32 _y = CANVAS_HEIGHT + 3;
	static s32 _time = _RAISING_RAIT;
	_time += _elapsedTime;

	_f1.color = get_mapped_color(8);
	_f2.color = get_mapped_color(15);

	update_input_context(AGE_IPT);

	switch(state) {
		case _S_DEFAULT:
			{
				if(_time >= _RAISING_RAIT) {
					_time -= _RAISING_RAIT;
					--_y;
					set_sprite_visible(AGE_CVS, game()->main, TRUE);
					set_sprite_visible(AGE_CVS, game()->subsidiary, TRUE);
					set_sprite_position(AGE_CVS, game()->main, _x, _y);
					set_sprite_position(AGE_CVS, game()->subsidiary, 0, _y + 20);
					if(_y < 3) {
						_y = CANVAS_HEIGHT + 3;
						_time = 0;
						++state;
						play_sound_string(AGE_SND, "DEDEC>A<C>A<EDEDEC>A<C>A<EPPPP", ST_BGM, TRUE);
					}
				}
			}
			break;
		case _S_WAITING:
			{
				if(is_key_down(AGE_IPT, 0, KC_OK)) {
					++state;
				}

				_draw_logo(_time);

				draw_string(AGE_CVS, 0, 28, 16,
					(_time % 1200 < 600) && (state == _S_WAITING) ?
						"Press OK key to continue" :
						"                        "
				);
			}
			break;
		case _S_MENU:
			{
				_draw_logo(_time);

				{
					static s32 __m = 0;
					for(i = 0; i < _countof(_MENU_TEXT); ++i) {
						draw_string(AGE_CVS, (i == __m) ? &_f2 : &_f1, 32, 15 + i, "%s %s", (i == __m) ? ">" : " ", _MENU_TEXT[i]);
					}

					if(is_key_down(AGE_IPT, 0, KC_UP)) {
						--__m;
						if(__m < 0) {
							__m = _countof(_MENU_TEXT) - 1;
						}
					} else if(is_key_down(AGE_IPT, 0, KC_DOWN)) {
						++__m;
						if(__m >= _countof(_MENU_TEXT)) {
							__m = 0;
						}
					} else if(is_key_down(AGE_IPT, 0, KC_OK)) {
						if(__m == 0) { /* play */
							destroy_sprite(AGE_CVS, game()->main);
							destroy_sprite(AGE_CVS, game()->subsidiary);
							game()->main = game()->subsidiary = 0;
							stop_sound(AGE_SND, ST_BGM);
							set_canvas_controller(AGE_CVS, state_main);
							AGE_CVS->prevRender = main_canvas_prev_render;
							AGE_CVS->postRender = main_canvas_post_render;
							clear_screen(AGE_CVS);
						} else if(__m == 1) { /* rank */
							// TODO
						} else if(__m == 2) { /* settings */
							// TODO
						} else if(__m == 3) { /* about */
							set_str_param(AGE_CVS_PAR, "STATE_TRANS_DATA", "data/info.txt");
							destroy_sprite(AGE_CVS, game()->main);
							destroy_sprite(AGE_CVS, game()->subsidiary);
							game()->main = game()->subsidiary = 0;
							stop_sound(AGE_SND, ST_BGM);
							set_canvas_controller(AGE_CVS, state_text_list);
							clear_screen(AGE_CVS);
						} else if(__m == 4) { /* exit */
							exit_world();
						}
						state = _S_DEFAULT;
						__m = 0;
					}
				}
			}
			break;
	}

	return result;
#undef _S_DEFAULT
#undef _S_WAITING
#undef _S_MENU
#undef _RAISING_RAIT
}

s32 state_text_list(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
#define _S_DEFAULT 0
#define _S_MAIN 1
#define _S_BACK 2
	s32 result = 0;
	static s32 state = _S_DEFAULT;
	static Str text = 0;
	Str data = 0;

	update_input_context(AGE_IPT);

	switch(state) {
		case _S_DEFAULT:
			clear_screen(AGE_CVS);
			get_str_param(AGE_CVS_PAR, "STATE_TRANS_DATA", &data);
			text = freadall(data);
			draw_string(AGE_CVS, 0, 0, 0, text);
			AGE_FREE(text);
			++state;
			break;
		case _S_MAIN:
			if(is_key_down(AGE_IPT, 0, KC_ESC)) {
				++state;
			}
			break;
		case _S_BACK:
			state = _S_DEFAULT;
			init();
			set_canvas_controller(AGE_CVS, state_show_logo);
			clear_screen(AGE_CVS);
			break;
	};

	return result;
#undef _S_DEFAULT
#undef _S_MAIN
#undef _S_BACK
}

s32 state_main(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
#define _S_DEFAULT 0
#define _S_MAIN 1
#define _S_BACK 2
	s32 result = 0;
	static s32 state = _S_DEFAULT;

	update_input_context(AGE_IPT);

	switch(state) {
		case _S_DEFAULT:
			game()->main = create_sprite(
				AGE_CVS,
				"ascii_hero",
				"data/sprite/ascii_hero_shape.txt",
				"data/sprite/ascii_hero_brush.txt",
				"data/sprite/ascii_hero_palete.txt"
			);
			game()->boardTemplate = create_sprite(
				AGE_CVS,
				"board_template",
				"data/sprite/board_shape.txt",
				"data/sprite/board_brush.txt",
				"data/sprite/board_palete.txt"
			);
			set_sprite_visible(AGE_CVS, game()->main, FALSE);
			set_sprite_visible(AGE_CVS, game()->boardTemplate, FALSE);
			set_sprite_controller(game()->main, on_ctrl_for_sprite_main_player);
			set_sprite_controller(game()->boardTemplate, on_ctrl_for_sprite_board);
			game()->main->objectRemoved = on_removing_for_sprite_main_player;
			game()->main->collided = on_collide_for_sprite_main_player;
			game()->main->update = on_update_for_sprite_main_player;
			game()->main->userdata.data = create_player_userdata();
			game()->main->userdata.destroy = destroy_player_userdata;
			((PlayerUserdata*)(game()->main->userdata.data))->fallTime = DEFAULT_FALL_TIME;
			game()->boardTemplate->objectRemoved = on_removing_for_sprite_board;
			game()->boardTemplate->collided = on_collide_for_sprite_board;
			game()->boardTemplate->update = on_update_for_sprite_board;
			++state;
			break;
		case _S_MAIN:
			{
				AsciiHeroBoardType bt = AHBT_SOLID;
				Sprite* bd = 0;
				s32 left = 0;

				if(is_key_down(AGE_IPT, 0, KC_ESC)) {
					++state;
				}

				game()->time += _elapsedTime;
				if(game()->time >= game()->lineUpTime) {
					game()->time -= game()->lineUpTime;
					++game()->lineCount;
					if(!(game()->lineCount % game()->levelDistance) && !game()->levelGenerated) {
						bt = game()->generate_board_type();
						bd = game()->add_board_by_type(bt);
						left = age_rand(GAME_AREA_LEFT + 1, GAME_AREA_RIGHT - 1 - bd->frameSize.w);
						set_sprite_position(AGE_CVS, bd, left, CANVAS_HEIGHT + 1);
						game()->levelGenerated = TRUE;
						if(game()->lineCount == game()->levelDistance) {
							set_sprite_position(AGE_CVS, game()->main, left + 2, GAME_AREA_TOP - game()->main->frameSize.h + 1);
							set_sprite_visible(AGE_CVS, game()->main, TRUE);
						}
					} else if(game()->lineCount % game()->levelDistance) {
						game()->levelGenerated = FALSE;
					}
					send_message_to_canvas(AGE_CVS, 0, MSG_BOARD_UP, 0, 0, 0);
				}
			}
			break;
		case _S_BACK:
			state = _S_DEFAULT;
			stop_sound(AGE_SND, ST_BGM);
			destroy_sprite(AGE_CVS, game()->main);
			destroy_sprite(AGE_CVS, game()->boardTemplate);
			game()->clear_board();
			game()->boardTemplate = 0;
			set_canvas_controller(AGE_CVS, 0);
			AGE_CVS->prevRender = 0;
			AGE_CVS->postRender = 0;
			clear_screen(AGE_CVS);
			init();
			set_canvas_controller(AGE_CVS, state_show_logo);
			break;
	};

	return result;
#undef _S_DEFAULT
#undef _S_MAIN
#undef _S_BACK
}
