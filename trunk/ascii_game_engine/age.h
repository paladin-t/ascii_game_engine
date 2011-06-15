#ifndef __AGE_H__
#define __AGE_H__

#include "ageconfig.h"
#include "script/my_basic/my_basic.h"
#include "common/agetype.h"
#include "input/ageinput.h"
#include "render/agerenderer.h"

typedef struct {
	Ptr impl;
	Str ver;
	Size size;
} Canvas;

typedef struct {
	Ptr impl;
	Str ver;
} Input;

typedef struct {
	Ptr impl;
	Str ver;
} Renderer;

AGE_API u32 get_ver(void);
AGE_API const s8* get_ver_string(void);

#endif /* __AGE_H__ */
