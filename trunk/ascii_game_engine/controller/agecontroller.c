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

#include "../common/ageallocator.h"
#include "../common/ageutil.h"
#include "../render/agerenderer.h"
#include "agecontroller.h"

void set_canvas_controller(Ptr _obj, control_proc _proc) {
	Canvas* cvs = (Canvas*)_obj;

	assert(cvs);

	cvs->control = _proc;
}

control_proc get_canvas_controller(Ptr _obj) {
	control_proc result = 0;
	Canvas* cvs = (Canvas*)_obj;

	assert(cvs);

	result = cvs->control;

	return result;
}

void set_sprite_controller(Ptr _obj, control_proc _proc) {
	Sprite* spr = (Sprite*)_obj;

	assert(spr);

	spr->control = _proc;
}

control_proc get_sprite_controller(Ptr _obj) {
	control_proc result = 0;
	Sprite* spr = (Sprite*)_obj;

	assert(spr);

	result = spr->control;

	return result;
}
