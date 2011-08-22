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

#ifndef __AGE_AUDIO_H__
#define __AGE_AUDIO_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "../common/ageallocator.h"

/**
 * @brief sound context
 */
typedef struct SoundContext {
	Str sequence; /**< sound command sequence */
	s32 position; /**< playing position */
	s32 time;     /**< audio updated time count */
	Ptr bgm;      /**< bgm thread information */
	Ptr sfx;      /**< sfx thread information */
} SoundContext;

/**
 * @brief sound types
 */
typedef enum SoundType {
	ST_BGM,   /**< background */
	ST_SFX,   /**< effect */
	ST_COUNT, /**< type count */
} SoundType;

/**
 * @brief create a sound system context
 *
 * @return - the created sound system context
 */
AGE_API SoundContext* create_sound_context(void);
/**
 * @brief destroy a created sound system context
 *
 * @return - the created sound system context
 */
AGE_API void destroy_sound_context(SoundContext* _cnt);

/**
 * @brief update audio system
 *
 * @param[in] _cnt         - sound system context
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void update_sound(SoundContext* _cnt, s32 _elapsedTime);

/**
 * @brief play a sound sequence
 *
 * @param[in] _cnt  - sound system context
 * @param[in] _seq  - sequenced sound commands
 * @param[in] _type - type to be operated
 * @param[in] _loop - loop this sound or not
 */
AGE_API void play_sound_string(SoundContext* _cnt, const Str _seq, SoundType _type, bl _loop);
/**
 * @brief stop a sound sequence
 *
 * @param[in] _cnt  - sound system context
 * @param[in] _type - type to be operated
 */
AGE_API void stop_sound(SoundContext* _cnt, SoundType _type);

#endif /* __AGE_AUDIO_H__ */
