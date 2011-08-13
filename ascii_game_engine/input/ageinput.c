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

#if AGE_IMPL == AGE_IMPL_CONSOLE

#include "../common/ageutil.h"
#include "../common/ageallocator.h"
#include "ageinput.h"

typedef struct InputContext {
	bl keys[256];
} InputContext;

static s32 KEY_MAP[MAX_PLAYER_COUNT][KC_COUNT];

bl open_input(void) {
	bl result = TRUE;

	memset(&KEY_MAP, 0, sizeof(KEY_MAP));

	return result;
}

bl close_input(void) {
	bl result = TRUE;

	return result;
}

Ptr create_input_context(void) {
	InputContext* result = AGE_MALLOC(InputContext);

	return result;
}

void destroy_input_context(Ptr _ctx) {
	AGE_FREE(_ctx);
}

void update_input_context(Ptr _ctx) {
	InputContext* ctx = (InputContext*)_ctx;
	s32 key = 0;

	memset(ctx->keys, 0, sizeof(ctx->keys));
	while(kbhit()) {
		key = getch();
		if(224 == key) {
			key = getch();
		}
		ctx->keys[key] = TRUE;
	}
}

bl register_key_map(s32 _player, KeyIndex _keyIdx, s32 _keyCode) {
	bl result = TRUE;

	assert(_player >= 0 && _player < MAX_PLAYER_COUNT && _keyIdx >= 0 && _keyIdx < KC_COUNT);

	KEY_MAP[_player][_keyIdx] = _keyCode;

	return result;
}

bl is_key_down(Ptr _ctx, s32 _player, KeyIndex _keyIdx) {
	bl result = FALSE;
	s32 key = 0;
	InputContext* ctx = (InputContext*)_ctx;

	assert(_player >= 0 && _player < MAX_PLAYER_COUNT && _keyIdx >= 0 && _keyIdx < KC_COUNT);

	result = ctx->keys[KEY_MAP[_player][_keyIdx]] != 0;

	return result;
}

#endif /* AGE_IMPL == AGE_IMPL_CONSOLE */
