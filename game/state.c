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
	s32 __w = game()->main->frame_size.w;
	s32 __h = game()->main->frame_size.h;
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
	if(state != _S_TATE_4 && state != _S_DEFAULT) {
		f.color = get_mapped_color(12);
		draw_string(AGE_CVS, &f, 31 + 6, 11, "'");
	}

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
		"     P L A Y     ",
		"H I G H S C O R E",
		"    A B O U T    ",
		"     E X I T     "
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
						if(game()->audio[AHAT_LOGO]) {
							play_sound_string(AGE_SND, game()->audio[AHAT_LOGO], ST_BGM, TRUE);
						}
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
						draw_string(AGE_CVS, (i == __m) ? &_f2 : &_f1, 31, 15 + i, "%s %s", (i == __m) ? ">" : " ", _MENU_TEXT[i]);
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
							AGE_CVS->prev_render = main_canvas_prev_render;
							AGE_CVS->post_render = main_canvas_post_render;
							clear_screen(AGE_CVS);
						} else if(__m == 1) { /* highscore */
							destroy_sprite(AGE_CVS, game()->main);
							destroy_sprite(AGE_CVS, game()->subsidiary);
							game()->main = game()->subsidiary = 0;
							stop_sound(AGE_SND, ST_BGM);
							set_canvas_controller(AGE_CVS, state_show_highscore);
							clear_screen(AGE_CVS);
						} else if(__m == 2) { /* about */
							set_str_param(AGE_CVS_PAR, "STATE_TRANS_DATA", "data/info.txt");
							destroy_sprite(AGE_CVS, game()->main);
							destroy_sprite(AGE_CVS, game()->subsidiary);
							game()->main = game()->subsidiary = 0;
							stop_sound(AGE_SND, ST_BGM);
							set_canvas_controller(AGE_CVS, state_text_list);
							clear_screen(AGE_CVS);
						} else if(__m == 3) { /* exit */
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
			remove_param(AGE_CVS_PAR, "STATE_TRANS_DATA");
			draw_string(AGE_CVS, 0, 0, 0, text);
			AGE_FREE(text);
			++state;
			break;
		case _S_MAIN:
			if(is_key_down(AGE_IPT, 0, KC_ESC) || is_key_down(AGE_IPT, 0, KC_OK)) {
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

s32 state_show_highscore(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
#define _S_DEFAULT 0
#define _S_MAIN 1
#define _S_BACK 2
	s32 result = 0;
	s32 hs = 0;
	static s8 buf[AGE_STR_LEN];
	static s32 state = _S_DEFAULT;

	update_input_context(AGE_IPT);

	switch(state) {
		case _S_DEFAULT:
			clear_screen(AGE_CVS);
			get_s32_param(AGE_CVS_PAR, "HIGH_SCORE", &hs);
			sprintf(buf, "Highscore: %04d", hs);
			++state;
			break;
		case _S_MAIN:
			if(is_key_down(AGE_IPT, 0, KC_ESC) || is_key_down(AGE_IPT, 0, KC_OK)) {
				++state;
			}

			draw_string(AGE_CVS, 0, 32, 13, buf);
			break;
		case _S_BACK:
			state = _S_DEFAULT;
			init();
			set_canvas_controller(AGE_CVS, state_show_logo);
			clear_screen(AGE_CVS);
			memset(buf, 0, _countof(buf));
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
	s32 hs = 0;
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
			game()->board_template = create_sprite(
				AGE_CVS,
				"board_template",
				"data/sprite/board_shape.txt",
				"data/sprite/board_brush.txt",
				"data/sprite/board_palete.txt"
			);
			pause_sprite(AGE_CVS, game()->main);
			pause_sprite(AGE_CVS, game()->board_template);
			set_sprite_position(AGE_CVS, game()->main, 0, GAME_AREA_TOP - game()->main->frame_size.h + 1);
			set_sprite_visible(AGE_CVS, game()->main, FALSE);
			set_sprite_visible(AGE_CVS, game()->board_template, FALSE);
			set_sprite_controller(game()->main, on_ctrl_for_sprite_main_player);
			set_sprite_controller(game()->board_template, on_ctrl_for_sprite_board);
			register_message_proc(&game()->main->message_map, MSG_MOVE, on_msg_proc_for_sprite_main_player_move);
			register_message_proc(&game()->main->message_map, MSG_JUMP, on_msg_proc_for_sprite_main_player_jump);
			set_sprite_physics_mode(AGE_CVS, game()->main, PHYSICS_MODE_OBSTACLE | PHYSICS_MODE_CHECKER);
			game()->main->object_removed = on_removing_for_sprite_main_player;
			game()->main->collided = on_collide_for_sprite_main_player;
			game()->main->update = 0;
			game()->main->userdata.data = create_player_userdata();
			game()->main->userdata.destroy = destroy_player_userdata;
			((PlayerUserdata*)(game()->main->userdata.data))->fall_time = DEFAULT_FALL_TIME;
			game()->board_template->object_removed = on_removing_for_sprite_board;
			game()->board_template->collided = on_collide_for_sprite_board;
			game()->board_template->update = on_update_for_sprite_board;
			game()->set_score_board_visible(TRUE);
			game()->set_score_board_value(0);
			draw_string(AGE_CVS, 0, GAME_AREA_WIDTH + 2, 0, "Score:");
			++state;
			break;
		case _S_MAIN:
			{
				BoardUserdata* bu = 0;
				AsciiHeroBoardType bt = AHBT_SOLID;
				Sprite* bd = 0;
				s32 left = 0;

				if(game()->game_over) {
					++state;
				}

				if(is_key_down(AGE_IPT, 0, KC_ESC)) {
					++state;
				}

				game()->time += _elapsedTime;
				if(game()->time >= game()->line_up_time) {
					game()->time -= game()->line_up_time;
					++game()->line_count;
					if(!(game()->line_count % game()->level_distance) && !game()->level_generated) {
						if(game()->audio[AHAT_TICK]) {
							play_sound_string(AGE_SND, game()->audio[AHAT_TICK], ST_SFX, FALSE);
						}

						bt = game()->generate_board_type();
						left = age_rand(GAME_AREA_LEFT + 1, GAME_AREA_RIGHT - 1 - 10);
						if(game()->line_count == game()->level_distance) {
							bt = AHBT_SOLID;
							game()->main->update = on_update_for_sprite_main_player;
							set_sprite_position(AGE_CVS, game()->main, left + 3, GAME_AREA_TOP - game()->main->frame_size.h + 1);
							set_sprite_visible(AGE_CVS, game()->main, TRUE);
						}
						bd = game()->add_board_by_type(bt);
						bu = (BoardUserdata*)(bd->userdata.data);
						bu->type = bt;
						set_sprite_position(AGE_CVS, bd, left, CANVAS_HEIGHT + 1);
						set_sprite_physics_mode(AGE_CVS, bd, PHYSICS_MODE_OBSTACLE | PHYSICS_MODE_CHECKER);
						game()->level_generated = TRUE;

						if(game()->line_up_time > MIN_LINE_UP_TIME) {
							game()->line_up_time -= 3;
							if(game()->line_up_time < MIN_LINE_UP_TIME) {
								game()->line_up_time = MIN_LINE_UP_TIME;
							}
						}
					} else if(game()->line_count % game()->level_distance) {
						game()->level_generated = FALSE;
					}
					send_message_to_canvas(AGE_CVS, 0, MSG_BOARD_UP, 0, 0, 0);
				}
			}
			break;
		case _S_BACK:
			state = _S_DEFAULT;
			get_s32_param(AGE_CVS_PAR, "HIGH_SCORE", &hs);
			if((s32)game()->level_count > hs) {
				hs = game()->level_count;
				set_s32_param(AGE_CVS_PAR, "HIGH_SCORE", hs);
			}
			stop_sound(AGE_SND, ST_BGM);
			destroy_sprite(AGE_CVS, game()->main);
			destroy_sprite(AGE_CVS, game()->board_template);
			game()->clear_board();
			game()->board_template = 0;
			set_canvas_controller(AGE_CVS, 0);
			AGE_CVS->prev_render = 0;
			AGE_CVS->post_render = 0;
			if(game()->game_over) {
				if(game()->audio[AHAT_GAME_OVER]) {
					play_sound_string(AGE_SND, game()->audio[AHAT_GAME_OVER], ST_BGM, FALSE);
				}
				game()->game_over = FALSE;
				set_canvas_controller(AGE_CVS, state_game_over);
			} else {
				game()->set_score_board_visible(FALSE);
				game()->set_score_board_value(0);
				clear_screen(AGE_CVS);
				init();
				set_canvas_controller(AGE_CVS, state_show_logo);
			}
			break;
	};

	return result;
#undef _S_DEFAULT
#undef _S_MAIN
#undef _S_BACK
}

s32 state_game_over(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
#define _S_DEFAULT 0
#define _S_INTRO_0 1
#define _S_INTRO_1 2
#define _S_MAIN 3
#define _S_BACK 4

#define _STEP_TIME 20
	s32 result = 0;
	static s32 state = _S_DEFAULT;
	static s32 x = GAME_AREA_LEFT + 1;
	static s32 y = GAME_AREA_TOP;
	static s32 time = 0;
	s32 i = 0;
	s32 j = 0;

	update_input_context(AGE_IPT);

	time += _elapsedTime;
	switch(state) {
		case _S_DEFAULT:
			++state;
			break;
		case _S_INTRO_0:
			if(time >= _STEP_TIME) {
				time -= _STEP_TIME;
				for(i = x; i < GAME_AREA_WIDTH - x + 1; ++i) {
					clear_pixel(AGE_CVS, i, y);
					clear_pixel(AGE_CVS, i, GAME_AREA_HEIGHT - 1 - y);
					put_char(AGE_CVS, 0, i, y, '*');
					put_char(AGE_CVS, 0, i, GAME_AREA_HEIGHT - 1 - y, '*');
				}
				for(j = y; j < GAME_AREA_HEIGHT - y; ++j) {
					clear_pixel(AGE_CVS, x, j);
					clear_pixel(AGE_CVS, GAME_AREA_WIDTH - x, j);
					put_char(AGE_CVS, 0, x, j, '*');
					put_char(AGE_CVS, 0, GAME_AREA_WIDTH - x, j, '*');
				}
				++x;
				++y;
				if(x >= (GAME_AREA_WIDTH - x) / 2 + 1 && y >= (GAME_AREA_HEIGHT - y) / 2 + 1) {
					++state;
					x = y = 0;
				}
			}
			break;
		case _S_INTRO_1:
			if(time >= _STEP_TIME) {
				time -= _STEP_TIME;
				for(i = x; i < GAME_AREA_WIDTH - x + 1; ++i) {
					clear_pixel(AGE_CVS, i, y);
					clear_pixel(AGE_CVS, i, GAME_AREA_HEIGHT - 1 - y);
					put_char(AGE_CVS, 0, i, y, ' ');
					put_char(AGE_CVS, 0, i, GAME_AREA_HEIGHT - 1 - y, ' ');
				}
				for(j = y; j < GAME_AREA_HEIGHT - y; ++j) {
					clear_pixel(AGE_CVS, x, j);
					clear_pixel(AGE_CVS, GAME_AREA_WIDTH - x, j);
					put_char(AGE_CVS, 0, x, j, ' ');
					put_char(AGE_CVS, 0, GAME_AREA_WIDTH - x, j, ' ');
				}
				++x;
				++y;
				if(x >= (GAME_AREA_WIDTH - x) / 2 + 1 && y >= (GAME_AREA_HEIGHT - y) / 2 + 1) {
					++state;
				}
			}
			break;
		case _S_MAIN:
			if(is_key_down(AGE_IPT, 0, KC_ESC) || is_key_down(AGE_IPT, 0, KC_OK)) {
				++state;
			}

			draw_string(AGE_CVS, 0, 27, 12, "Game Over");
			break;
		case _S_BACK:
			state = _S_DEFAULT;
			x = GAME_AREA_LEFT + 1;
			y = GAME_AREA_TOP;
			time = 0;
			game()->set_score_board_visible(FALSE);
			game()->set_score_board_value(0);
			clear_screen(AGE_CVS);
			init();
			set_canvas_controller(AGE_CVS, state_show_logo);
			break;
	};

	return result;
#undef _S_DEFAULT
#undef _S_INTRO_0
#undef _S_INTRO_1
#undef _S_MAIN
#undef _S_BACK
}
