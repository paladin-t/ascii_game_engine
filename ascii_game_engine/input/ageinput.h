#ifndef __AGE_INPUT_H__
#define __AGE_INPUT_H__

#include "../ageconfig.h"
#include "../common/agetype.h"

typedef enum {
	KC_UP = 72,
	KC_DOWN = 80,
	KC_LEFT = 75,
	KC_RIGHT = 77,
	KC_OK = 13,
	KC_ESC = 27,
} KeyCodes;

AGE_API bl is_key_down(KeyCodes _keyCode);

#endif /* __AGE_INPUT_H__ */
