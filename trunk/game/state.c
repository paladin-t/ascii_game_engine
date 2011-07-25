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

#include "state.h"
#include "game.h"

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

s32 state_show_logo(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
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

				draw_string(AGE_CVS, 0, 28, 16,
					(_time % 1200 < 600) && (state == _S_WAITING) ?
						"Press OK key to continue" :
						"                        "
				);

				if(state != _S_WAITING) {
					goto _exit;
				}
			}
		case _S_MENU:
			{
				{
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

				if(state == _S_MENU) {
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
							set_canvas_controller(AGE_CVS, state_main);
							clear_screen(AGE_CVS);
						} else if(__m == 1) { /* rank */
							// TODO
						} else if(__m == 2) { /* settings */
							// TODO
						} else if(__m == 3) { /* about */
							// TODO
						} else if(__m == 4) { /* exit */
							exit_world();
						}
					}
				}
			}
			break;
	}

_exit:
	return result;
}

s32 state_main(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;

	return result;
}
