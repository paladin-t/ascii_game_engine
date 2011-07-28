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
#include "../common/ageallocator.h"
#include "../common/agelist.h"
#include "../common/agehashtable.h"
#include "../common/ageparamset.h"
#include "../message/agemessage.h"
#include "../controller/agecontroller.h"

#define MAX_CACHED_FRAME_COUNT 16

/**
 * @brief named frame prefix, for data string parsing
 */
static const s8 NAMED_FRAME_PREFIX = '@';

/**
 * @brief default z-order
 */
static const s32 DEFAULT_Z_ORDER = 0x0FFFFFFF;

/**
 * @brief color used for erase a pixel
 */
static const Color ERASE_PIXEL_COLOR = -1;

/**
 * @brief shape used for erase a pixel
 */
static const s8 ERASE_PIXEL_SHAPE = ' ';

/**
 * @brief invalid frame index
 */
static const s32 INVALID_FRAME_INDEX = -1;

/**
 * @brief empty physics mode
 */
static const u32 PHYSICS_MODE_NULL = 0;

/**
 * @brief obstacle physics mode
 */
static const u32 PHYSICS_MODE_OBSTACLE = (1 << 0);

/**
 * @brief checker physics mode
 */
static const u32 PHYSICS_MODE_CHECKER = (1 << 1);

/**
 * @brief font structure
 */
typedef struct Font {
	Color color; /**< font color */
} Font;

struct Frame;
struct Sprite;
struct Canvas;

/**
 * @brief pixel structure
 */
typedef struct Pixel {
	/* common */
	struct Frame* parent; /**< parent frame object */
	s8 shape;             /**< shape data */
	Color color;          /**< color value */
	s32 zorder;           /**< z-order of this pixel */
	union {
		/* for sprite frame */
		struct {
			s8 brush;     /**< brush data, used with palete to paint a pixel */
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
 * @brief sprite playing event callback functor
 *
 * @param[in] _cvs     - canvas object
 * @param[in] _spr     - sprite object
 * @param[in] _begin   - begin frame name of this animation
 * @param[in] _end     - end frame name of this animation
 * @param[in] _currIdx - current frame index
 * @return - execution status
 */
typedef s32 (* SpritePlayingCallbackFunc)(struct Canvas* _cvs, struct Sprite* _spr, const Str _begin, const Str _end, s32 _currIdx);

/**
 * @brief time line structure
 */
typedef struct TimeLine {
	Frame* frames;                      /**< all frames */
	s32 frameCount;                     /**< frames count */
	s32 currentFrame;                   /**< current frame index */
	s32 lastFrame;                      /**< last frame index */
	ht_node_t* namedFrames;             /**< named frame information */
	Str beginName;                      /**< begin frame name */
	Str endName;                        /**< end frame name */
	s32 beginIndex;                     /**< begin frame index */
	s32 endIndex;                       /**< end frame index */
	bl loop;                            /**< whether loop between begin and end frame */
	SpritePlayingCallbackFunc callback; /**< sprite playing event callback functor */
} TimeLine;

/**
 * @brief customized animation structure
 */
typedef struct CustomAnimation {
	Ptr context;       /**< animation context */
	Destroyer destroy; /**< destroyer functor */
} CustomAnimation;

/**
 * @brief sprite collision callback functor
 *
 * @param[in] _cvs - canvas object
 * @param[in] _spr - sprite object
 * @param[in] _px  - x position of collided pixel in sprite
 * @param[in] _py  - y position of collided pixel in sprite
 */
typedef void (* SpriteCollisionCallbackFunc)(struct Canvas* _cvs, struct Sprite* _spr, s32 _px, s32 _py);

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
	struct Canvas* owner;                 /**< owner canvas object */
	Str name;                             /**< name */
	Point position;                       /**< position */
	Point oldPosition;                    /**< old position */
	Size frameSize;                       /**< size of each frame */
	TimeLine timeLine;                    /**< time line data */
	CustomAnimation customAnimation;      /**< customized animation data */
	f32 frameRate;                        /**< frame rate information */
	s32 frameTick;                        /**< frame updating time tick count */
	u32 physicsMode;                      /**< physics mode */
	SpriteCollisionCallbackFunc collided; /**< collided physics callback */
	MessageMap messageMap;                /**< message processing map */
	ControlProc control;                  /**< controlling functor, for motion controlling */
	SpriteUpdateFunc update;              /**< updating functor, for animation controlling */
	SpriteRenderFunc fireRender;          /**< fire rendering functor */
	SpriteRenderFunc postRender;          /**< post rendering functor */
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
	AgeParamSet* params;    /**< parameter set */
	Size size;              /**< canvas size */
	Pixel* pixels;          /**< frame buffer */
	ht_node_t* sprites;     /**< alive sprite objects */
	s32 frameRate;          /**< canvas frame rate, in millisecond */
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
 * @brief get the color of a pixel in a sprite
 *
 * @param[in] _cvs   - canvas object
 * @param[in] _spr   - sprite object
 * @param[in] _frame - frame index
 * @param[in] _x     - x offset of a pixel
 * @param[in] _y     - y offset of a pixel
 * @return - color of the appointed pixel
 */
AGE_API Color get_sprite_pixel_color(Canvas* _cvs, Sprite* _spr, s32 _frame, s32 _x, s32 _y);
/**
 * @brief set the color of a pixel in a sprite
 *
 * @param[in] _cvs   - canvas object
 * @param[in] _spr   - sprite object
 * @param[in] _frame - frame index
 * @param[in] _x     - x offset of a pixel
 * @param[in] _y     - y offset of a pixel
 * @param[in] _col   - color of the appointed pixel
 */
AGE_API void set_sprite_pixel_color(Canvas* _cvs, Sprite* _spr, s32 _frame, s32 _x, s32 _y, Color _col);

/**
 * @brief set position of a sprite
 *
 * @param[in] _cvs - canvas object
 * @param[in] _spr - sprite object
 * @param[in] _x   - x
 * @param[in] _y   - y
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl set_sprite_position(Canvas* _cvs, Sprite* _spr, s32 _x, s32 _y);
/**
 * @brief get position of a sprite
 *
 * @param[in] _cvs - canvas object
 * @param[in] _spr - sprite object
 * @param[out] _x  - pointer to output x
 * @param[out] _y  - pointer to output y
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl get_sprite_position(Canvas* _cvs, Sprite* _spr, s32* _x, s32* _y);

/**
 * @brief get the index of a given named frame
 *
 * @param[in] _cvs  - canvas object
 * @param[in] _spr  - sprite object
 * @param[in] _name - frame name
 * @return - found index, INVALID_FRAME_INDEX if not found
 */
AGE_API s32 get_named_frame_index(Canvas* _cvs, Sprite* _spr, const Str _name);
/**
 * @brief play an animation of a time line
 *
 * @param[in] _cvs   - canvas object
 * @param[in] _spr   - sprite object
 * @param[in] _begin - begin frame name of this animation
 * @param[in] _end   - end frame name of this animation
 * @param[in] _loop  - whether loop this animation
 * @param[in] _cb    - playing event callback functor
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl play_sprite(Canvas* _cvs, Sprite* _spr, const Str _begin, const Str _end, bl _loop, SpritePlayingCallbackFunc _cb);
/**
 * @brief stop an animation of a time line
 *
 * @param[in] _cvs    - canvas object
 * @param[in] _spr    - sprite object
 * @param[in] _stopAt - stop animation at which frame, pass INVALID_FRAME_INDEX to stop at current frame
 * @return - return TRUE if succeed, or FALSE if failed
 */
AGE_API bl stop_sprite(Canvas* _cvs, Sprite* _spr, s32 _stopAt);

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
 * @brief get the physics mode of a sprite object
 *
 * @param[in] _cvs - canvas object
 * @param[in] _spr - sprite object
 * @return - physics mode
 */
AGE_API u32 get_sprite_physics_mode(Canvas* _cvs, Sprite* _spr);
/**
 * @brief set the physics mode of a sprite object
 *
 * @param[in] _cvs  - canvas object
 * @param[in] _spr  - sprite object
 * @param[in] _mode - physics mode
 */
AGE_API void set_sprite_physics_mode(Canvas* _cvs, Sprite* _spr, u32 _mode);

/**
 * @brief draw a string on a canvas
 *
 * @param[in] _cvs  - canvas object
 * @param[in] _font - drawing font
 * @param[in] _x    - x position to draw
 * @param[in] _y    - y position to draw
 * @param[in] _text - text to draw
 */
AGE_API void draw_string(Canvas* _cvs, Font* _font, s32 _x, s32 _y, const Str _text, ...);

/**
 * @brief get a mapped color value
 *
 * @param[in] _index - color index
 * @return - mapped color value
 */
AGE_API Color get_mapped_color(s32 _index);
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

/**
 * @brief clear screen
 *
 * @param[in] _cvs - canvas object
 */
AGE_API void clear_screen(Canvas* _cvs);

#endif /* __AGE_RENDERER_H__ */
