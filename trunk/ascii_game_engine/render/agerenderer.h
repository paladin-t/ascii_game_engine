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

#ifndef __AGE_RENDERER_H__
#define __AGE_RENDERER_H__

#include "../ageconfig.h"
#include "../common/agetype.h"
#include "../common/agelist.h"
#include "../common/agehashtable.h"
#include "../message/agemessage.h"
#include "../controller/agecontroller.h"

#define MAX_CACHED_FRAME_COUNT 16

struct Frame;
struct Sprite;
struct Canvas;

/**
 * @brief pixel structure
 */
typedef struct Pixel {
	/* common */
	s8 shape;     /**< shape data */
	Color color;  /**< color value */
	union {
		/* for sprite frame */
		struct {
			s8 brush;   /**< brush data, used with palete to paint a pixel */
			s32 zorder; /**< z-order of this pixel */
		};
		/* for canvas frame buffer */
		struct {
			struct Frame* ownerFrames[MAX_CACHED_FRAME_COUNT]; /**< owner frames */
			s32 frameCount;                                    /**< owner frames count */
		};
	};
} Pixel;

/**
 * @brief frame structure
 */
typedef struct Frame {
	struct Sprite* parent; /**< parent sprite object */
	Pixel* tex;            /**< pixels */
} Frame;

/**
 * @brief sprite updating functor
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
typedef void (* SpriteUpdateFunc)(struct Canvas* _cvs, struct Sprite* _spr, s32 _elapsedTime);
/**
 * @brief sprite rendering functor
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
typedef void (* SpriteRenderFunc)(struct Canvas* _cvs, struct Sprite* _spr, s32 _elapsedTime);

/**
 * @brief sprite structure
 */
typedef struct Sprite {
	Str name;                     /**< name */
	struct Canvas* owner;         /**< owner canvas object */
	Point position;               /**< position */
	Size frameSize;               /**< size of each frame */
	Frame* frames;                /**< frames data */
	s32 frameCount;               /**< frames count */
	s32 currentFrame;             /**< current frame index */
	f32 frameRate;                /**< frame rate information */
	MessageMap messageMap;        /**< message processing map */
	ControlProc control;          /**< controlling functor */
	SpriteUpdateFunc update;      /**< updating functor */
	SpriteRenderFunc fireRender;  /**< fire rendering functor */
	SpriteRenderFunc postRender;  /**< post rendering functor */
} Sprite;

/**
 * @brief running context structure
 */
typedef struct RunningContext {
	s32 lastElapsedTime; /**< elapsed time since last frame */
	u32 lastLParam;      /**< first param of last message */
	u32 lastWParam;      /**< second param of last message */
	Ptr lastExtra;       /**< extra user defined data of last message */
	Color lastColor;     /**< color value since last draw call */
} RunningContext;

/**
 * @brief canvas structure
 */
typedef struct Canvas {
	Str name;               /**< name */
	Size size;              /**< canvas size */
	Pixel* pixels;          /**< frame buffer */
	ht_node_t* sprites;     /**< alive sprite objects */
	s32 frameRate;          /**< canvas frame rate */
	RunningContext context; /**< running context */
	ControlProc control;    /**< canvas controlling functor*/
} Canvas;

/**
 * @brief create a canvas object
 *
 * @param[in] _name - canvas name
 * @return - created canvas object
 */
AGE_API Canvas* create_canvas(const Str _name);
/**
 * @brief destroy a canvas object
 *
 * @param[in] _cvs - canvas object
 */
AGE_API void destroy_canvas(Canvas* _cvs);

/**
 * @brief set frame rate of a canvas
 *
 * @param[in] _cvs  - canvas object
 * @param[in] _rate - frame rate value
 */
AGE_API void set_frame_rate(Canvas* _cvs, s32 _rate);
/**
 * @brief get frame rate of a canvas
 *
 * @param[in] _cvs - canvas object
 * @return - frame rate value
 */
AGE_API s32 get_frame_rate(Canvas* _cvs);

/**
 * @brief update a canvas for a frame
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void update_canvas(Canvas* _cvs, s32 _elapsedTime);
/**
 * @brief render a canvas for a frame
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void render_canvas(Canvas* _cvs, s32 _elapsedTime);

/**
 * @brief get a sprite by name
 *
 * @param[in] _cvs        - canvas object
 * @param[in] _name       - sprite name
 * @return - corresponded sprite object, zero pointer for none
 */
AGE_API Sprite* get_sprite_by_name(Canvas* _cvs, const Str _name);
/**
 * @brief create a sprite object
 *
 * @param[in] _cvs        - canvas object
 * @param[in] _name       - sprite name
 * @param[in] _shapeFile  - shape data file name
 * @param[in] _brushFile  - brush data file name
 * @param[in] _paleteFile - palete data file name
 * @return - created sprite object
 */
AGE_API Sprite* create_sprite(Canvas* _cvs, const Str _name, const Str _shapeFile, const Str _brushFile, const Str _paleteFile);
/**
 * @brief destroy a sprite in a canvas
 *
 * @param[in] _cvs - canvas object
 * @param[in] _spr - sprite object
 */
AGE_API void destroy_sprite(Canvas* _cvs, Sprite* _spr);
/**
 * @brief destroy all sprites in a canvas
 *
 * @param[in] _cvs - canvas object
 */
AGE_API void destroy_all_sprites(Canvas* _cvs);
/**
 * @brief sprite updating
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void update_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);
/**
 * @brief fire sprite rendering
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void fire_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);
/**
 * @brief post sprite rendering
 *
 * @param[in] _cvs         - canvas object
 * @param[in] _spr         - sprite object
 * @param[in] _elapsedTime - elapsed time since last frame
 */
AGE_API void post_render_sprite(Canvas* _cvs, Sprite* _spr, s32 _elapsedTime);

/**
 * @brief set whether the console cursor is visible
 *
 * @param[in] _vis - whether the console cursor is visible
 */
AGE_API void set_cursor_visible(bl _vis);
/**
 * @brief set the console cursor posotion to a given place
 *
 * @param[in] _x - x
 * @param[in] _y - y
 */
AGE_API void goto_xy(s32 _x, s32 _y);
/**
 * @brief set the console output color
 *
 * @param[in] _cvs - canvas object
 * @param[in] _col - console output color
 */
AGE_API void set_color(Canvas* _cvs, Color _col);

#endif /* __AGE_RENDERER_H__ */
