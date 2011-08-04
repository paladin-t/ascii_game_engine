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

#ifndef __GAME_H__
#define __GAME_H__

#include "age.h"

typedef enum AsciiHeroSpriteType {
	AHST_WALK,
	AHST_JUMP,
	AHST_HURT,
	AHST_FLIP,
	AHST_COUNT,
} AsciiHeroSpriteType;

typedef struct AsciiHeroGame {
	Sprite* main;
	Sprite* subsidiary;
	Sprite* spriteTemplates[AHST_COUNT];
	Sprite** spritePool;
	s32 spritePoolSize;
	s32 spriteCount;
	Sprite* (* add_sprite_by_type)(AsciiHeroSpriteType _type);
	void (* add_sprite)(Sprite* _spr);
	void (* remove_sprite)(Sprite* _spr);
} AsciiHeroGame;

AsciiHeroGame* game(void);

void init(void);

#endif /* __GAME_H__ */
