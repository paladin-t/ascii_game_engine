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

#define _RAISING_RAIT 128

s32 state_show_logo(Ptr _obj, const Str _name, s32 _elapsedTime, u32 _lparam, u32 _wparam, Ptr _extra) {
	s32 result = 0;
	s32 _x = 16;
	static s32 state = _S_DEFAULT;

	s32 i = 0; s32 j = 0;

	static s32 _y = CANVAS_HEIGHT + 3;
	static s32 _time = _RAISING_RAIT;
	_time += _elapsedTime;

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
				draw_string(AGE_CVS, 0, 27, 16,
					(_time % 1200 < 600) ?
						"Press OK key to continue" :
						"                        "
				);
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
			}
			break;
	}

	return result;
}
