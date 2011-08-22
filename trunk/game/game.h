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

#ifndef __GAME_H__
#define __GAME_H__

#include "age.h"

#define DEFAULT_LINE_UP_TIME 300
#define DEFAULT_LEVEL_DISTANCE 5

typedef enum AsciiHeroBoardType {
	AHBT_SOLID,
	AHBT_SPRING,
	AHBT_SERRATION,
	AHBT_OVERTURN,
	AHBT_L_SCROLL,
	AHBT_R_SCROLL,
	AHBT_COUNT,
} AsciiHeroBoardType;

typedef struct BoardInfo {
	const Str startFrame;
	const Str endFrame;
	struct {
		s32 probMin;
		s32 probMax;
		AsciiHeroBoardType type;
	};
} BoardInfo;

static const BoardInfo BOARD_INFO[AHBT_COUNT] = {
	{ "solid", "end_solid", 0, 29, AHBT_SOLID },
	{ "spring", "end_spring", 30, 39, AHBT_SPRING },
	{ "serration", "end_serration", 40, 54, AHBT_SERRATION },
	{ "overturn", "end_overturn", 55, 69, AHBT_OVERTURN },
	{ "l-scroll", "end_l-scroll", 70, 79, AHBT_L_SCROLL },
	{ "r-scroll", "end_r-scroll", 80, 89, AHBT_R_SCROLL },
};

typedef struct AsciiHeroGame {
	Sprite* main;
	Sprite* subsidiary;
	Sprite* boardTemplate;
	Sprite** boardPool;
	s32 boardPoolSize;
	s32 boardCount;
	s32 time;
	s32 lineUpTime;
	u32 lineCount;
	u32 levelDistance;
	u32 levelCount;
	bl levelGenerated;
	AsciiHeroBoardType (* generate_board_type)(void);
	Sprite* (* add_board_by_type)(AsciiHeroBoardType _type);
	s32 (* remove_board)(Sprite* _spr);
	s32 (* clear_board)(void);
} AsciiHeroGame;

AsciiHeroGame* game(void);

void init(void);

#endif /* __GAME_H__ */
