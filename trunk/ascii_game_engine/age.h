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

#ifndef __AGE_H__
#define __AGE_H__

#include "ageconfig.h"
#include "script/my_basic/my_basic.h"
#include "script/agescriptapi.h"
#include "common/agetype.h"
#include "common/agestringtable.h"
#include "common/ageallocator.h"
#include "common/ageutil.h"
#include "common/agelist.h"
#include "common/agehashtable.h"
#include "input/ageinput.h"
#include "render/agerenderer.h"
#include "message/agemessage.h"
#include "controller/agecontroller.h"

#ifndef AGE_WLD
#	define AGE_WLD get_world()
#endif
#ifndef AGE_CVS
#	define AGE_CVS get_world()->canvas
#endif
#ifndef AGE_IPT
#	define AGE_IPT get_world()->input
#endif

/**
 * @brief world object
 */
typedef struct World {
	Ptr input;                 /**< input context */
	Canvas* canvas;            /**< canvas object */
	mb_interpreter_t* script;  /**< global script object */
	bl running;                /**< whether the world is running, set to FALSE to exit the game */
} World;

/**
 * @brief get the version of current ascii game engine
 *
 * @return - version in integer format
 */
AGE_API u32 get_ver(void);
/**
 * @brief get the version of current ascii game engine
 *
 * @return - version in string format
 */
AGE_API const Str get_ver_string(void);

/**
 * @brief create the game world
 *
 * @return - the created world
 */
AGE_API World* create_world(void);
/**
 * @brief get the created singleton game world
 *
 * @return - the created singleton world
 */
AGE_API World* get_world(void);
/**
 * @brief destroy a created singleton game world
 */
AGE_API void destroy_world(void);

/**
 * @brief run a script to config the game world
 *
 * @param[in] _cfgFile  - configuration script file name
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl config_world(const Str _cfgFile);

/**
 * @brief run the world for one frame
 *
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API s32 run_world(void);
/**
 * @brief request to exit the game world
 */
AGE_API void exit_world(void);

/**
 * @brief run a script in the world context
 *
 * @param[in] _sptFile  - script file name
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl run_world_script(const Str _sptFile);
/**
 * @brief run a script in a new context
 *
 * @param[in] _sptFile  - script file name
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl run_new_script(const Str _sptFile);

#endif /* __AGE_H__ */
